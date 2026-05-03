import { HttpException, HttpStatus } from '@nestjs/common';
import { RateLimitMiddleware } from './rate-limit.middleware';

describe('RateLimitMiddleware', () => {
  const originalMax = process.env.RATE_LIMIT_MAX_REQUESTS;

  afterEach(() => {
    process.env.RATE_LIMIT_MAX_REQUESTS = originalMax;
  });

  it('throws a structured Nest exception after the configured request limit', () => {
    process.env.RATE_LIMIT_MAX_REQUESTS = '1';
    const middleware = new RateLimitMiddleware();
    const request = { ip: '127.0.0.1', socket: {} };
    const response = { setHeader: jest.fn() };
    const next = jest.fn();

    middleware.use(request as never, response as never, next);

    expect(() => middleware.use(request as never, response as never, next)).toThrow(HttpException);
    try {
      middleware.use(request as never, response as never, next);
    } catch (error) {
      expect((error as HttpException).getStatus()).toBe(HttpStatus.TOO_MANY_REQUESTS);
    }
    expect(next).toHaveBeenCalledTimes(1);
    expect(response.setHeader).toHaveBeenCalledWith('Retry-After', expect.any(Number));
  });
});
