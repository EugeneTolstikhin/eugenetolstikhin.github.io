import { ApiProperty } from '@nestjs/swagger';
import { IsString, Matches } from 'class-validator';

export class VerifyOtpDto {
  @ApiProperty({ example: '123456', pattern: '^\\d{6}$' })
  @IsString()
  @Matches(/^\d{6}$/)
  code: string;
}
