import { HttpException, HttpStatus, Injectable, NestMiddleware } from '@nestjs/common';
import { NextFunction, Request, Response } from 'express';

interface Bucket {
  count: number;
  resetAt: number;
}

@Injectable()
export class RateLimitMiddleware implements NestMiddleware {
  private readonly buckets = new Map<string, Bucket>();
  private readonly windowMs = Number(process.env.RATE_LIMIT_WINDOW_SECONDS ?? 60) * 1000;
  private readonly maxRequests = Number(process.env.RATE_LIMIT_MAX_REQUESTS ?? 120);

  use(request: Request, response: Response, next: NextFunction): void {
    const now = Date.now();
    const key = request.ip ?? request.socket.remoteAddress ?? 'unknown';
    const bucket = this.buckets.get(key);

    if (!bucket || bucket.resetAt <= now) {
      this.buckets.set(key, { count: 1, resetAt: now + this.windowMs });
      next();
      return;
    }

    bucket.count += 1;
    response.setHeader('Retry-After', Math.ceil((bucket.resetAt - now) / 1000));

    if (bucket.count > this.maxRequests) {
      throw new HttpException('Rate limit exceeded', HttpStatus.TOO_MANY_REQUESTS);
    }

    next();
  }
}
