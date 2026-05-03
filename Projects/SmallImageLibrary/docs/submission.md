# Small Image Library Submission

GitHub project link: https://github.com/EugeneTolstikhin/eugenetolstikhin.github.io/tree/main/Projects/SmallImageLibrary

Runnable solution: Docker Compose locally. A live hosted deployment is not included because the solution intentionally depends on Postgres, Redis, and a NestJS API; it is packaged to run reproducibly as a Docker stack.

## Architecture Overview

Small Image Library is implemented as a Docker Compose application with four services: Next.js frontend, NestJS API, Postgres, and Redis. Prisma owns schema migrations, typed database access, and seed orchestration. The frontend owns the search experience, while the NestJS service owns API contracts, preprocessing, indexing, analytics, and persistence integration.

This is a deliberate service-oriented interpretation of the challenge. It still exposes a simple `GET /api/search` search layer, but it demonstrates how the solution would be maintained as a backend service rather than burying all behavior in the UI runtime.

## Search Strategy

Media rows are stored in Postgres through Prisma, then loaded into a NestJS in-memory inverted index at API startup. Each record is normalized into searchable text, token arrays, restrictions, and a nullable ISO date.

Relevance is weighted:

- `suchtext`: primary field, weight `5`
- `fotografen`: secondary field, weight `2`
- `bildnummer`: exact/prefix lookup field, weight `8`

The index supports token and prefix matching. Results can be sorted by relevance, newest date, or oldest date. Pagination is applied after filtering and sorting.

## Preprocessing

The preprocessing step improves consistency before search:

- German date strings are parsed from `DD.MM.YYYY`; invalid dates become `null`.
- Text is lowercased, accent-normalized, and tokenized.
- Restriction strings such as `PUBLICATIONxINxGERxSUIxAUTxONLY` are extracted and normalized to human-readable labels.
- Noisy delimiters such as punctuation and slashes are normalized into token boundaries.

Preprocessing currently happens during seed and startup indexing. For continuous ingestion, it would run when new media records arrive, either in the API service or a background worker.

## Data And Analytics

Postgres stores the durable media catalog and normalized metadata. Prisma migrations own schema changes and the seed script owns deterministic fixture loading. Redis stores usage analytics and cached search responses. The demo tracks total searches, recent response timings, and the most common normalized keywords.

The Redis response cache uses a short TTL to keep invalidation simple. For a production ingestion pipeline, cache keys would include an index version or be invalidated by media-update events.

## Security

The API validates query parameters with DTO constraints, limits query/page sizes, applies Helmet headers, restricts CORS, returns structured errors, and rate-limits requests. Prisma provides typed database access and keeps schema creation out of service code. The frontend uses CSP and renders metadata through React text nodes rather than raw HTML, which keeps HTML-like metadata from becoming executable script.

## Scaling To Millions

For 10,000 records, an in-memory index keeps the implementation transparent and fast. For millions of records, I would keep Postgres as the source of truth and move search to OpenSearch or Elasticsearch.

The scale architecture would be:

1. Ingest raw media into Postgres.
2. Normalize metadata in a worker.
3. Publish indexing events.
4. Store searchable documents in OpenSearch.
5. Keep Redis for analytics, hot query caching, and rate-limiting support.

This separates durable storage, search ranking, and operational metrics while keeping the API contract stable for the frontend.

## Testing Approach

The automated test strategy covers the highest-risk behavior:

- Tokenization and normalization
- Restriction extraction
- Invalid date handling
- Weighted relevance and prefix matching
- Filtering by credit, date, and restrictions
- Sorting and pagination response shape
- Frontend snippet highlighting
- Malicious query strings, oversized parameters, and HTML-like metadata rendering

Further work would add full integration tests against live Postgres and Redis, plus browser-level tests for debounce, loading, empty, error, and pagination states.

## Trade-Offs

The architecture is heavier than a single Next.js challenge app, but it better demonstrates service boundaries, persistence, containerization, and backend reasoning. The main limitation is that the search index rebuilds on API startup. That is acceptable for a challenge dataset and explicitly points toward OpenSearch for production scale.

Further work would add a hosted container deployment, real ingestion worker, full integration/e2e tests, production observability, and a dedicated search engine such as OpenSearch, Elasticsearch, or Meilisearch.
