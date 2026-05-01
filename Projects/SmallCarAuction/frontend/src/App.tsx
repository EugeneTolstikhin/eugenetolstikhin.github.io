'use client';

import { RefreshCcw } from 'lucide-react';
import { use, useState } from 'react';
import { listAuctions, logout, placeBid } from './api';
import { AuctionCard } from './AuctionCard';
import { AuthPanel } from './AuthPanel';
import type { Auction, SessionUser } from './types';

type Props = {
  initialAuctionsPromise: Promise<Auction[]>;
};

export function App({ initialAuctionsPromise }: Props) {
  const initialAuctions = use(initialAuctionsPromise);
  return <AuctionDashboard initialAuctions={initialAuctions} />;
}

export function AuctionDashboard({ initialAuctions }: { initialAuctions: Auction[] }) {
  const [user, setUser] = useState<SessionUser>();
  const [auctions, setAuctions] = useState<Auction[]>(initialAuctions);
  const [error, setError] = useState('');
  const [isLoading, setIsLoading] = useState(false);

  async function loadAuctions(
    sessionUser = user,
    options: { showLoading?: boolean } = {},
  ) {
    const showLoading = options.showLoading ?? true;

    if (!sessionUser?.csrfToken) {
      setError('Sign in and verify OTP before refreshing auctions.');
      return;
    }

    if (showLoading) {
      setIsLoading(true);
    }
    setError('');

    try {
      setAuctions(await listAuctions());
    } catch (loadError) {
      setError(
        loadError instanceof Error
          ? loadError.message
          : 'Could not load auctions. Sign in and try again.',
      );
    } finally {
      if (showLoading) {
        setIsLoading(false);
      }
    }
  }

  async function handleAuthenticated(nextUser: SessionUser) {
    setUser(nextUser);
    await loadAuctions(nextUser);
  }

  async function handleLogout() {
    if (user?.csrfToken) {
      await logout(user.csrfToken);
    }

    setUser(undefined);
    setAuctions([]);
    setError('');
  }

  async function submitBid(auctionId: string, amount: number) {
    if (!user?.csrfToken) {
      throw new Error('Sign in before placing a bid.');
    }

    const bidResult = await placeBid(auctionId, amount, user.csrfToken);
    await loadAuctions(user, { showLoading: false });
    return { isWinningBid: bidResult.isWinningBid ?? false };
  }

  return (
    <main className="app-shell">
      <header className="page-header">
        <div>
          <p className="eyebrow">SmallCarAuction</p>
          <h1>Auction dashboard</h1>
        </div>
        <AuthPanel user={user} onAuthenticated={handleAuthenticated} onLogout={handleLogout} />
      </header>

      <section className="toolbar" aria-label="Auction controls">
        <div>
          <strong>{auctions.length}</strong>
          <span>{auctions.length === 1 ? 'auction' : 'auctions'}</span>
        </div>
        <button
          className="icon-text-button"
          type="button"
          onClick={() => void loadAuctions()}
          disabled={isLoading}
        >
          <RefreshCcw aria-hidden="true" />
          Refresh
        </button>
      </section>

      {error && <p className="notice notice--error">{error}</p>}

      {!user ? (
        <section className="empty-state">
          <h2>Sign in to view auctions</h2>
          <p>The dashboard uses the backend session cookie and CSRF token for protected auction actions.</p>
        </section>
      ) : isLoading ? (
        <section className="empty-state">
          <h2>Loading auctions</h2>
          <p>Fetching the latest list from the backend.</p>
        </section>
      ) : auctions.length === 0 ? (
        <section className="empty-state">
          <h2>No auctions yet</h2>
          <p>Create and activate auctions from the backend API, then refresh this dashboard.</p>
        </section>
      ) : (
        <section className="auction-grid" aria-label="Auction list">
          {auctions.map((auction) => (
            <AuctionCard
              key={auction.id}
              auction={auction}
              csrfToken={user.csrfToken}
              currentUserId={user.id}
              onBid={submitBid}
            />
          ))}
        </section>
      )}
    </main>
  );
}
