import {
  CanActivate,
  ExecutionContext,
  ForbiddenException,
  Injectable,
} from '@nestjs/common';
import { Request } from 'express';

const SAFE_METHODS = new Set(['GET', 'HEAD', 'OPTIONS']);

@Injectable()
export class CsrfGuard implements CanActivate {
  canActivate(context: ExecutionContext): boolean {
    const request = context.switchToHttp().getRequest<Request>();

    if (SAFE_METHODS.has(request.method)) {
      return true;
    }

    const sessionToken = request.session.csrfToken;
    const requestToken = request.header('x-csrf-token');

    if (!sessionToken || !requestToken || sessionToken !== requestToken) {
      throw new ForbiddenException('Invalid CSRF token');
    }

    return true;
  }
}
