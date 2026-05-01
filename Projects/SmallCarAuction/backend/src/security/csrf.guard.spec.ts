import { ExecutionContext, ForbiddenException } from '@nestjs/common';
import { CsrfGuard } from './csrf.guard';

describe('CsrfGuard', () => {
  const guard = new CsrfGuard();

  function contextFor(request: {
    method: string;
    session: { csrfToken?: string };
    header: jest.Mock;
  }) {
    return {
      switchToHttp: () => ({
        getRequest: () => request,
      }),
    } as unknown as ExecutionContext;
  }

  it('allows safe read methods without a token', () => {
    const request = {
      method: 'GET',
      session: {},
      header: jest.fn(),
    };

    expect(guard.canActivate(contextFor(request))).toBe(true);
  });

  it('rejects unsafe methods without a matching token', () => {
    const request = {
      method: 'POST',
      session: { csrfToken: 'session-token' },
      header: jest.fn().mockReturnValue('wrong-token'),
    };

    expect(() => guard.canActivate(contextFor(request))).toThrow(
      ForbiddenException,
    );
  });

  it('allows unsafe methods with a matching token', () => {
    const request = {
      method: 'POST',
      session: { csrfToken: 'session-token' },
      header: jest.fn().mockReturnValue('session-token'),
    };

    expect(guard.canActivate(contextFor(request))).toBe(true);
  });
});
