import { Module } from '@nestjs/common';
import { MailModule } from '../mail/mail.module';
import { AuthController } from './auth.controller';
import { AuthService } from './auth.service';
import { OtpService } from './otp.service';
import { PasswordService } from './password.service';

@Module({
  imports: [MailModule],
  controllers: [AuthController],
  providers: [AuthService, OtpService, PasswordService],
})
export class AuthModule {}
