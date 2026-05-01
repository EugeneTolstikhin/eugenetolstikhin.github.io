import { NextFunction, Request, Response } from 'express';
import Redis from 'ioredis';

export type RateLimitOptions = {
  redis: Redis;
  windowSeconds: number;
  maxRequests: number;
  authWindowSeconds: number;
  authMaxRequests: number;
};

export function createRateLimitMiddleware(options: RateLimitOptions) {
  return async (request: Request, response: Response, next: NextFunction) => {
    const isAuthEndpoint =
      request.path === '/auth/login' || request.path === '/auth/verify-otp';
    const windowSeconds = isAuthEndpoint
      ? options.authWindowSeconds
      : options.windowSeconds;
    const maxRequests = isAuthEndpoint
      ? options.authMaxRequests
      : options.maxRequests;
    const bucket = Math.floor(Date.now() / (windowSeconds * 1000));
    const key = `rate-limit:${request.ip}:${request.method}:${request.path}:${bucket}`;

    try {
      const count = await options.redis.incr(key);

      if (count === 1) {
        await options.redis.expire(key, windowSeconds);
      }

      response.setHeader('RateLimit-Limit', maxRequests.toString());
      response.setHeader('RateLimit-Remaining', Math.max(maxRequests - count, 0).toString());
      response.setHeader('RateLimit-Reset', windowSeconds.toString());

      if (count > maxRequests) {
        response.status(429).json({ message: 'Too many requests' });
        return;
      }

      next();
    } catch {
      response.status(503).json({ message: 'Rate limiter unavailable' });
    }
  };
}
