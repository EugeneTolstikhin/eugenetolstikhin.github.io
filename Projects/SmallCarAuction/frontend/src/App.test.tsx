import { render, screen } from '@testing-library/react';
import userEvent from '@testing-library/user-event';
import { beforeEach, describe, expect, it, vi } from 'vitest';
import { AuctionDashboard } from './App';
import { listAuctions, login, placeBid, verifyOtp } from './api';
import type { Auction } from './types';

vi.mock('./api', () => ({
  listAuctions: vi.fn(),
  login: vi.fn(),
  logout: vi.fn(),
  placeBid: vi.fn(),
  verifyOtp: vi.fn(),
}));

const auction: Auction = {
  id: 'auction-1',
  state: 'ACTIVE',
  startingPrice: 25000,
  currentPrice: 28000,
  endTime: '2030-01-01T12:00:00.000Z',
  vehicle: {
    id: 'vehicle-1',
    brand: 'Toyota',
    model: 'Supra',
    vin: 'JT2MA70L0H0123456',
  },
  bids: [
    {
      id: 'bid-1',
      amount: 28000,
      createdAt: '2029-12-31T10:30:00.000Z',
    },
  ],
};

describe('App auction flow', () => {
  beforeEach(() => {
    vi.mocked(login).mockResolvedValue({
      mfaRequired: true,
      delivery: 'email',
      email: 'buyer@example.com',
      expiresInSeconds: 300,
    });
    vi.mocked(verifyOtp).mockResolvedValue({
      id: 'buyer-1',
      email: 'buyer@example.com',
      role: 'BUYER',
      csrfToken: 'csrf-token',
    });
    vi.mocked(listAuctions).mockResolvedValue([auction]);
    vi.mocked(placeBid).mockResolvedValue({
      auction: {
        ...auction,
        currentPrice: 29000,
        bids: [
          {
            id: 'bid-2',
            amount: 29000,
            createdAt: '2029-12-31T10:35:00.000Z',
          },
        ],
      },
      bid: {
        id: 'bid-2',
        amount: 29000,
        createdAt: '2029-12-31T10:35:00.000Z',
      },
    });
  });

  it('keeps the selected auction open after a successful bid', async () => {
    const user = userEvent.setup();

    render(<AuctionDashboard initialAuctions={[]} />);

    await user.click(await screen.findByRole('button', { name: /continue/i }));
    await user.type(await screen.findByLabelText(/otp code/i), '123456');
    await user.click(screen.getByRole('button', { name: /verify/i }));

    await user.click(await screen.findByRole('button', { name: /go to auction/i }));
    await user.type(screen.getByLabelText(/bid amount/i), '29000');
    await user.click(screen.getByRole('button', { name: /make a bid/i }));

    expect(await screen.findByText(/your bid was accepted/i)).toBeInTheDocument();
    expect(screen.getByRole('button', { name: /return to auction list/i })).toBeInTheDocument();
  });
});
