# SmallCarAuction Frontend

Next.js App Router dashboard with React 19 and TailwindCSS for browsing auctions and placing bids through the session-authenticated backend.

## Run with Docker

From `Projects/SmallCarAuction/backend`:

```bash
docker compose up -d frontend
```

The app is available at http://localhost:5173.

From `Projects/SmallCarAuction/frontend`:

```bash
docker build -t small-car-auction-frontend .
docker run --rm -p 5173:5173 -e NEXT_PUBLIC_API_BASE_URL=http://localhost:3000 small-car-auction-frontend
```

## Behavior

- The App Router page passes an initial auctions promise into the client dashboard, where React 19 `use()` unwraps it during render.
- The dashboard loads protected auctions after login and OTP verification.
- After login, the dashboard subscribes to `GET /auctions/events` with SSE and silently reloads auctions when live events arrive.
- Auction cards use a small state machine:
  - `list`: title, status tag, no-photo placeholder, highest bid.
  - `hover`: zoomed card with enter button, VIN, highest bid, and end timing.
  - `selected`: expanded card with return button and bid form.
- All bid requests use the backend session cookie and `X-CSRF-Token`.
