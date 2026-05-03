import { extractRestrictions, normalizeMediaItem, parseGermanDate, tokenize } from './preprocess';

describe('preprocess', () => {
  it('parses valid German dates and rejects invalid dates', () => {
    expect(parseGermanDate('01.11.1995')).toBe('1995-11-01');
    expect(parseGermanDate('31.02.1999')).toBeNull();
    expect(parseGermanDate('1995-11-01')).toBeNull();
  });

  it('extracts restriction tokens from noisy suchtext', () => {
    expect(extractRestrictions('foo PUBLICATIONxINxGERxSUIxAUTxONLY bar')).toEqual([
      'PUBLICATION IN GER SUI AUT ONLY',
    ]);
  });

  it('normalizes tokens across delimiters and casing', () => {
    expect(tokenize('Michael-Jackson...PUBLICATIONxINxGERxONLY')).toEqual(
      expect.arrayContaining(['michael', 'jackson', 'publication', 'ger', 'only']),
    );
  });

  it('normalizes media and keeps malformed dates as nullable metadata', () => {
    const item = normalizeMediaItem(
      {
        suchtext: 'Archive football PUBLICATIONxINxGERxONLY',
        bildnummer: '001',
        fotografen: 'IMAGO / Test',
        datum: '31.02.1999',
        hoehe: '100',
        breite: '200',
      },
      1,
    );

    expect(item.dateIso).toBeNull();
    expect(item.restrictions).toEqual(['PUBLICATION IN GER ONLY']);
  });
});
