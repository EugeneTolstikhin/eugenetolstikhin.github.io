import {
  Body,
  Controller,
  Post,
  Req,
  UnauthorizedException,
  UseGuards,
} from '@nestjs/common';
import { randomBytes } from 'crypto';
import { Request } from 'express';
import { CsrfGuard } from '../security/csrf.guard';
import { AuthService } from './auth.service';
import { LoginDto } from './dto/login.dto';
import { VerifyOtpDto } from './dto/verify-otp.dto';
import { SessionAuthGuard } from './guards/session-auth.guard';

@Controller('auth')
export class AuthController {
  constructor(private readonly authService: AuthService) {}

  @Post('login')
  async login(@Body() dto: LoginDto, @Req() request: Request) {
    await this.regenerateSession(request);
    const challenge = await this.authService.beginLogin(dto.email, dto.password);
    request.session.pendingMfa = {
      challengeId: challenge.challengeId,
      userId: challenge.userId,
    };

    return {
      mfaRequired: true,
      delivery: 'email',
      email: challenge.email,
      expiresInSeconds: challenge.expiresInSeconds,
    };
  }

  @Post('verify-otp')
  async verifyOtp(@Body() dto: VerifyOtpDto, @Req() request: Request) {
    const challengeId = request.session.pendingMfa?.challengeId;

    if (!challengeId) {
      throw new UnauthorizedException('Login required');
    }

    const user = await this.authService.verifyOtp(challengeId, dto.code);
    await this.regenerateSession(request);
    request.session.csrfToken = randomBytes(32).toString('hex');
    request.session.user = user;

    return { ...user, csrfToken: request.session.csrfToken };
  }

  @Post('logout')
  @UseGuards(SessionAuthGuard, CsrfGuard)
  logout(@Req() request: Request) {
    request.session.destroy(() => undefined);
    return { ok: true };
  }

  private regenerateSession(request: Request) {
    return new Promise<void>((resolve, reject) => {
      request.session.regenerate((error) => {
        if (error) {
          reject(error);
          return;
        }

        resolve();
      });
    });
  }
}
