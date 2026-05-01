import { createRateLimitMiddleware } from './rate-limit.middleware';

describe('createRateLimitMiddleware', () => {
  function buildResponse() {
    return {
      setHeader: jest.fn(),
      status: jest.fn().mockReturnThis(),
      json: jest.fn(),
    };
  }

  it('allows requests below the configured limit', async () => {
    const redis = {
      incr: jest.fn().mockResolvedValue(1),
      expire: jest.fn().mockResolvedValue(1),
    };
    const request = {
      ip: '127.0.0.1',
      method: 'GET',
      path: '/auctions',
    };
    const response = buildResponse();
    const next = jest.fn();

    await createRateLimitMiddleware({
      redis: redis as never,
      windowSeconds: 60,
      maxRequests: 3,
      authWindowSeconds: 300,
      authMaxRequests: 1,
    })(request as never, response as never, next);

    expect(next).toHaveBeenCalledTimes(1);
    expect(response.status).not.toHaveBeenCalled();
    expect(redis.expire).toHaveBeenCalledWith(expect.any(String), 60);
  });

  it('blocks requests over the configured limit', async () => {
    const redis = {
      incr: jest.fn().mockResolvedValue(4),
      expire: jest.fn(),
    };
    const request = {
      ip: '127.0.0.1',
      method: 'GET',
      path: '/auctions',
    };
    const response = buildResponse();
    const next = jest.fn();

    await createRateLimitMiddleware({
      redis: redis as never,
      windowSeconds: 60,
      maxRequests: 3,
      authWindowSeconds: 300,
      authMaxRequests: 1,
    })(request as never, response as never, next);

    expect(next).not.toHaveBeenCalled();
    expect(response.status).toHaveBeenCalledWith(429);
    expect(response.json).toHaveBeenCalledWith({ message: 'Too many requests' });
  });

  it('uses the stricter auth limit for login and OTP endpoints', async () => {
    const redis = {
      incr: jest.fn().mockResolvedValue(2),
      expire: jest.fn(),
    };
    const request = {
      ip: '127.0.0.1',
      method: 'POST',
      path: '/auth/login',
    };
    const response = buildResponse();
    const next = jest.fn();

    await createRateLimitMiddleware({
      redis: redis as never,
      windowSeconds: 60,
      maxRequests: 100,
      authWindowSeconds: 300,
      authMaxRequests: 1,
    })(request as never, response as never, next);

    expect(response.status).toHaveBeenCalledWith(429);
    expect(next).not.toHaveBeenCalled();
  });

  it('fails closed when Redis is unavailable', async () => {
    const redis = {
      incr: jest.fn().mockRejectedValue(new Error('offline')),
      expire: jest.fn(),
    };
    const request = {
      ip: '127.0.0.1',
      method: 'GET',
      path: '/auctions',
    };
    const response = buildResponse();
    const next = jest.fn();

    await createRateLimitMiddleware({
      redis: redis as never,
      windowSeconds: 60,
      maxRequests: 3,
      authWindowSeconds: 300,
      authMaxRequests: 1,
    })(request as never, response as never, next);

    expect(response.status).toHaveBeenCalledWith(503);
    expect(response.json).toHaveBeenCalledWith({
      message: 'Rate limiter unavailable',
    });
    expect(next).not.toHaveBeenCalled();
  });
});
