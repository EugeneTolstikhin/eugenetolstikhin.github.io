import { BadRequestException } from '@nestjs/common';
import { AuctionState, Prisma } from '@prisma/client';
import { AuctionsService } from './auctions.service';
import { AuctionStateMachine } from './state/auction-state-machine';

describe('AuctionsService business rules', () => {
  const fixedNow = new Date('2026-04-29T12:00:00.000Z');
  let prisma: {
    vehicle: {
      findUnique: jest.Mock;
    };
    auction: {
      create: jest.Mock;
      findMany: jest.Mock;
      findUnique: jest.Mock;
      update: jest.Mock;
    };
    bid: {
      create: jest.Mock;
      findFirst: jest.Mock;
    };
    $transaction: jest.Mock;
  };
  let service: AuctionsService;

  beforeEach(() => {
    jest.useFakeTimers();
    jest.setSystemTime(fixedNow);

    prisma = {
      vehicle: {
        findUnique: jest.fn(),
      },
      auction: {
        create: jest.fn(),
        findMany: jest.fn(),
        findUnique: jest.fn(),
        update: jest.fn(),
      },
      bid: {
        create: jest.fn(),
        findFirst: jest.fn(),
      },
      $transaction: jest.fn((callback: (tx: typeof prisma) => unknown) =>
        callback(prisma),
      ),
    };

    service = new AuctionsService(
      prisma as never,
      new AuctionStateMachine(),
    );
  });

  afterEach(() => {
    jest.useRealTimers();
  });

  it('creates an auction only when the VIN is not already assigned', async () => {
    const endTime = new Date('2026-04-30T12:00:00.000Z');
    const createdAuction = {
      id: 'auction-1',
      state: AuctionState.DRAFT,
      startingPrice: new Prisma.Decimal(15000),
      currentPrice: new Prisma.Decimal(15000),
      endTime,
      vehicle: {
        id: 'vehicle-1',
        brand: 'Audi',
        model: 'A4',
        vin: 'WAUZZZ8K9AA123456',
      },
    };

    prisma.vehicle.findUnique.mockResolvedValue(null);
    prisma.auction.create.mockResolvedValue(createdAuction);

    const auction = await service.create({
      brand: 'Audi',
      model: 'A4',
      vin: 'wauzzz8k9aa123456',
      startingPrice: 15000,
      endTime,
    });

    expect(auction).toBe(createdAuction);
    expect(prisma.vehicle.findUnique).toHaveBeenCalledWith({
      where: { vin: 'WAUZZZ8K9AA123456' },
      include: {
        auction: {
          include: { vehicle: true },
        },
      },
    });
    expect(prisma.auction.create).toHaveBeenCalledWith({
      data: {
        startingPrice: 15000,
        currentPrice: 15000,
        endTime,
        state: AuctionState.DRAFT,
        vehicle: {
          create: {
            brand: 'Audi',
            model: 'A4',
            vin: 'WAUZZZ8K9AA123456',
          },
        },
      },
      include: { vehicle: true },
    });
  });

  it('returns the existing auction when creation is retried with the same VIN and payload', async () => {
    const endTime = new Date('2026-04-30T12:00:00.000Z');
    const existingAuction = {
      id: 'auction-1',
      state: AuctionState.ACTIVE,
      startingPrice: new Prisma.Decimal(15000),
      currentPrice: new Prisma.Decimal(15000),
      endTime,
      winnerBuyerId: null,
      vehicle: {
        id: 'vehicle-1',
        brand: 'Audi',
        model: 'A4',
        vin: 'WAUZZZ8K9AA123456',
      },
    };

    prisma.vehicle.findUnique.mockResolvedValue({
      id: 'vehicle-1',
      vin: 'WAUZZZ8K9AA123456',
      auction: existingAuction,
    });

    await expect(
      service.create({
        brand: 'Audi',
        model: 'A4',
        vin: 'WAUZZZ8K9AA123456',
        startingPrice: 15000,
        endTime,
      }),
    ).resolves.toBe(existingAuction);

    expect(prisma.auction.create).not.toHaveBeenCalled();
  });

  it('connects an existing vehicle when the VIN exists without an auction', async () => {
    const endTime = new Date('2026-04-30T12:00:00.000Z');
    const createdAuction = {
      id: 'auction-1',
      state: AuctionState.DRAFT,
      startingPrice: new Prisma.Decimal(15000),
      endTime,
      vehicle: {
        id: 'vehicle-1',
        brand: 'Audi',
        model: 'A4',
        vin: 'WAUZZZ8K9AA123456',
      },
    };

    prisma.vehicle.findUnique.mockResolvedValue({
      id: 'vehicle-1',
      brand: 'Audi',
      model: 'A4',
      vin: 'WAUZZZ8K9AA123456',
      auction: null,
    });
    prisma.auction.create.mockResolvedValue(createdAuction);

    await expect(
      service.create({
        brand: 'Audi',
        model: 'A4',
        vin: 'WAUZZZ8K9AA123456',
        startingPrice: 15000,
        endTime,
      }),
    ).resolves.toBe(createdAuction);

    expect(prisma.auction.create).toHaveBeenCalledWith({
      data: {
        startingPrice: 15000,
        currentPrice: 15000,
        endTime,
        state: AuctionState.DRAFT,
        vehicle: {
          connect: { id: 'vehicle-1' },
        },
      },
      include: { vehicle: true },
    });
  });

  it('rejects a different auction request when the VIN is already active or sold', async () => {
    const endTime = new Date('2026-04-30T12:00:00.000Z');

    prisma.vehicle.findUnique.mockResolvedValue({
      id: 'vehicle-1',
      vin: 'WAUZZZ8K9AA123456',
      auction: {
        id: 'auction-1',
        state: AuctionState.ACTIVE,
        startingPrice: new Prisma.Decimal(15000),
        endTime,
        winnerBuyerId: null,
        vehicle: {
          brand: 'Audi',
          model: 'A4',
          vin: 'WAUZZZ8K9AA123456',
        },
      },
    });

    await expect(
      service.create({
        brand: 'Audi',
        model: 'A4',
        vin: 'WAUZZZ8K9AA123456',
        startingPrice: 16000,
        endTime,
      }),
    ).rejects.toThrow(BadRequestException);

    expect(prisma.auction.create).not.toHaveBeenCalled();
  });

  it('rejects bids for active auctions that are already expired', async () => {
    prisma.auction.findUnique.mockResolvedValue({
      id: 'auction-1',
      state: AuctionState.ACTIVE,
      currentPrice: new Prisma.Decimal(10000),
      endTime: new Date(fixedNow.getTime() - 1),
      vehicle: {
        id: 'vehicle-1',
        brand: 'Toyota',
        model: 'Supra',
        vin: 'JT2MA70L0H0123456',
      },
    });
    prisma.auction.update.mockResolvedValue({
      id: 'auction-1',
      state: AuctionState.ENDED,
      winnerBuyerId: 'buyer-1',
      winningBidId: 'bid-1',
    });
    prisma.bid.findFirst.mockResolvedValue({
      id: 'bid-1',
      auctionId: 'auction-1',
      buyerId: 'buyer-1',
      amount: new Prisma.Decimal(10000),
      createdAt: new Date('2026-04-29T11:59:59.000Z'),
    });

    await expect(
      service.placeBid('auction-1', 'buyer-1', { amount: 11000 }),
    ).rejects.toThrow(BadRequestException);

    expect(prisma.auction.update).toHaveBeenCalledWith({
      where: { id: 'auction-1' },
      data: {
        state: AuctionState.ENDED,
        winnerBuyerId: 'buyer-1',
        winningBidId: 'bid-1',
      },
      include: {
        vehicle: true,
        winnerBuyer: {
          select: { id: true, email: true },
        },
        winningBid: true,
      },
    });
    expect(prisma.bid.findFirst).toHaveBeenCalledWith({
      where: {
        auctionId: 'auction-1',
        createdAt: { lte: new Date(fixedNow.getTime() - 1) },
      },
      orderBy: [{ amount: 'desc' }, { createdAt: 'asc' }],
    });
    expect(prisma.bid.create).not.toHaveBeenCalled();
  });

  it('accepts matching highest bids made before expiry and ranks the earliest one as winner', async () => {
    const auction = {
      id: 'auction-1',
      state: AuctionState.ACTIVE,
      currentPrice: new Prisma.Decimal(12000),
      endTime: new Date(fixedNow.getTime() + 999),
      vehicle: {
        id: 'vehicle-1',
        brand: 'BMW',
        model: 'M3',
        vin: 'WBSBG9321VEY12345',
      },
    };

    prisma.auction.findUnique.mockResolvedValue(auction);
    prisma.bid.create
      .mockResolvedValueOnce({
        id: 'bid-1',
        auctionId: 'auction-1',
        buyerId: 'buyer-1',
        amount: new Prisma.Decimal(12000),
        createdAt: new Date('2026-04-29T12:00:00.100Z'),
      })
      .mockResolvedValueOnce({
        id: 'bid-2',
        auctionId: 'auction-1',
        buyerId: 'buyer-2',
        amount: new Prisma.Decimal(12000),
        createdAt: new Date('2026-04-29T12:00:00.500Z'),
      });
    prisma.auction.update.mockResolvedValue(auction);

    await service.placeBid('auction-1', 'buyer-1', { amount: 12000 });
    await service.placeBid('auction-1', 'buyer-2', { amount: 12000 });

    expect(prisma.bid.create).toHaveBeenNthCalledWith(1, {
      data: {
        auctionId: 'auction-1',
        buyerId: 'buyer-1',
        amount: new Prisma.Decimal(12000),
      },
    });
    expect(prisma.bid.create).toHaveBeenNthCalledWith(2, {
      data: {
        auctionId: 'auction-1',
        buyerId: 'buyer-2',
        amount: new Prisma.Decimal(12000),
      },
    });

    prisma.auction.findMany
      .mockResolvedValueOnce([
        {
          id: 'auction-1',
          state: AuctionState.ACTIVE,
          endTime: new Date(fixedNow.getTime() + 999),
        },
      ])
      .mockResolvedValueOnce([
        {
          ...auction,
          state: AuctionState.ENDED,
          bids: [
            {
              id: 'bid-1',
              buyerId: 'buyer-1',
              amount: new Prisma.Decimal(12000),
              createdAt: new Date('2026-04-29T12:00:00.100Z'),
            },
          ],
        },
      ]);
    prisma.bid.findFirst.mockResolvedValue({
      id: 'bid-1',
      auctionId: 'auction-1',
      buyerId: 'buyer-1',
      amount: new Prisma.Decimal(12000),
      createdAt: new Date('2026-04-29T12:00:00.100Z'),
    });

    jest.setSystemTime(new Date(fixedNow.getTime() + 1000));

    const auctions = await service.list();

    expect(prisma.auction.findMany).toHaveBeenLastCalledWith({
      orderBy: { createdAt: 'desc' },
      include: {
        vehicle: true,
        winnerBuyer: {
          select: { id: true, email: true },
        },
        winningBid: true,
        bids: {
          orderBy: [{ amount: 'desc' }, { createdAt: 'asc' }],
          take: 1,
          include: {
            buyer: {
              select: { id: true, email: true },
            },
          },
        },
      },
    });
    expect(prisma.bid.findFirst).toHaveBeenLastCalledWith({
      where: {
        auctionId: 'auction-1',
        createdAt: { lte: new Date(fixedNow.getTime() + 999) },
      },
      orderBy: [{ amount: 'desc' }, { createdAt: 'asc' }],
    });
    expect(auctions[0].bids[0].buyerId).toBe('buyer-1');
  });
});
