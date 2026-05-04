import { generateMediaItems } from './fixture-data';
import { makeSnippet, normalizeMediaItem, tokenize } from './preprocess';
import { AnalyticsSnapshot, FacetsResponse, MediaItem, SearchQuery, SearchResponse, SortMode } from './types';

interface ScoredItem {
  item: MediaItem;
  score: number;
}

const analytics = {
  searches: 0,
  timingsMs: [] as number[],
  keywords: new Map<string, number>(),
};

let store: {
  items: Map<number, MediaItem>;
  index: Map<string, Map<number, number>>;
  facets: FacetsResponse;
} | null = null;

export function getStore() {
  if (store) return store;

  const total = Number(process.env.SEED_SIZE ?? 10000);
  const items = new Map<number, MediaItem>();
  const index = new Map<string, Map<number, number>>();
  const credits = new Set<string>();
  const restrictions = new Set<string>();

  for (const [zeroBasedIndex, raw] of generateMediaItems(total).entries()) {
    const item = normalizeMediaItem(raw, zeroBasedIndex + 1);
    items.set(item.id, item);
    credits.add(item.fotografen);
    for (const restriction of item.restrictions) restrictions.add(restriction);
    addWeightedTokens(index, item.id, tokenize(item.suchtext), 5);
    addWeightedTokens(index, item.id, tokenize(item.fotografen), 2);
    addWeightedTokens(index, item.id, tokenize(item.bildnummer), 8);
  }

  store = {
    items,
    index,
    facets: {
      credits: Array.from(credits).sort(),
      restrictions: Array.from(restrictions).sort(),
    },
  };
  return store;
}

export function searchMedia(query: SearchQuery): SearchResponse {
  const started = performance.now();
  const queryTokens = tokenize(query.q);
  const scores = score(query.q);
  const scored = Array.from(getStore().items.values())
    .map((item) => ({ item, score: scores.get(item.id) ?? 0 }))
    .filter(({ score }) => queryTokens.length === 0 || score > 0)
    .filter(({ item }) => !query.credit || item.fotografen === query.credit)
    .filter(({ item }) => matchesDateRange(item.dateIso, query.dateFrom, query.dateTo))
    .filter(({ item }) =>
      query.restrictions.length === 0
        ? true
        : query.restrictions.every((restriction) => item.restrictions.includes(restriction)),
    );

  scored.sort((left, right) => compare(left, right, query.sort));

  const total = scored.length;
  const totalPages = Math.max(1, Math.ceil(total / query.pageSize));
  const safePage = Math.min(query.page, totalPages);
  const offset = (safePage - 1) * query.pageSize;
  const items = scored.slice(offset, offset + query.pageSize).map(({ item, score: itemScore }) => ({
    id: item.id,
    suchtext: item.suchtext,
    bildnummer: item.bildnummer,
    fotografen: item.fotografen,
    datum: item.datum,
    dateIso: item.dateIso,
    hoehe: item.hoehe,
    breite: item.breite,
    restrictions: item.restrictions,
    score: itemScore,
    snippet: makeSnippet(item.suchtext, queryTokens),
  }));

  const response = {
    items,
    page: safePage,
    pageSize: query.pageSize,
    total,
    totalPages,
    timingMs: Number((performance.now() - started).toFixed(2)),
  };

  recordAnalytics(query.q, response.timingMs);
  return response;
}

export function getFacets(): FacetsResponse {
  return getStore().facets;
}

export function getAnalytics(): AnalyticsSnapshot {
  return {
    searches: analytics.searches,
    timingsMs: analytics.timingsMs.slice(0, 50),
    topKeywords: Array.from(analytics.keywords.entries())
      .sort((left, right) => right[1] - left[1])
      .slice(0, 10)
      .map(([keyword, count]) => ({ keyword, count })),
  };
}

export function parseSearchParams(params: URLSearchParams): SearchQuery {
  return {
    q: limit(params.get('q') ?? '', 120),
    credit: optional(limit(params.get('credit') ?? '', 120)),
    dateFrom: optional(params.get('dateFrom') ?? ''),
    dateTo: optional(params.get('dateTo') ?? ''),
    restrictions: limit(params.get('restrictions') ?? '', 400)
      .split(',')
      .map((item) => item.trim())
      .filter(Boolean),
    sort: parseSort(params.get('sort')),
    page: clampNumber(params.get('page'), 1, 9999, 1),
    pageSize: clampNumber(params.get('pageSize'), 1, 50, 12),
  };
}

function addWeightedTokens(index: Map<string, Map<number, number>>, id: number, tokens: string[], weight: number): void {
  for (const token of tokens) {
    const postings = index.get(token) ?? new Map<number, number>();
    postings.set(id, (postings.get(id) ?? 0) + weight);
    index.set(token, postings);
  }
}

function score(query: string): Map<number, number> {
  const queryTokens = tokenize(query);
  const scores = new Map<number, number>();
  if (queryTokens.length === 0) {
    for (const item of getStore().items.values()) scores.set(item.id, 0);
    return scores;
  }

  for (const token of queryTokens) {
    for (const [indexedToken, postings] of getStore().index.entries()) {
      if (indexedToken === token || indexedToken.startsWith(token)) {
        for (const [id, itemScore] of postings.entries()) {
          scores.set(id, (scores.get(id) ?? 0) + itemScore);
        }
      }
    }
  }

  return scores;
}

function matchesDateRange(dateIso: string | null, from?: string, to?: string): boolean {
  if (!from && !to) return true;
  if (!dateIso) return false;
  if (from && dateIso < from) return false;
  if (to && dateIso > to) return false;
  return true;
}

function compare(left: ScoredItem, right: ScoredItem, sort: SortMode): number {
  if (sort === 'date_asc') return dateValue(left.item.dateIso) - dateValue(right.item.dateIso);
  if (sort === 'date_desc') return dateValue(right.item.dateIso) - dateValue(left.item.dateIso);
  return right.score - left.score || dateValue(right.item.dateIso) - dateValue(left.item.dateIso) || left.item.id - right.item.id;
}

function dateValue(value: string | null): number {
  return value ? Date.parse(`${value}T00:00:00.000Z`) : Number.NEGATIVE_INFINITY;
}

function parseSort(value: string | null): SortMode {
  return value === 'date_asc' || value === 'date_desc' ? value : 'relevance';
}

function clampNumber(value: string | null, min: number, max: number, fallback: number): number {
  const parsed = Number(value ?? fallback);
  if (!Number.isFinite(parsed)) return fallback;
  return Math.min(max, Math.max(min, Math.floor(parsed)));
}

function limit(value: string, maxLength: number): string {
  return value.trim().slice(0, maxLength);
}

function optional(value: string): string | undefined {
  const trimmed = value.trim();
  return trimmed || undefined;
}

function recordAnalytics(query: string, timingMs: number): void {
  analytics.searches += 1;
  analytics.timingsMs.unshift(Math.round(timingMs));
  analytics.timingsMs = analytics.timingsMs.slice(0, 200);
  for (const token of tokenize(query)) {
    analytics.keywords.set(token, (analytics.keywords.get(token) ?? 0) + 1);
  }
}
