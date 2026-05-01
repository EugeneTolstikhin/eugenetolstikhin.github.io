import { Injectable } from '@nestjs/common';
import { createHash, randomInt } from 'crypto';

@Injectable()
export class OtpService {
  generateCode() {
    return randomInt(0, 1_000_000).toString().padStart(6, '0');
  }

  hashCode(code: string) {
    return createHash('sha256').update(code).digest('hex');
  }
}
