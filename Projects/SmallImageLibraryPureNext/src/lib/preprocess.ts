import { MediaItem, RawMediaItem } from './types';

const STOP_WORDS = new Set(['and', 'or', 'the', 'der', 'die', 'das', 'ein', 'eine', 'in', 'on', 'at', 'for']);

export function parseGermanDate(value: string): string | null {
  const match = /^(\d{2})\.(\d{2})\.(\d{4})$/.exec(value.trim());
  if (!match) return null;

  const [, dayText, monthText, yearText] = match;
  const day = Number(dayText);
  const month = Number(monthText);
  const year = Number(yearText);
  const date = new Date(Date.UTC(year, month - 1, day));

  if (
    date.getUTCFullYear() !== year ||
    date.getUTCMonth() !== month - 1 ||
    date.getUTCDate() !== day
  ) {
    return null;
  }

  return `${yearText}-${monthText}-${dayText}`;
}

export function normalizeText(value: string): string {
  return value
    .normalize('NFKD')
    .replace(/[\u0300-\u036f]/g, '')
    .replace(/([A-Z])x([A-Z])/g, '$1 $2')
    .replace(/[^a-zA-Z0-9]+/g, ' ')
    .toLowerCase()
    .trim();
}

export function tokenize(value: string): string[] {
  const normalized = normalizeText(value);
  if (!normalized) return [];
  return Array.from(
    new Set(
      normalized
        .split(/\s+/)
        .filter((token) => token.length > 1 && !STOP_WORDS.has(token)),
    ),
  );
}

export function extractRestrictions(suchtext: string): string[] {
  const rawMatches = suchtext.match(/\b[A-Z]+x(?:[A-Z]+x?)+\b/g) ?? [];
  return Array.from(
    new Set(
      rawMatches.map((match) =>
        match
          .replace(/x/g, ' ')
          .replace(/\s+/g, ' ')
          .trim()
          .toUpperCase(),
      ),
    ),
  );
}

export function normalizeMediaItem(raw: RawMediaItem, id: number): MediaItem {
  const dateIso = parseGermanDate(raw.datum);
  const restrictions = extractRestrictions(raw.suchtext);
  const weightedText = `${raw.suchtext} ${raw.suchtext} ${raw.fotografen} ${raw.bildnummer}`;

  return {
    ...raw,
    id,
    dateIso,
    restrictions,
    searchableText: normalizeText(`${raw.suchtext} ${raw.fotografen} ${raw.bildnummer}`),
    tokens: tokenize(weightedText),
  };
}

export function makeSnippet(text: string, queryTokens: string[]): string {
  if (queryTokens.length === 0) return text.slice(0, 180);
  const lower = text.toLowerCase();
  const hit = queryTokens
    .map((token) => lower.indexOf(token.toLowerCase()))
    .filter((index) => index >= 0)
    .sort((a, b) => a - b)[0];

  if (hit === undefined) return text.slice(0, 180);
  const start = Math.max(0, hit - 70);
  const end = Math.min(text.length, hit + 130);
  return `${start > 0 ? '...' : ''}${text.slice(start, end)}${end < text.length ? '...' : ''}`;
}
