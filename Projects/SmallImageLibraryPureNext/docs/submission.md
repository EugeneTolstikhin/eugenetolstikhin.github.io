# Small Image Library Pure Next Submission

## Architecture Overview

Small Image Library Pure Next is implemented as a single Dockerized Next.js TypeScript application. The frontend lives in `app/page.tsx`, while the backend search layer is implemented with App Router route handlers under `app/api/*`.

This version intentionally follows the challenge wording closely: the search layer is inside the Next.js app. There are no separate NestJS, Postgres, or Redis services. Data is generated in memory at runtime and indexed inside the application process.

## Search Strategy

The app uses the two provided media examples and generated fixture data to create 10,000 media items. At runtime, each item is normalized and loaded into an in-memory inverted index.

Relevance is weighted:

- `suchtext`: primary field, weight `5`
- `fotografen`: secondary field, weight `2`
- `bildnummer`: identifier-style field, weight `8`

The index supports token and prefix matching. Results can be sorted by relevance, newest date, or oldest date. Filtering and pagination are applied after scoring.

## Preprocessing

The preprocessing step improves search consistency before indexing:

- German dates are parsed from `DD.MM.YYYY` to ISO dates.
- Invalid dates become `null`, remain keyword-searchable, and are excluded from active date-range filters.
- Text is accent-normalized, lowercased, tokenized, and cleaned of noisy delimiters.
- Restriction strings such as `PUBLICATIONxINxGERxSUIxAUTxONLY` are extracted and normalized to labels such as `PUBLICATION IN GER SUI AUT ONLY`.
- Simple stop words are removed from tokens.

For the demo, preprocessing happens at runtime when the in-memory store is initialized.

## API And UI

The app exposes:

- `GET /api/search`
- `GET /api/facets`
- `GET /api/analytics`

The search response includes `items`, `page`, `pageSize`, `total`, `totalPages`, and `timingMs`.

The Tailwind UI includes debounced search, credit filter, date range controls, restriction chips, sorting, result cards, snippet highlighting, loading state, empty state, error state, and pagination controls.

## Analytics

The demo tracks basic analytics in memory:

- Total number of searches.
- Recent query response timings.
- Most common normalized search keywords.

This is intentionally lightweight. In production, analytics should be persisted to a durable observability or product analytics system.

## Scaling And Continuous Ingestion

For 10,000 records, an in-memory index is simple, transparent, and fast enough for the challenge. For millions of records, I would keep the API contract stable but move search indexing and ranking to Meilisearch, OpenSearch, or Elasticsearch.

If new items arrived every minute, the system would ingest the raw item, run the same preprocessing, append or update the item in durable storage, update the search index asynchronously, and invalidate or version cached results.

## Testing Approach

Automated tests cover:

- Date parsing and invalid-date handling.
- Restriction extraction.
- Accent normalization and tokenization.
- Search, filters, sorting, pagination.
- Query parameter clamping.
- XSS-safe snippet highlighting.

## Trade-Offs

This version is more directly aligned with the challenge prompt than a service-oriented architecture. The trade-off is that it is less production-like: data and analytics are in memory, and there is no durable database or external search service.

For a production IMAGO-scale system, I would add durable storage, asynchronous ingestion, a dedicated search engine, real analytics, CI/CD, observability, integration tests, and a hosted deployment.
