import { SearchIndexService } from './search-index.service';
import { normalizeMediaItem } from './preprocess';

describe('SearchIndexService', () => {
  it('weights suchtext over credit and supports prefix matching', () => {
    const service = new SearchIndexService({ findAllMedia: jest.fn() } as never);
    service.rebuild([
      normalizeMediaItem(
        {
          suchtext: 'Serena Williams tennis final Berlin',
          bildnummer: '001',
          fotografen: 'IMAGO / Sportsphoto',
          datum: '01.01.2020',
          hoehe: '100',
          breite: '100',
        },
        1,
      ),
      normalizeMediaItem(
        {
          suchtext: 'Archive portrait Berlin',
          bildnummer: '002',
          fotografen: 'IMAGO / Serena Agency',
          datum: '01.01.2021',
          hoehe: '100',
          breite: '100',
        },
        2,
      ),
    ]);

    const scores = service.score('Sere');
    expect(scores.get(1)).toBeGreaterThan(scores.get(2) ?? 0);
  });
});
