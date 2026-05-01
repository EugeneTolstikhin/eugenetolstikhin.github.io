import { Injectable, UnauthorizedException } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { UserRole } from '@prisma/client';
import { MailService } from '../mail/mail.service';
import { PrismaService } from '../prisma/prisma.service';
import { OtpService } from './otp.service';
import { PasswordService } from './password.service';

type SessionUser = {
  id: string;
  email: string;
  role: UserRole;
};

@Injectable()
export class AuthService {
  constructor(
    private readonly prisma: PrismaService,
    private readonly passwords: PasswordService,
    private readonly otp: OtpService,
    private readonly mail: MailService,
    private readonly config: ConfigService,
  ) {}

  async beginLogin(email: string, password: string) {
    const user = await this.prisma.user.findUnique({
      where: { email },
      select: { id: true, email: true, passwordHash: true },
    });

    if (!user || !(await this.passwords.verify(password, user.passwordHash))) {
      throw new UnauthorizedException('Invalid email or password');
    }

    const code = this.otp.generateCode();
    const challenge = await this.prisma.otpChallenge.create({
      data: {
        userId: user.id,
        codeHash: this.otp.hashCode(code),
        expiresAt: new Date(Date.now() + this.otpTtlMs()),
      },
      select: { id: true },
    });

    await this.mail.send({
      to: user.email,
      subject: 'Your Small Car Auction sign-in code',
      text: `Your Small Car Auction one-time code is ${code}. It expires in ${Math.floor(
        this.otpTtlMs() / 60_000,
      )} minutes.`,
    });

    return {
      challengeId: challenge.id,
      userId: user.id,
      email: user.email,
      expiresInSeconds: Math.floor(this.otpTtlMs() / 1000),
    };
  }

  async verifyOtp(challengeId: string, code: string): Promise<SessionUser> {
    const challenge = await this.prisma.otpChallenge.findUnique({
      where: { id: challengeId },
      include: { user: { select: { id: true, email: true, role: true } } },
    });

    if (
      !challenge ||
      challenge.consumedAt ||
      challenge.expiresAt <= new Date() ||
      challenge.attempts >= this.maxOtpAttempts()
    ) {
      throw new UnauthorizedException('Invalid or expired OTP');
    }

    const codeMatches = challenge.codeHash === this.otp.hashCode(code);

    if (!codeMatches) {
      await this.prisma.otpChallenge.update({
        where: { id: challenge.id },
        data: { attempts: { increment: 1 } },
      });
      throw new UnauthorizedException('Invalid or expired OTP');
    }

    await this.prisma.otpChallenge.update({
      where: { id: challenge.id },
      data: { consumedAt: new Date() },
    });

    return challenge.user;
  }

  private otpTtlMs() {
    return this.config.get<number>('OTP_TTL_SECONDS', 300) * 1000;
  }

  private maxOtpAttempts() {
    return this.config.get<number>('OTP_MAX_ATTEMPTS', 5);
  }
}
