# Car Auction Backend

Small NestJS backend for car auctions with Prisma, PostgreSQL, Redis-backed sessions, email OTP MFA, role-based access, and an explicit auction state machine.

When an active auction expires, the backend finalizes it by assigning `winnerBuyerId` and `winningBidId` on the auction. The winning bid is the highest bid created before or at `endTime`; if multiple bids have the same amount, the earliest bid wins.

Auction creation is VIN-idempotent. Retrying the same create request for the same VIN returns the existing auction, but a different request for a VIN already assigned to an auction is rejected. A vehicle that is sold or currently active cannot be assigned to another auction.

## Run Backend Only

```bash
docker compose up -d
```

From `Projects/SmallCarAuction/backend`, this starts only the backend stack: API, database, Redis session store, Mailpit test SMTP server, and Jaeger tracing.

## Run Full Stack

From `Projects/SmallCarAuction`, run:

```bash
docker compose up -d
```

This starts the backend stack plus the Next.js frontend.

- API: http://localhost:3000
- Frontend: http://localhost:5173
- API docs: http://localhost:3000/docs
- OpenAPI JSON: http://localhost:3000/docs-json
- Mailpit: http://localhost:8025
- Jaeger UI: http://localhost:16686

Mailpit receives OTP emails sent through SMTP port 1025. Jaeger receives OpenTelemetry traces through OTLP HTTP at `http://jaeger:4318/v1/traces`.

## Observability

Tracing is enabled in Docker with:

```yaml
TRACING_ENABLED: "true"
OTEL_SERVICE_NAME: small-car-auction-api
OTEL_EXPORTER_OTLP_TRACES_ENDPOINT: http://jaeger:4318/v1/traces
```

Open Jaeger at http://localhost:16686 and select `small-car-auction-api` to inspect request traces. Set `TRACING_ENABLED=false` to disable tracing.

## API documentation

Swagger UI is served at http://localhost:3000/docs and the raw OpenAPI document is served at http://localhost:3000/docs-json. Set `API_DOCS_ENABLED=false` to disable API documentation in stricter environments.

## Security controls

- Cookie-backed sessions are stored in Redis and are the only token used by protected REST APIs.
- Password login is followed by email OTP MFA. Session IDs are regenerated before pending MFA and again after MFA succeeds.
- Unsafe authenticated requests require the `X-CSRF-Token` returned by `POST /auth/verify-otp`.
- CORS is credentialed and allowlisted through `CORS_ORIGINS`.
- Helmet sets defensive HTTP headers including CSP, frame denial, no-sniff, and no-referrer.
- JSON responses are escaped, DTO validation strips unknown fields, and request bodies are limited with `REQUEST_BODY_LIMIT`.
- Redis-backed rate limiting protects all endpoints, with stricter limits for login and OTP verification.
- Production cookies are `HttpOnly`, `SameSite=Strict`, `Secure`, and can use the `__Host-` prefix when `NODE_ENV=production`.

Production deployments should set a strong `SESSION_SECRET`, explicit `CORS_ORIGINS`, `NODE_ENV=production`, `API_DOCS_ENABLED=false` unless docs are intentionally exposed, and `TRUST_PROXY=true` when TLS terminates at a trusted reverse proxy.

## Seed users

```json
{ "email": "admin@example.com", "password": "AdminPass123!", "role": "ADMIN" }
{ "email": "buyer@example.com", "password": "BuyerPass123!", "role": "BUYER" }
{ "email": "buyer2@example.com", "password": "BuyerTwoPass123!", "role": "BUYER" }
```

The seed passwords can be overridden with `SEED_ADMIN_PASSWORD`, `SEED_BUYER_PASSWORD`, and `SEED_BUYER_TWO_PASSWORD` in the API container environment.

## APIs

### Login

Checks email/password, creates an OTP challenge, sends the one-time code through Mailpit, and stores the pending MFA challenge in the server-side session.

```http
POST /auth/login
Content-Type: application/json

{ "email": "admin@example.com", "password": "AdminPass123!" }
```

### Verify OTP

Completes MFA and stores the authenticated user in the server-side session. Use the session cookie from login and verify-OTP for every protected REST API call. There are no bearer access tokens. For unsafe authenticated requests, send the returned `csrfToken` as `X-CSRF-Token`.

```http
POST /auth/verify-otp
Content-Type: application/json

{ "code": "123456" }
```

### Create auction

Admin session only. Creates a `DRAFT` auction.

```http
POST /auctions
Content-Type: application/json

{
  "brand": "Toyota",
  "model": "Supra",
  "vin": "JT2MA70L0H0123456",
  "startingPrice": 25000,
  "endTime": "2026-12-31T18:00:00.000Z"
}
```

### Activate auction

Admin only. Uses the state machine transition `DRAFT -> ACTIVE`.

```http
POST /auctions/{auctionId}/activate
```

### List auctions

Authenticated sessions.

```http
GET /auctions
```

Returns all auctions with their current status and the winning bid candidate, ordered by highest amount and then earliest bid time.

### Get auction status

Buyer session only.

```http
GET /auctions/{auctionId}/status
```

### Get current highest bid

Buyer session only. Equal highest bids are resolved by earliest `createdAt`.

```http
GET /auctions/{auctionId}/highest-bid
```

### Subscribe to live auction events

Authenticated admin or buyer session. The browser receives Server-Sent Events for dashboard refreshes:

- `auction.created`
- `auction.activated`
- `auction.highestBidChanged`
- `auction.expired`

```http
GET /auctions/events
Accept: text/event-stream
```

### Place bid

Buyer session only. Auction must be `ACTIVE` and not expired.

```http
POST /auctions/{auctionId}/bids
Content-Type: application/json

{ "amount": 27500 }
```
