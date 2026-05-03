import { SearchService } from './search.service';
import { normalizeMediaItem } from './preprocess';

const items = [
  normalizeMediaItem(
    {
      suchtext: 'Michael Jackson music concert Berlin PUBLICATIONxINxGERxONLY',
      bildnummer: '001',
      fotografen: 'IMAGO / teutopress',
      datum: '01.11.1995',
      hoehe: '948',
      breite: '1440',
    },
    1,
  ),
  normalizeMediaItem(
    {
      suchtext: 'Serena Williams tennis final Paris EDITORIALxUSExONLY',
      bildnummer: '002',
      fotografen: 'IMAGO / Sportsphoto',
      datum: '03.06.2022',
      hoehe: '1000',
      breite: '1600',
    },
    2,
  ),
  normalizeMediaItem(
    {
      suchtext: 'Broken archive date Michael portrait PUBLICATIONxINxGERxONLY',
      bildnummer: '003',
      fotografen: 'IMAGO / teutopress',
      datum: '31.02.1999',
      hoehe: '1000',
      breite: '1600',
    },
    3,
  ),
];

function createService() {
  const index = {
    score: jest.fn((query: string) => {
      const scores = new Map<number, number>();
      for (const item of items) {
        const hit = item.searchableText.includes(query.toLowerCase());
        if (!query || hit) scores.set(item.id, hit ? 10 : 0);
      }
      return scores;
    }),
    allItems: jest.fn(() => items),
  };
  const analytics = {
    getCached: jest.fn(async () => null),
    setCached: jest.fn(async () => undefined),
    recordSearch: jest.fn(async () => undefined),
    snapshot: jest.fn(async () => ({ searches: 0, timingsMs: [], topKeywords: [] })),
  };
  const database = {
    getFacets: jest.fn(async () => ({ credits: [], restrictions: [] })),
  };
  return new SearchService(index as never, analytics as never, database as never);
}

describe('SearchService', () => {
  it('filters by credit, restriction, and date range', async () => {
    const service = createService();
    const result = await service.search({
      q: 'Michael',
      credit: 'IMAGO / teutopress',
      dateFrom: '1990-01-01',
      dateTo: '2000-01-01',
      restrictions: ['PUBLICATION IN GER ONLY'],
      sort: 'relevance',
      page: 1,
      pageSize: 10,
    });

    expect(result.items).toHaveLength(1);
    expect(result.items[0].bildnummer).toBe('001');
  });

  it('excludes malformed dates when a date range is active', async () => {
    const service = createService();
    const result = await service.search({
      q: 'Michael',
      dateFrom: '1990-01-01',
      dateTo: '2030-01-01',
      restrictions: [],
      sort: 'relevance',
      page: 1,
      pageSize: 10,
    });

    expect(result.items.map((item) => item.bildnummer)).not.toContain('003');
  });

  it('paginates results with a stable response contract', async () => {
    const service = createService();
    const result = await service.search({
      q: '',
      restrictions: [],
      sort: 'date_desc',
      page: 2,
      pageSize: 1,
    });

    expect(result.page).toBe(2);
    expect(result.pageSize).toBe(1);
    expect(result.total).toBe(3);
    expect(result.totalPages).toBe(3);
    expect(result.items).toHaveLength(1);
  });
});
