# Small Image Library Pure Next

Minimal pure Next.js implementation of the IMAGO-style media metadata search challenge.

This version intentionally keeps the whole solution inside one Next.js TypeScript app:

- Frontend: `app/page.tsx`
- API: App Router route handlers under `app/api/*`
- Search/data layer: in-memory generated dataset and inverted index under `src/lib`
- Styling: Tailwind
- Runtime: Docker only

## Run Locally

No local Node/npm installation is required. From this folder:

```bash
docker compose up --build
```

Open:

- UI: `http://localhost:3010`
- Search API: `http://localhost:3010/api/search?q=Michael&page=1&pageSize=12`
- Facets API: `http://localhost:3010/api/facets`
- Analytics API: `http://localhost:3010/api/analytics`

## API

`GET /api/search`

Query parameters:

- `q`: keyword search text
- `credit`: exact photographer/credit filter
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

## Search Strategy

The app generates 10,000 media records at runtime from the two provided examples plus realistic fixture variations. It preprocesses each item and builds an in-memory inverted index.

Preprocessing:

- Parses `datum` from `DD.MM.YYYY` to ISO dates.
- Keeps invalid dates as `null`.
- Normalizes casing, accents, punctuation, and noisy delimiters.
- Extracts restrictions like `PUBLICATIONxINxGERxSUIxAUTxONLY`.
- Tokenizes text and removes simple stop words.

Scoring:

- `suchtext`: weight `5`
- `fotografen`: weight `2`
- `bildnummer`: weight `8`
- Supports prefix matching, so `Mich` can match `Michael`.

## Performance And Scaling

For 10,000 records, the runtime in-memory index is simple, transparent, and fast enough for the challenge. For millions of records, I would keep this API contract but move indexing/search to Meilisearch, OpenSearch, or Elasticsearch, with asynchronous ingestion and cache invalidation.

## Continuous Ingestion

For the demo, data is generated on startup. If new items arrived every minute, the app would:

1. Accept or receive the raw item.
2. Run the same preprocessing.
3. Append/update the item in durable storage.
4. Update the search index asynchronously.
5. Invalidate or version cached results.

## Analytics

The demo tracks in-memory:

- Number of searches.
- Recent query response times.
- Most common normalized keywords.

## Tests

Run inside Docker:

```bash
docker compose run --rm app npm test
docker compose run --rm app npm run build
```

Covered behavior:

- Date parsing and invalid dates.
- Restriction extraction.
- Tokenization/accent normalization.
- Search, filters, sorting, pagination.
- Query parameter clamping.
- XSS-safe snippet highlighting.

## Trade-Offs

- This version is more directly aligned with the prompt than the service-oriented variant.
- There is no Postgres/Redis/NestJS; all data and analytics are in memory.
- It is easier to run and review, but less production-like.
- A production version should use durable storage, a dedicated search backend, real analytics, integration/e2e tests, and hosted deployment.
