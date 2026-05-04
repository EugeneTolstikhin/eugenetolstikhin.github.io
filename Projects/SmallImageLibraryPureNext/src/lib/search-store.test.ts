import { describe, expect, it } from 'vitest';
import { parseSearchParams, searchMedia } from './search-store';

describe('search-store', () => {
  it('returns paginated keyword results', () => {
    const result = searchMedia({
      q: 'Michael',
      restrictions: [],
      sort: 'relevance',
      page: 1,
      pageSize: 3,
    });

    expect(result.items).toHaveLength(3);
    expect(result.total).toBeGreaterThan(100);
    expect(result.items[0].suchtext.toLowerCase()).toContain('michael');
  });

  it('filters by credit, date, and restriction', () => {
    const result = searchMedia({
      q: 'Michael',
      credit: 'IMAGO / teutopress',
      dateFrom: '1990-01-01',
      dateTo: '2000-01-01',
      restrictions: ['PUBLICATION IN GER ONLY'],
      sort: 'relevance',
      page: 1,
      pageSize: 10,
    });

    expect(result.items.every((item) => item.fotografen === 'IMAGO / teutopress')).toBe(true);
    expect(result.items.every((item) => item.dateIso && item.dateIso >= '1990-01-01' && item.dateIso <= '2000-01-01')).toBe(true);
    expect(result.items.every((item) => item.restrictions.includes('PUBLICATION IN GER ONLY'))).toBe(true);
  });

  it('clamps unsafe query parameters', () => {
    const query = parseSearchParams(new URLSearchParams('q='.concat('x'.repeat(200), '&pageSize=500&sort=bad')));
    expect(query.q).toHaveLength(120);
    expect(query.pageSize).toBe(50);
    expect(query.sort).toBe('relevance');
  });
});
