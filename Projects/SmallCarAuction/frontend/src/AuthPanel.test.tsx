import { render, screen } from '@testing-library/react';
import userEvent from '@testing-library/user-event';
import { describe, expect, it, vi } from 'vitest';
import { AuthPanel } from './AuthPanel';

describe('AuthPanel', () => {
  it('renders a logout action for an active session', async () => {
    const user = userEvent.setup();
    const onLogout = vi.fn().mockResolvedValue(undefined);

    render(
      <AuthPanel
        user={{
          id: 'buyer-1',
          email: 'buyer@example.com',
          role: 'BUYER',
          csrfToken: 'csrf-token',
        }}
        onAuthenticated={vi.fn()}
        onLogout={onLogout}
      />,
    );

    await user.click(screen.getByRole('button', { name: /logout/i }));

    expect(onLogout).toHaveBeenCalledOnce();
  });
});
