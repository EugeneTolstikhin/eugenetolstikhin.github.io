import { RawMediaItem } from './types';

export const challengeItems: RawMediaItem[] = [
  {
    suchtext:
      'J.Morris, Manchester Utd inside right 7th January 1948 UnitedArchives00421716 PUBLICATIONxINxGERxSUIxAUTxONLY',
    bildnummer: '0059987730',
    fotografen: 'IMAGO / United Archives International',
    datum: '01.01.1900',
    hoehe: '2460',
    breite: '3643',
  },
  {
    suchtext:
      'Michael Jackson 11 95 her Mann Musik Gesang Pop USA Hemd leger Studio hoch ganz stehend Bühne PUBLICATIONxINxGERxONLY',
    bildnummer: '0056821849',
    fotografen: 'IMAGO / teutopress',
    datum: '01.11.1995',
    hoehe: '948',
    breite: '1440',
  },
];

const people = ['Serena Williams', 'Michael Jackson', 'Ada Lovelace', 'Lionel Messi', 'Angela Merkel', 'Naomi Osaka'];
const places = ['Berlin', 'Manchester', 'New York', 'Tokyo', 'Paris', 'Munich', 'Zurich', 'Vienna'];
const subjects = ['football match', 'portrait session', 'music concert', 'press conference', 'archive editorial', 'tennis final'];
const credits = [
  'IMAGO / United Archives International',
  'IMAGO / teutopress',
  'IMAGO / Sven Simon',
  'IMAGO / ZUMA Wire',
  'IMAGO / Future Image',
  'IMAGO / Sportsphoto',
];
const restrictions = [
  'PUBLICATIONxINxGERxSUIxAUTxONLY',
  'EDITORIALxUSExONLY',
  'NOxCOMMERCIALxUSExONLY',
  'PUBLICATIONxINxGERxONLY',
  'WORLDWIDExRIGHTSxAVAILABLE',
];

function pad(value: number, width: number): string {
  return String(value).padStart(width, '0');
}

function formatDate(index: number): string {
  if (index % 997 === 0) return '31.02.1999';
  const year = 1940 + (index % 84);
  const month = (index % 12) + 1;
  const day = (index % 27) + 1;
  return `${pad(day, 2)}.${pad(month, 2)}.${year}`;
}

export function generateMediaItems(total: number): RawMediaItem[] {
  const generated = [...challengeItems];
  for (let index = generated.length; index < total; index += 1) {
    const person = people[index % people.length];
    const place = places[(index * 3) % places.length];
    const subject = subjects[(index * 5) % subjects.length];
    const credit = credits[(index * 7) % credits.length];
    const restriction = restrictions[(index * 11) % restrictions.length];
    const noisyDelimiter = index % 4 === 0 ? ' / ' : index % 4 === 1 ? '...' : ' ';

    generated.push({
      suchtext: `${person}${noisyDelimiter}${subject} ${place} ${1940 + (index % 84)} color editorial metadata-${index} ${restriction}`,
      bildnummer: `00${pad(40000000 + index, 8)}`,
      fotografen: credit,
      datum: formatDate(index),
      hoehe: String(700 + (index % 2800)),
      breite: String(900 + (index % 3200)),
    });
  }
  return generated;
}
