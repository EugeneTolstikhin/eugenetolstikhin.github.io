import { PrismaPg } from '@prisma/adapter-pg';
import { AuctionState, PrismaClient, UserRole } from '@prisma/client';
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
  const buyerTwoPasswordHash = await hashPassword(
    process.env.SEED_BUYER_TWO_PASSWORD ?? 'BuyerTwoPass123!',
  );
  const demoBidderPasswordHash = await hashPassword(
    process.env.SEED_DEMO_BIDDER_PASSWORD ?? 'DemoBidderPass123!',
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

  await prisma.user.upsert({
    where: { email: 'buyer2@example.com' },
    update: { passwordHash: buyerTwoPasswordHash },
    create: {
      email: 'buyer2@example.com',
      passwordHash: buyerTwoPasswordHash,
      role: UserRole.BUYER,
    },
  });

  const demoBidder = await prisma.user.upsert({
    where: { email: 'demo-bidder@example.com' },
    update: { passwordHash: demoBidderPasswordHash },
    create: {
      email: 'demo-bidder@example.com',
      passwordHash: demoBidderPasswordHash,
      role: UserRole.BUYER,
    },
  });

  await seedDemoAuctions(demoBidder.id);
}

async function seedDemoAuctions(buyerId: string) {
  const now = new Date();
  const demoAuctions = [
    {
      brand: 'Toyota',
      model: 'Supra',
      vin: 'JT2MA70L0H0123456',
      startingPrice: 25000,
      currentPrice: 28000,
      seedBidAmount: 28000,
      endTime: addHours(now, 8),
    },
    {
      brand: 'BMW',
      model: 'M3',
      vin: 'WBSBG9321VEY12345',
      startingPrice: 32000,
      currentPrice: 34500,
      seedBidAmount: 34500,
      endTime: addHours(now, 14),
    },
    {
      brand: 'Audi',
      model: 'RS5',
      vin: 'WUAC6AFR8FA901234',
      startingPrice: 38000,
      currentPrice: 39750,
      seedBidAmount: 39750,
      endTime: addHours(now, 20),
    },
    {
      brand: 'Mercedes-Benz',
      model: 'C63 AMG',
      vin: 'WDDGF77X88F123456',
      startingPrice: 41000,
      currentPrice: 41000,
      endTime: addHours(now, 26),
    },
    {
      brand: 'Porsche',
      model: 'Cayman S',
      vin: 'WP0AB2A89EK123456',
      startingPrice: 46000,
      currentPrice: 46000,
      endTime: addHours(now, 32),
    },
    {
      brand: 'Ford',
      model: 'Mustang GT',
      vin: '1FA6P8CF7K5123456',
      startingPrice: 29000,
      currentPrice: 29000,
      endTime: addHours(now, 38),
    },
    {
      brand: 'Nissan',
      model: '370Z Nismo',
      vin: 'JN1AZ4EH3FM123456',
      startingPrice: 27000,
      currentPrice: 27000,
      endTime: addHours(now, 44),
    },
    {
      brand: 'Chevrolet',
      model: 'Camaro SS',
      vin: '1G1FH1R77H0123456',
      startingPrice: 31000,
      currentPrice: 31000,
      endTime: addHours(now, 50),
    },
    {
      brand: 'Lexus',
      model: 'RC F',
      vin: 'JTHHP5BC1G5123456',
      startingPrice: 36000,
      currentPrice: 36000,
      endTime: addHours(now, 56),
    },
    {
      brand: 'Subaru',
      model: 'WRX STI',
      vin: 'JF1VA2M67G9823456',
      startingPrice: 24000,
      currentPrice: 24000,
      endTime: addHours(now, 62),
    },
  ];

  for (const demoAuction of demoAuctions) {
    const vehicle = await prisma.vehicle.upsert({
      where: { vin: demoAuction.vin },
      update: {
        brand: demoAuction.brand,
        model: demoAuction.model,
      },
      create: {
        brand: demoAuction.brand,
        model: demoAuction.model,
        vin: demoAuction.vin,
      },
    });

    const auction = await prisma.auction.upsert({
      where: { vehicleId: vehicle.id },
      update: {
        startingPrice: demoAuction.startingPrice,
        currentPrice: demoAuction.currentPrice,
        endTime: demoAuction.endTime,
        state: AuctionState.ACTIVE,
        winnerBuyerId: null,
        winningBidId: null,
      },
      create: {
        vehicleId: vehicle.id,
        startingPrice: demoAuction.startingPrice,
        currentPrice: demoAuction.currentPrice,
        endTime: demoAuction.endTime,
        state: AuctionState.ACTIVE,
      },
    });

    await prisma.bid.deleteMany({
      where: {
        auctionId: auction.id,
        buyer: {
          email: {
            in: ['buyer@example.com', 'buyer2@example.com', 'demo-bidder@example.com'],
          },
        },
      },
    });

    if (demoAuction.seedBidAmount) {
      await prisma.bid.create({
        data: {
          auctionId: auction.id,
          buyerId,
          amount: demoAuction.seedBidAmount,
        },
      });
    }
  }
}

function addHours(date: Date, hours: number) {
  return new Date(date.getTime() + hours * 60 * 60 * 1000);
}

main()
  .finally(async () => {
    await prisma.$disconnect();
  })
  .catch(async (error) => {
    console.error(error);
    process.exit(1);
  });
