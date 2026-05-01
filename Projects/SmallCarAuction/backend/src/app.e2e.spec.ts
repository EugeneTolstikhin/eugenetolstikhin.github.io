import { BadRequestException, INestApplication, UnauthorizedException, ValidationPipe } from '@nestjs/common';
import { Test } from '@nestjs/testing';
import { UserRole } from '@prisma/client';
import session from 'express-session';
import request from 'supertest';
import { AuthController } from './auth/auth.controller';
import { AuthService } from './auth/auth.service';
import { RolesGuard } from './auth/guards/roles.guard';
import { SessionAuthGuard } from './auth/guards/session-auth.guard';
import { AuctionsController } from './auctions/auctions.controller';
import { AuctionEventsService } from './auctions/auction-events.service';
import { AuctionsService } from './auctions/auctions.service';
import { CsrfGuard } from './security/csrf.guard';

describe('SmallCarAuction API security and scenario coverage', () => {
  let app: INestApplication;
  let authService: {
    beginLogin: jest.Mock;
    verifyOtp: jest.Mock;
  };
  let auctionsService: {
    create: jest.Mock;
    activate: jest.Mock;
    list: jest.Mock;
    getStatus: jest.Mock;
    getHighestBid: jest.Mock;
    placeBid: jest.Mock;
  };
  let auctionEventsService: {
    stream: jest.Mock;
  };

  beforeEach(async () => {
    authService = {
      beginLogin: jest.fn(),
      verifyOtp: jest.fn(),
    };
    auctionsService = {
      create: jest.fn(),
      activate: jest.fn(),
      list: jest.fn(),
      getStatus: jest.fn(),
      getHighestBid: jest.fn(),
      placeBid: jest.fn(),
    };
    auctionEventsService = {
      stream: jest.fn(),
    };

    const moduleRef = await Test.createTestingModule({
      controllers: [AuthController, AuctionsController],
      providers: [
        SessionAuthGuard,
        CsrfGuard,
        RolesGuard,
        { provide: AuthService, useValue: authService },
        { provide: AuctionsService, useValue: auctionsService },
        { provide: AuctionEventsService, useValue: auctionEventsService },
      ],
    }).compile();

    app = moduleRef.createNestApplication();
    app.use(
      session({
        secret: 'test-session-secret',
        resave: false,
        saveUninitialized: false,
      }),
    );
    app.useGlobalPipes(
      new ValidationPipe({
        whitelist: true,
        forbidNonWhitelisted: true,
        transform: true,
      }),
    );
    await app.init();
  });

  afterEach(async () => {
    await app.close();
  });

  it('returns 401 for a wrong password', async () => {
    authService.beginLogin.mockRejectedValue(
      new UnauthorizedException('Invalid email or password'),
    );

    await request(app.getHttpServer())
      .post('/auth/login')
      .send({ email: 'admin@example.com', password: 'wrong-password' })
      .expect(401);
  });

  it('returns 401 when verifying OTP without a pending login session', async () => {
    await request(app.getHttpServer())
      .post('/auth/verify-otp')
      .send({ code: '123456' })
      .expect(401);
  });

  it('allows an admin session to create and activate an auction', async () => {
    const agent = await loginAs(UserRole.ADMIN, 'admin@example.com');
    const auction = {
      id: 'auction-1',
      state: 'DRAFT',
      vehicle: { vin: 'JT2MA70L0H0123456' },
    };
    auctionsService.create.mockResolvedValue(auction);
    auctionsService.activate.mockResolvedValue({ ...auction, state: 'ACTIVE' });

    await agent
      .post('/auctions')
      .set('X-CSRF-Token', agent.csrfToken)
      .send({
        brand: 'Toyota',
        model: 'Supra',
        vin: 'JT2MA70L0H0123456',
        startingPrice: 25000,
        endTime: '2026-12-31T18:00:00.000Z',
      })
      .expect(201)
      .expect(({ body }) => expect(body).toMatchObject(auction));

    await agent
      .post('/auctions/auction-1/activate')
      .set('X-CSRF-Token', agent.csrfToken)
      .expect(201)
      .expect(({ body }) => expect(body.state).toBe('ACTIVE'));

    expect(auctionsService.create).toHaveBeenCalledTimes(1);
    expect(auctionsService.activate).toHaveBeenCalledWith('auction-1');
  });

  it('returns 403 when a buyer tries to create an auction', async () => {
    const agent = await loginAs(UserRole.BUYER, 'buyer@example.com');

    await agent
      .post('/auctions')
      .set('X-CSRF-Token', agent.csrfToken)
      .send({
        brand: 'Toyota',
        model: 'Supra',
        vin: 'JT2MA70L0H0123456',
        startingPrice: 25000,
        endTime: '2026-12-31T18:00:00.000Z',
      })
      .expect(403);

    expect(auctionsService.create).not.toHaveBeenCalled();
  });

  it('returns 403 for unsafe authenticated requests without X-CSRF-Token', async () => {
    const agent = await loginAs(UserRole.ADMIN, 'admin@example.com');

    await agent
      .post('/auctions')
      .send({
        brand: 'Toyota',
        model: 'Supra',
        vin: 'JT2MA70L0H0123456',
        startingPrice: 25000,
        endTime: '2026-12-31T18:00:00.000Z',
      })
      .expect(403);

    expect(auctionsService.create).not.toHaveBeenCalled();
  });

  it('allows a buyer to bid through the API when the auction is active', async () => {
    const agent = await loginAs(UserRole.BUYER, 'buyer@example.com');
    auctionsService.placeBid.mockResolvedValue({
      auction: { id: 'auction-1', state: 'ACTIVE', currentPrice: 27500 },
      bid: { id: 'bid-1', amount: 27500 },
    });

    await agent
      .post('/auctions/auction-1/bids')
      .set('X-CSRF-Token', agent.csrfToken)
      .send({ amount: 27500 })
      .expect(201)
      .expect(({ body }) => {
        expect(body.auction.state).toBe('ACTIVE');
        expect(body.bid.amount).toBe(27500);
      });

    expect(auctionsService.placeBid).toHaveBeenCalledWith(
      'auction-1',
      expect.any(String),
      { amount: 27500 },
    );
  });

  it('returns 400 when a bid is below the current price', async () => {
    const agent = await loginAs(UserRole.BUYER, 'buyer@example.com');
    auctionsService.placeBid.mockRejectedValue(
      new BadRequestException('Bid must be equal to or greater than current price'),
    );

    await agent
      .post('/auctions/auction-1/bids')
      .set('X-CSRF-Token', agent.csrfToken)
      .send({ amount: 999 })
      .expect(400);
  });

  async function loginAs(role: UserRole, email: string) {
    const testAgent = request.agent(app.getHttpServer()) as unknown as request.SuperAgentTest & {
      csrfToken: string;
    };

    authService.beginLogin.mockResolvedValueOnce({
      challengeId: `challenge-${role}`,
      userId: `user-${role}`,
      email,
      expiresInSeconds: 300,
    });
    authService.verifyOtp.mockResolvedValueOnce({
      id: `user-${role}`,
      email,
      role,
    });

    await testAgent
      .post('/auth/login')
      .send({ email, password: `${role}Pass123!` })
      .expect(201);

    const verifyResponse = await testAgent
      .post('/auth/verify-otp')
      .send({ code: '123456' })
      .expect(201);

    testAgent.csrfToken = verifyResponse.body.csrfToken;
    return testAgent;
  }
});
