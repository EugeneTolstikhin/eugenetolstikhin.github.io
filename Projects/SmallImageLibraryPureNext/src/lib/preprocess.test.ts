import { describe, expect, it } from 'vitest';
import { extractRestrictions, normalizeMediaItem, parseGermanDate, tokenize } from './preprocess';

describe('preprocess', () => {
  it('parses valid German dates and rejects invalid dates', () => {
    expect(parseGermanDate('01.11.1995')).toBe('1995-11-01');
    expect(parseGermanDate('31.02.1999')).toBeNull();
  });

  it('extracts restriction tokens', () => {
    expect(extractRestrictions('foo PUBLICATIONxINxGERxSUIxAUTxONLY bar')).toEqual([
      'PUBLICATION IN GER SUI AUT ONLY',
    ]);
  });

  it('normalizes tokens and keeps malformed dates nullable', () => {
    const item = normalizeMediaItem(
      {
        suchtext: 'Bühne Michael PUBLICATIONxINxGERxONLY',
        bildnummer: '001',
        fotografen: 'IMAGO / Test',
        datum: '31.02.1999',
        hoehe: '1',
        breite: '2',
      },
      1,
    );
    expect(item.dateIso).toBeNull();
    expect(tokenize(item.suchtext)).toEqual(expect.arrayContaining(['buhne', 'michael', 'publication']));
  });
});
