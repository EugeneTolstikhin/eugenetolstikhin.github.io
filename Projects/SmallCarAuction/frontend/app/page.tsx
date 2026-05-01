import { Suspense } from 'react';
import { App } from '../src/App';
import type { Auction } from '../src/types';

export default function Page() {
  return (
    <Suspense fallback={<DashboardFallback />}>
      <App initialAuctionsPromise={getInitialAuctions()} />
    </Suspense>
  );
}

export const dynamic = 'force-dynamic';

async function getInitialAuctions(): Promise<Auction[]> {
  const apiBaseUrl =
    process.env.INTERNAL_API_BASE_URL ??
    process.env.NEXT_PUBLIC_API_BASE_URL ??
    'http://localhost:3000';

  try {
    const response = await fetch(`${apiBaseUrl}/auctions`, {
      cache: 'no-store',
    });

    if (!response.ok) {
      return [];
    }

    return (await response.json()) as Auction[];
  } catch {
    return [];
  }
}

function DashboardFallback() {
  return (
    <main className="app-shell">
      <section className="empty-state">
        <h2>Loading dashboard</h2>
        <p>Preparing the auction experience.</p>
      </section>
    </main>
  );
}
