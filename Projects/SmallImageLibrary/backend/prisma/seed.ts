import { PrismaClient } from '@prisma/client';
import { generateMediaItems } from '../src/search/fixture-data';
import { normalizeMediaItem } from '../src/search/preprocess';

const prisma = new PrismaClient();

function toDate(value: string | null): Date | null {
  return value ? new Date(`${value}T00:00:00.000Z`) : null;
}

async function main() {
  const targetSize = Number(process.env.SEED_SIZE ?? 10000);
  const count = await prisma.mediaItem.count();

  if (count >= targetSize) {
    console.log(`Seed skipped; ${count} media rows already exist.`);
    return;
  }

  const items = generateMediaItems(targetSize).map((item, index) => normalizeMediaItem(item, index + 1));
  const batchSize = 500;

  for (let index = 0; index < items.length; index += batchSize) {
    const batch = items.slice(index, index + batchSize);
    await prisma.mediaItem.createMany({
      data: batch.map((item) => ({
        suchtext: item.suchtext,
        bildnummer: item.bildnummer,
        fotografen: item.fotografen,
        datum: item.datum,
        hoehe: item.hoehe,
        breite: item.breite,
        dateIso: toDate(item.dateIso),
        restrictions: item.restrictions,
        searchableText: item.searchableText,
        tokens: item.tokens,
      })),
      skipDuplicates: true,
    });
  }

  console.log(`Seeded up to ${targetSize} media rows.`);
}

main()
  .catch((error) => {
    console.error(error);
    process.exitCode = 1;
  })
  .finally(async () => {
    await prisma.$disconnect();
  });
