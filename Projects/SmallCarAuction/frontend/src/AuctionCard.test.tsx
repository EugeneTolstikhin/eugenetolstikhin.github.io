import { render, screen } from '@testing-library/react';
import userEvent from '@testing-library/user-event';
import { describe, expect, it, vi } from 'vitest';
import { AuctionCard } from './AuctionCard';
import type { Auction } from './types';

const auction: Auction = {
  id: 'auction-1',
  state: 'ACTIVE',
  startingPrice: 25000,
  currentPrice: 27500,
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
      amount: 27500,
      createdAt: '2029-12-31T10:30:00.000Z',
    },
  ],
};

describe('AuctionCard', () => {
  it('renders the list card content by default', () => {
    render(<AuctionCard auction={auction} csrfToken="token" onBid={vi.fn()} />);

    expect(screen.getByRole('heading', { name: 'Toyota Supra' })).toBeInTheDocument();
    expect(screen.getByText('ACTIVE')).toBeInTheDocument();
    expect(screen.getByText('No Photo')).toBeInTheDocument();
    expect(screen.getByText('$27,500.00')).toBeInTheDocument();
    expect(screen.getByRole('button', { name: /go to auction/i })).toBeInTheDocument();
    expect(screen.queryByText(/VIN/)).not.toBeInTheDocument();
  });

  it('shows hover details and opens the selected card', async () => {
    const user = userEvent.setup();
    render(<AuctionCard auction={auction} csrfToken="token" onBid={vi.fn()} />);

    await user.hover(screen.getByTestId('auction-card-auction-1'));

    expect(screen.getByText('VIN JT2MA70L0H0123456')).toBeInTheDocument();
    expect(screen.getByText(/bid placed/i)).toBeInTheDocument();
    expect(screen.getByText(/Dec 31, 2029/)).toBeInTheDocument();
    await user.click(screen.getByRole('button', { name: /go to auction/i }));

    expect(screen.getByRole('button', { name: /return to auction list/i })).toBeInTheDocument();
    expect(screen.getByLabelText(/bid amount/i)).toBeInTheDocument();
    expect(screen.getByText(/bid placed/i)).toBeInTheDocument();
  });

  it('submits a bid and shows an accepted message', async () => {
    const user = userEvent.setup();
    const onBid = vi.fn().mockResolvedValue(undefined);
    render(<AuctionCard auction={auction} csrfToken="token" onBid={onBid} />);

    await user.hover(screen.getByTestId('auction-card-auction-1'));
    await user.click(screen.getByRole('button', { name: /go to auction/i }));
    await user.type(screen.getByLabelText(/bid amount/i), '28000');
    await user.click(screen.getByRole('button', { name: /make a bid/i }));

    expect(onBid).toHaveBeenCalledWith('auction-1', 28000);
    expect(await screen.findByText(/your bid was accepted/i)).toBeInTheDocument();
  });

  it('shows a friendly rejected bid message', async () => {
    const user = userEvent.setup();
    const onBid = vi
      .fn()
      .mockRejectedValue(new Error('Bid must be greater than current price'));
    render(<AuctionCard auction={auction} csrfToken="token" onBid={onBid} />);

    await user.hover(screen.getByTestId('auction-card-auction-1'));
    await user.click(screen.getByRole('button', { name: /go to auction/i }));
    await user.type(screen.getByLabelText(/bid amount/i), '20000');
    await user.click(screen.getByRole('button', { name: /make a bid/i }));

    expect(
      await screen.findByText(/higher than the current price/i),
    ).toBeInTheDocument();
  });
});
