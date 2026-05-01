import { PrismaPg } from '@prisma/adapter-pg';
import { PrismaClient, UserRole } from '@prisma/client';
import { randomBytes, scrypt } from 'crypto';
import { promisify } from 'util';

const connectionString = process.env.DATABASE_URL;

if (!connectionString) {
  throw new Error('DATABASE_URL is required');
}

const prisma = new PrismaClient({
  adapter: new PrismaPg({ connectionString }),
});

const scryptAsync = promisify(scrypt);

async function hashPassword(password: string) {
  const salt = randomBytes(16).toString('hex');
  const key = (await scryptAsync(password, salt, 64)) as Buffer;

  return `scrypt:${salt}:${key.toString('hex')}`;
}

async function main() {
  const adminPasswordHash = await hashPassword(
    process.env.SEED_ADMIN_PASSWORD ?? 'AdminPass123!',
  );
  const buyerPasswordHash = await hashPassword(
    process.env.SEED_BUYER_PASSWORD ?? 'BuyerPass123!',
  );

  await prisma.user.upsert({
    where: { email: 'admin@example.com' },
    update: { passwordHash: adminPasswordHash },
    create: {
      email: 'admin@example.com',
      passwordHash: adminPasswordHash,
      role: UserRole.ADMIN,
    },
  });

  await prisma.user.upsert({
    where: { email: 'buyer@example.com' },
    update: { passwordHash: buyerPasswordHash },
    create: {
      email: 'buyer@example.com',
      passwordHash: buyerPasswordHash,
      role: UserRole.BUYER,
    },
  });
}

main()
  .finally(async () => {
    await prisma.$disconnect();
  })
  .catch(async (error) => {
    console.error(error);
    process.exit(1);
  });
