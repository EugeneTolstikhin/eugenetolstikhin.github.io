import { Injectable } from '@nestjs/common';
import { createHash } from 'crypto';
import { AnalyticsService } from './analytics.service';
import { DatabaseService } from './database.service';
import { SearchQuery, SearchResponse, SortMode } from './media.types';
import { makeSnippet, tokenize } from './preprocess';
import { SearchIndexService } from './search-index.service';
import { SearchQueryDto } from './dto/search-query.dto';

@Injectable()
export class SearchService {
  constructor(
    private readonly index: SearchIndexService,
    private readonly analytics: AnalyticsService,
    private readonly database: DatabaseService,
  ) {}

  async search(query: SearchQuery): Promise<SearchResponse> {
    const cacheKey = this.cacheKey(query);
    const cached = await this.analytics.getCached(cacheKey);
    if (cached) return cached;

    const started = performance.now();
    const scores = this.index.score(query.q);
    const queryTokens = tokenize(query.q);
    const scored = this.index
      .allItems()
      .map((item) => ({ item, score: scores.get(item.id) ?? 0 }))
      .filter(({ score }) => queryTokens.length === 0 || score > 0)
      .filter(({ item }) => !query.credit || item.fotografen === query.credit)
      .filter(({ item }) => this.matchesDateRange(item.dateIso, query.dateFrom, query.dateTo))
      .filter(({ item }) =>
        query.restrictions.length === 0
          ? true
          : query.restrictions.every((restriction) => item.restrictions.includes(restriction)),
      );

    scored.sort((left, right) => this.compare(left, right, query.sort));

    const total = scored.length;
    const totalPages = Math.max(1, Math.ceil(total / query.pageSize));
    const safePage = Math.min(query.page, totalPages);
    const offset = (safePage - 1) * query.pageSize;
    const items = scored.slice(offset, offset + query.pageSize).map(({ item, score }) => ({
      id: item.id,
      suchtext: item.suchtext,
      bildnummer: item.bildnummer,
      fotografen: item.fotografen,
      datum: item.datum,
      dateIso: item.dateIso,
      hoehe: item.hoehe,
      breite: item.breite,
      restrictions: item.restrictions,
      score,
      snippet: makeSnippet(item.suchtext, queryTokens),
    }));

    const response: SearchResponse = {
      items,
      page: safePage,
      pageSize: query.pageSize,
      total,
      totalPages,
      timingMs: Number((performance.now() - started).toFixed(2)),
    };

    await Promise.all([
      this.analytics.recordSearch(query.q, response.timingMs),
      this.analytics.setCached(cacheKey, response),
    ]);

    return response;
  }

  facets(): Promise<{ credits: string[]; restrictions: string[] }> {
    return this.database.getFacets();
  }

  analyticsSnapshot() {
    return this.analytics.snapshot();
  }

  parseQuery(params: Record<string, string | string[] | undefined>): SearchQuery {
    return {
      q: this.first(params.q) ?? '',
      credit: this.first(params.credit),
      dateFrom: this.first(params.dateFrom),
      dateTo: this.first(params.dateTo),
      restrictions: this.list(params.restrictions),
      sort: this.sortMode(this.first(params.sort)),
      page: Math.max(1, Number(this.first(params.page) ?? 1) || 1),
      pageSize: Math.min(50, Math.max(1, Number(this.first(params.pageSize) ?? 12) || 12)),
    };
  }

  fromDto(dto: SearchQueryDto): SearchQuery {
    return {
      q: dto.q,
      credit: dto.credit,
      dateFrom: dto.dateFrom,
      dateTo: dto.dateTo,
      restrictions: dto.restrictions ? dto.restrictions.split(',').map((item) => item.trim()).filter(Boolean) : [],
      sort: dto.sort,
      page: dto.page,
      pageSize: dto.pageSize,
    };
  }

  private matchesDateRange(dateIso: string | null, from?: string, to?: string): boolean {
    if (!from && !to) return true;
    if (!dateIso) return false;
    if (from && dateIso < from) return false;
    if (to && dateIso > to) return false;
    return true;
  }

  private compare(
    left: { item: { dateIso: string | null; id: number }; score: number },
    right: { item: { dateIso: string | null; id: number }; score: number },
    sort: SortMode,
  ): number {
    if (sort === 'date_asc') return this.dateValue(left.item.dateIso) - this.dateValue(right.item.dateIso);
    if (sort === 'date_desc') return this.dateValue(right.item.dateIso) - this.dateValue(left.item.dateIso);
    return right.score - left.score || this.dateValue(right.item.dateIso) - this.dateValue(left.item.dateIso) || left.item.id - right.item.id;
  }

  private dateValue(value: string | null): number {
    return value ? Date.parse(`${value}T00:00:00.000Z`) : Number.NEGATIVE_INFINITY;
  }

  private cacheKey(query: SearchQuery): string {
    return `search:${createHash('sha1').update(JSON.stringify(query)).digest('hex')}`;
  }

  private first(value: string | string[] | undefined): string | undefined {
    return Array.isArray(value) ? value[0] : value;
  }

  private list(value: string | string[] | undefined): string[] {
    const first = this.first(value);
    return first ? first.split(',').map((item) => item.trim()).filter(Boolean) : [];
  }

  private sortMode(value?: string): SortMode {
    return value === 'date_asc' || value === 'date_desc' ? value : 'relevance';
  }
}
