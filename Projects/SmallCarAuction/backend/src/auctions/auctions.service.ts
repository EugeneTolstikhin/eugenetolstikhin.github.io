import {
  BadRequestException,
  Injectable,
  NotFoundException,
  OnModuleDestroy,
  OnModuleInit,
} from '@nestjs/common';
import { AuctionState, Prisma } from '@prisma/client';
import { PrismaService } from '../prisma/prisma.service';
import { CreateAuctionDto } from './dto/create-auction.dto';
import { PlaceBidDto } from './dto/place-bid.dto';
import { AuctionStateMachine } from './state/auction-state-machine';

@Injectable()
export class AuctionsService implements OnModuleInit, OnModuleDestroy {
  private expiryTimer?: NodeJS.Timeout;

  constructor(
    private readonly prisma: PrismaService,
    private readonly stateMachine: AuctionStateMachine,
  ) {}

  onModuleInit() {
    void this.expireActiveAuctions();
    this.expiryTimer = setInterval(() => {
      void this.expireActiveAuctions();
    }, 1000);
  }

  onModuleDestroy() {
    if (this.expiryTimer) {
      clearInterval(this.expiryTimer);
    }
  }

  async create(dto: CreateAuctionDto) {
    const normalizedDto = {
      ...dto,
      brand: dto.brand.trim(),
      model: dto.model.trim(),
      vin: dto.vin.trim().toUpperCase(),
    };

    if (normalizedDto.endTime <= new Date()) {
      throw new BadRequestException('Auction end time must be in the future');
    }

    try {
      return await this.prisma.$transaction(
        async (tx) => {
          const existingVehicle = await this.findVehicleWithAuction(
            tx,
            normalizedDto.vin,
          );

          const existingAuction = existingVehicle?.auction;

          if (existingAuction) {
            if (this.isIdempotentCreateRequest(existingAuction, normalizedDto)) {
              return existingAuction;
            }

            if (
              existingAuction.state === AuctionState.ACTIVE ||
              existingAuction.winnerBuyerId
            ) {
              throw new BadRequestException(
                'Vehicle is already sold or assigned to an active auction',
              );
            }

            throw new BadRequestException(
              'Vehicle VIN is already assigned to another auction',
            );
          }

          if (existingVehicle && !this.isSameVehicle(existingVehicle, normalizedDto)) {
            throw new BadRequestException(
              'Vehicle VIN already exists with different vehicle details',
            );
          }

          return tx.auction.create({
            data: {
              startingPrice: normalizedDto.startingPrice,
              currentPrice: normalizedDto.startingPrice,
              endTime: normalizedDto.endTime,
              state: AuctionState.DRAFT,
              vehicle: existingVehicle
                ? { connect: { id: existingVehicle.id } }
                : {
                    create: {
                      brand: normalizedDto.brand,
                      model: normalizedDto.model,
                      vin: normalizedDto.vin,
                    },
                  },
            },
            include: { vehicle: true },
          });
        },
        { isolationLevel: Prisma.TransactionIsolationLevel.Serializable },
      );
    } catch (error) {
      if (!this.isCreateRaceError(error)) {
        throw error;
      }

      const existingVehicle = await this.findVehicleWithAuction(
        this.prisma,
        normalizedDto.vin,
      );

      if (
        existingVehicle?.auction &&
        this.isIdempotentCreateRequest(existingVehicle.auction, normalizedDto)
      ) {
        return existingVehicle.auction;
      }

      throw new BadRequestException(
        'Vehicle VIN is already assigned to another auction',
      );
    }
  }

  async activate(id: string) {
    const auction = await this.prisma.auction.findUnique({
      where: { id },
      include: { vehicle: true },
    });

    if (!auction) {
      throw new NotFoundException('Auction not found');
    }

    if (auction.endTime <= new Date()) {
      throw new BadRequestException('Expired auction cannot be activated');
    }

    const nextState = this.stateMachine.transition(auction.state, 'ACTIVATE');

    return this.prisma.auction.update({
      where: { id },
      data: { state: nextState },
      include: { vehicle: true },
    });
  }

