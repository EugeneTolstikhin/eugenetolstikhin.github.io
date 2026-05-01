import {
  Body,
  Controller,
  Post,
  Req,
  UnauthorizedException,
  UseGuards,
} from '@nestjs/common';
import {
  ApiBody,
  ApiCookieAuth,
  ApiCreatedResponse,
  ApiOperation,
  ApiSecurity,
  ApiTags,
  ApiUnauthorizedResponse,
} from '@nestjs/swagger';
import { randomBytes } from 'crypto';
import { Request } from 'express';
import { CsrfGuard } from '../security/csrf.guard';
import { AuthService } from './auth.service';
import { LoginDto } from './dto/login.dto';
import { VerifyOtpDto } from './dto/verify-otp.dto';
import { SessionAuthGuard } from './guards/session-auth.guard';

@Controller('auth')
@ApiTags('auth')
export class AuthController {
  constructor(private readonly authService: AuthService) {}

  @Post('login')
  @ApiOperation({ summary: 'Start password login and send an email OTP' })
  @ApiBody({ type: LoginDto })
  @ApiCreatedResponse({
    description: 'Password accepted; OTP challenge stored in the session.',
    schema: {
      example: {
        mfaRequired: true,
        delivery: 'email',
        email: 'admin@example.com',
        expiresInSeconds: 300,
      },
    },
  })
  @ApiUnauthorizedResponse({ description: 'Invalid email or password.' })
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
  @ApiOperation({ summary: 'Complete MFA and create an authenticated session' })
  @ApiCookieAuth()
  @ApiBody({ type: VerifyOtpDto })
  @ApiCreatedResponse({
    description: 'Authenticated user and CSRF token for unsafe requests.',
    schema: {
      example: {
        id: '9d8cb599-4c4a-4fd0-85f7-89dc2fbbf9fb',
        email: 'admin@example.com',
        role: 'ADMIN',
        csrfToken: '6b58d2fbd5d2474478e8a7f5b65d7f3c',
      },
    },
  })
  @ApiUnauthorizedResponse({ description: 'Missing pending login or invalid OTP.' })
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
  @ApiOperation({ summary: 'Destroy the current authenticated session' })
  @ApiCookieAuth()
  @ApiSecurity('csrf')
  @ApiCreatedResponse({ schema: { example: { ok: true } } })
  @ApiUnauthorizedResponse({ description: 'Login required.' })
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
