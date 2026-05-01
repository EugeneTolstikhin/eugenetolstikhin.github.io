import { Injectable } from '@nestjs/common';
import { randomBytes, scrypt, timingSafeEqual } from 'crypto';
import { promisify } from 'util';

const scryptAsync = promisify(scrypt);
const KEY_LENGTH = 64;

@Injectable()
export class PasswordService {
  async hash(secret: string) {
    const salt = randomBytes(16).toString('hex');
    const derivedKey = (await scryptAsync(secret, salt, KEY_LENGTH)) as Buffer;

    return `scrypt:${salt}:${derivedKey.toString('hex')}`;
  }

  async verify(secret: string, storedHash: string) {
    const [algorithm, salt, hash] = storedHash.split(':');

    if (algorithm !== 'scrypt' || !salt || !hash) {
      return false;
    }

    const derivedKey = (await scryptAsync(secret, salt, KEY_LENGTH)) as Buffer;
    const storedKey = Buffer.from(hash, 'hex');

    return (
      derivedKey.length === storedKey.length &&
      timingSafeEqual(derivedKey, storedKey)
    );
  }
}
