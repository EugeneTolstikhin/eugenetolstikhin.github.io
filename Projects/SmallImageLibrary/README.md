# Small Image Library

Dockerized media metadata search demo for the IMAGO-style coding challenge.

## Submission Links

- GitHub project link: https://github.com/EugeneTolstikhin/eugenetolstikhin.github.io/tree/main/Projects/SmallImageLibrary
- Runnable solution: Docker Compose locally via the instructions below.
- Live hosted deployment: not provided for this submission because the solution intentionally depends on a multi-service Docker stack with Postgres, Redis, and a NestJS API. The app is packaged to run reproducibly with one Docker command.

## Assumptions

- A service-oriented architecture is acceptable even though the prompt suggests a lightweight search layer inside Next.js. The decision is documented as a role-aligned trade-off for stronger API, database, Docker, and backend design signal.
- Generated fixture data may extend the two provided examples as long as the original examples are included.
- Redis-backed analytics and cache are sufficient for the demo; production analytics would use durable metrics/observability tooling.
- Invalid metadata should remain searchable where possible instead of being discarded.

## Architecture

This project intentionally uses a service-oriented shape:

- `frontend`: Next.js, TypeScript, Tailwind UI on `http://localhost:3002`
- `api`: NestJS REST API on `http://localhost:3001`
- `postgres`: durable media metadata store managed through Prisma migrations
- `redis`: search analytics and short-lived response cache

The original challenge asks for a lightweight search layer in a Next.js app. I expanded that into a separate NestJS API because the target role emphasizes full-stack ownership, REST APIs, microservice architecture, database design, Docker, and automated testing. The API boundary is still small enough for the challenge but maps cleanly to a future production search service.

## Run Locally

All commands should be run through Docker.

```bash
docker compose up --build
```

Then open:

- Frontend: `http://localhost:3002`
- Search API: `http://localhost:3001/api/search?q=Michael&page=1&pageSize=12`
- Facets API: `http://localhost:3001/api/facets`
- Analytics API: `http://localhost:3001/api/analytics`

The API container runs Prisma migrations, generates the Prisma client, and seeds data automatically on startup. The startup command can baseline the first migration when an existing demo database already has the expected table. Seeding is idempotent because `bildnummer` is unique and Prisma `createMany` uses duplicate skipping.

## API Contract

`GET /api/search`

Query parameters:

- `q`: keyword search text
- `credit`: exact `fotografen` filter
- `dateFrom`: ISO date lower bound, for example `1990-01-01`
- `dateTo`: ISO date upper bound
- `restrictions`: comma-separated restriction labels
- `sort`: `relevance`, `date_asc`, or `date_desc`
- `page`: 1-based page number
- `pageSize`: 1 to 50

Response:

```json
{
  "items": [],
  "page": 1,
  "pageSize": 12,
  "total": 0,
  "totalPages": 1,
  "timingMs": 2.4
}
```

## Search And Relevance

The NestJS API loads normalized media rows from Postgres and builds an in-memory inverted index at startup.

Preprocessing:

- Parse `datum` from `DD.MM.YYYY` to ISO date; invalid dates become `null`.
- Normalize text with accent stripping, lowercasing, delimiter cleanup, and stop-word removal.
- Extract restrictions from `suchtext`, for example `PUBLICATIONxINxGERxSUIxAUTxONLY` becomes `PUBLICATION IN GER SUI AUT ONLY`.
- Store normalized tokens and restrictions in Postgres through Prisma, then rebuild the application index from those fields.

Scoring:

- `suchtext` tokens weight `5`
- `fotografen` tokens weight `2`
- `bildnummer` tokens weight `8`
- Prefix matches are supported, so `Mich` can match `Michael`
- Date sorting can override relevance sorting

This approach is transparent and testable for 10,000 records. For millions of records, I would move the inverted index and ranking to OpenSearch or Elasticsearch, keep Postgres as the system of record, and update the search index asynchronously from an ingestion event stream.

## Redis Analytics And Cache

Redis stores:

- Total search count
- Recent response timing samples
- Most common normalized search keywords
- Cached search responses with a short TTL

The cache is intentionally simple. For continuous ingestion, cache invalidation would move to index-versioned keys or event-driven invalidation after media updates.

## Security Hardening

The API uses several small production-style guardrails:

- NestJS `ValidationPipe` with DTO constraints for query params.
- Maximum query, credit, restriction, page, and page-size limits.
- Helmet security headers on API responses.
- Strict CORS for the frontend origin and `GET` methods only.
- In-memory request rate limiting for the demo.
- Structured JSON error responses.
- Prisma-managed writes and migrations instead of hand-written schema creation in service code.

The frontend adds Content Security Policy, frame protection, MIME sniffing protection, and referrer policy headers through Next.js. React renders snippets as escaped text nodes and does not use `dangerouslySetInnerHTML`, so HTML-like metadata stays inert.

## Continuous Ingestion

For the demo, data is seeded at startup. If new items arrived every minute:

1. Insert raw media into Postgres.
2. Normalize metadata in the API or a background worker.
3. Append/update the search index without blocking user queries.
4. Invalidate or version Redis cache entries.
5. In production, publish the normalized item to OpenSearch and use Postgres only for durable record ownership.

## Testing

Run tests inside containers:

```bash
docker compose run --rm api npm test
docker compose run --rm frontend npm test
```

Current coverage focuses on:

- Tokenization and normalization
- Restriction extraction
- Invalid date handling
- Weighted scoring and prefix matching
- Filtering, sorting, pagination, and response shape
- Frontend snippet highlighting

Testing TODOs retained from the risk review:

- Add backend integration tests for `GET /api/search` against live Postgres and Redis containers.
- Add frontend interaction tests for debounce, filter changes, loading/empty/error states, and pagination.
- Expand fixture regression tests to prove generated data exercises credits, restrictions, partial matches, and relevance ordering.

## Trade-Offs

- NestJS + Prisma + Postgres + Redis is heavier than a single Next.js app, but better demonstrates API/service design for the stated role.
- The in-memory index rebuilds on API startup, which is acceptable for 10,000 rows but not a production-scale search architecture.
- Redis analytics are operationally useful but intentionally lightweight.
- Invalid dates are not discarded; they remain searchable but are excluded from active date-range filters.

## Limitations And What I Would Do Next

- Add a hosted deployment on Render, Fly.io, Railway, or a similar container platform with managed Postgres/Redis.
- Replace the in-memory index with OpenSearch, Elasticsearch, or Meilisearch for millions of records and multi-instance API deployments.
- Add full integration tests against real Postgres/Redis and browser-level tests for the main search workflow.
- Add an ingestion endpoint or worker for continuous media updates instead of startup-only seeding.
- Add production observability with structured logs, tracing, dashboards, and alerting.

## PDF Submission

The PDF content lives in `docs/submission.md`, and the generated PDF is included at `docs/submission.pdf`.
