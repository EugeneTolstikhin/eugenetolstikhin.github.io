import { ConfigService } from '@nestjs/config';
import { UnauthorizedException } from '@nestjs/common';
import { UserRole } from '@prisma/client';
import { AuthService } from './auth.service';
import { OtpService } from './otp.service';

describe('AuthService', () => {
  const prisma = {
    user: {
      findUnique: jest.fn(),
    },
    otpChallenge: {
      create: jest.fn(),
      findUnique: jest.fn(),
      update: jest.fn(),
    },
  };
  const passwords = {
    verify: jest.fn(),
  };
  const otp = {
    generateCode: jest.fn(),
    hashCode: jest.fn(),
  } as unknown as OtpService;
  const mail = {
    send: jest.fn(),
  };
  const config = {
    get: jest.fn((_key: string, fallback: number) => fallback),
  } as unknown as ConfigService;

  let service: AuthService;

  beforeEach(() => {
    jest.clearAllMocks();
    service = new AuthService(
      prisma as never,
      passwords as never,
      otp,
      mail as never,
      config,
    );
  });

  it('rejects login when the password does not match', async () => {
    prisma.user.findUnique.mockResolvedValue({
      id: 'user-1',
      email: 'buyer@example.com',
      passwordHash: 'stored-hash',
    });
    passwords.verify.mockResolvedValue(false);

    await expect(
      service.beginLogin('buyer@example.com', 'wrong-password'),
    ).rejects.toBeInstanceOf(UnauthorizedException);
    expect(prisma.otpChallenge.create).not.toHaveBeenCalled();
    expect(mail.send).not.toHaveBeenCalled();
  });

  it('creates an OTP challenge and sends the code by email after password login', async () => {
    prisma.user.findUnique.mockResolvedValue({
      id: 'user-1',
      email: 'buyer@example.com',
      passwordHash: 'stored-hash',
    });
    passwords.verify.mockResolvedValue(true);
    jest.spyOn(otp, 'generateCode').mockReturnValue('123456');
    jest.spyOn(otp, 'hashCode').mockReturnValue('hashed-code');
    prisma.otpChallenge.create.mockResolvedValue({ id: 'challenge-1' });

    await expect(
      service.beginLogin('buyer@example.com', 'BuyerPass123!'),
    ).resolves.toMatchObject({
      challengeId: 'challenge-1',
      userId: 'user-1',
      email: 'buyer@example.com',
      expiresInSeconds: 300,
    });
    expect(prisma.otpChallenge.create).toHaveBeenCalledWith({
      data: expect.objectContaining({
        userId: 'user-1',
        codeHash: 'hashed-code',
      }),
      select: { id: true },
    });
    expect(mail.send).toHaveBeenCalledWith(
      expect.objectContaining({
        to: 'buyer@example.com',
        text: expect.stringContaining('123456'),
      }),
    );
  });

  it('increments attempts and rejects an invalid OTP', async () => {
    jest.spyOn(otp, 'hashCode').mockReturnValue('wrong-hash');
    prisma.otpChallenge.findUnique.mockResolvedValue({
      id: 'challenge-1',
      codeHash: 'expected-hash',
      expiresAt: new Date(Date.now() + 60_000),
      consumedAt: null,
      attempts: 0,
      user: {
        id: 'user-1',
        email: 'buyer@example.com',
        role: UserRole.BUYER,
      },
    });

    await expect(
      service.verifyOtp('challenge-1', '000000'),
    ).rejects.toBeInstanceOf(UnauthorizedException);
    expect(prisma.otpChallenge.update).toHaveBeenCalledWith({
      where: { id: 'challenge-1' },
      data: { attempts: { increment: 1 } },
    });
  });

  it('consumes a valid OTP and returns the session user', async () => {
    jest.spyOn(otp, 'hashCode').mockReturnValue('expected-hash');
    prisma.otpChallenge.findUnique.mockResolvedValue({
      id: 'challenge-1',
      codeHash: 'expected-hash',
      expiresAt: new Date(Date.now() + 60_000),
      consumedAt: null,
      attempts: 0,
      user: {
        id: 'user-1',
        email: 'buyer@example.com',
        role: UserRole.BUYER,
      },
    });

    await expect(service.verifyOtp('challenge-1', '123456')).resolves.toEqual({
      id: 'user-1',
      email: 'buyer@example.com',
      role: UserRole.BUYER,
    });
    expect(prisma.otpChallenge.update).toHaveBeenCalledWith({
      where: { id: 'challenge-1' },
      data: { consumedAt: expect.any(Date) },
    });
  });
});