  async list() {
    await this.expireActiveAuctions();

    return this.prisma.auction.findMany({
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
  }

  async getStatus(id: string) {
    const auction = await this.expireAuctionIfNeeded(id);

    return {
      auctionId: auction.id,
      status: auction.state,
      endTime: auction.endTime,
      winnerBuyerId: auction.winnerBuyerId,
      winningBidId: auction.winningBidId,
    };
  }

  async getHighestBid(id: string) {
    await this.ensureAuctionExistsAndRefreshState(id);

    const bid = await this.prisma.bid.findFirst({
      where: { auctionId: id },
      orderBy: [{ amount: 'desc' }, { createdAt: 'asc' }],
      include: {
        buyer: {
          select: { id: true, email: true },
        },
      },
    });

    return {
      auctionId: id,
      highestBid: bid,
    };
  }

  async placeBid(id: string, buyerId: string, dto: PlaceBidDto) {
    return this.prisma.$transaction(
      async (tx) => {
        const auction = await tx.auction.findUnique({
          where: { id },
          include: { vehicle: true },
        });

        if (!auction) {
          throw new NotFoundException('Auction not found');
        }

        if (auction.state === AuctionState.ACTIVE && auction.endTime <= new Date()) {
          await this.finishAuction(tx, id, auction.state, auction.endTime);
          throw new BadRequestException('Auction has expired');
        }

        if (auction.state !== AuctionState.ACTIVE) {
          throw new BadRequestException('Auction is not active');
        }

        const amount = new Prisma.Decimal(dto.amount);

        if (amount.lt(auction.currentPrice)) {
          throw new BadRequestException(
            'Bid must be equal to or greater than current price',
          );
        }

        const bid = await tx.bid.create({
          data: {
            auctionId: id,
            buyerId,
            amount,
          },
        });

        const updatedAuction = await tx.auction.update({
          where: { id },
          data: { currentPrice: amount },
          include: { vehicle: true },
        });

        return { auction: updatedAuction, bid };
      },
      { isolationLevel: Prisma.TransactionIsolationLevel.Serializable },
    );
  }

  private async expireActiveAuctions() {
    const activeExpiredAuctions = await this.prisma.auction.findMany({
      where: {
        state: AuctionState.ACTIVE,
        endTime: { lte: new Date() },
      },
      select: { id: true, state: true, endTime: true },
    });

    await Promise.all(
      activeExpiredAuctions.map((auction) =>
        this.prisma.$transaction((tx) =>
          this.finishAuction(tx, auction.id, auction.state, auction.endTime),
        ),
      ),
    );
  }

  private async ensureAuctionExistsAndRefreshState(id: string) {
    return this.expireAuctionIfNeeded(id);
  }

  private async expireAuctionIfNeeded(id: string) {
    const auction = await this.prisma.auction.findUnique({
      where: { id },
      include: { vehicle: true },
    });

    if (!auction) {
      throw new NotFoundException('Auction not found');
    }

    if (auction.state !== AuctionState.ACTIVE || auction.endTime > new Date()) {
      return auction;
    }

    return this.prisma.$transaction((tx) =>
      this.finishAuction(tx, id, auction.state, auction.endTime),
    );
  }

  private async finishAuction(
    tx: Prisma.TransactionClient,
    id: string,
    state: AuctionState,
    endTime: Date,
  ) {
    const winningBid = await tx.bid.findFirst({
      where: {
        auctionId: id,
        createdAt: { lte: endTime },
      },
      orderBy: [{ amount: 'desc' }, { createdAt: 'asc' }],
    });

    return tx.auction.update({
      where: { id },
      data: {
        state: this.stateMachine.transition(state, 'EXPIRE'),
        winnerBuyerId: winningBid?.buyerId,
        winningBidId: winningBid?.id,
      },
      include: {
        vehicle: true,
        winnerBuyer: {
          select: { id: true, email: true },
        },
        winningBid: true,
      },
    });
  }

  private isIdempotentCreateRequest(
    auction: {
      startingPrice: Prisma.Decimal;
      endTime: Date;
      vehicle: {
        brand: string;
        model: string;
        vin: string;
      };
    },
    dto: CreateAuctionDto,
  ) {
    return (
      auction.vehicle.brand === dto.brand &&
      auction.vehicle.model === dto.model &&
      auction.vehicle.vin === dto.vin &&
      auction.startingPrice.eq(dto.startingPrice) &&
      auction.endTime.getTime() === dto.endTime.getTime()
    );
  }

  private isSameVehicle(
    vehicle: {
      brand: string;
      model: string;
      vin: string;
    },
    dto: Pick<CreateAuctionDto, 'brand' | 'model' | 'vin'>,
  ) {
    return (
      vehicle.brand === dto.brand &&
      vehicle.model === dto.model &&
      vehicle.vin === dto.vin
    );
  }

  private findVehicleWithAuction(
    db: Pick<Prisma.TransactionClient, 'vehicle'> | Pick<PrismaService, 'vehicle'>,
    vin: string,
  ) {
    return db.vehicle.findUnique({
      where: { vin },
      include: {
        auction: {
          include: { vehicle: true },
        },
      },
    });
  }

  private isCreateRaceError(error: unknown) {
    return (
      error instanceof Prisma.PrismaClientKnownRequestError &&
      (error.code === 'P2002' || error.code === 'P2034')
    );
  }
}
