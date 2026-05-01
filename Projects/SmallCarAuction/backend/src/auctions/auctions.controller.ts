import {
  Body,
  Controller,
  Get,
  Param,
  Post,
  Req,
  UseGuards,
} from '@nestjs/common';
import {
  ApiBadRequestResponse,
  ApiCookieAuth,
  ApiCreatedResponse,
  ApiForbiddenResponse,
  ApiOkResponse,
  ApiOperation,
  ApiSecurity,
  ApiTags,
  ApiUnauthorizedResponse,
} from '@nestjs/swagger';
import { UserRole } from '@prisma/client';
import { Request } from 'express';
import { Roles } from '../auth/roles.decorator';
import { RolesGuard } from '../auth/guards/roles.guard';
import { SessionAuthGuard } from '../auth/guards/session-auth.guard';
import { CsrfGuard } from '../security/csrf.guard';
import { AuctionsService } from './auctions.service';
import { CreateAuctionDto } from './dto/create-auction.dto';
import { PlaceBidDto } from './dto/place-bid.dto';

@Controller('auctions')
@UseGuards(SessionAuthGuard, CsrfGuard, RolesGuard)
@ApiTags('auctions')
@ApiCookieAuth()
@ApiUnauthorizedResponse({ description: 'Login required.' })
export class AuctionsController {
  constructor(private readonly auctionsService: AuctionsService) {}

  @Post()
  @Roles(UserRole.ADMIN)
  @ApiOperation({ summary: 'Create a draft auction for a vehicle VIN' })
  @ApiSecurity('csrf')
  @ApiCreatedResponse({ description: 'Draft auction created or idempotently returned.' })
  @ApiBadRequestResponse({ description: 'Invalid auction payload or VIN conflict.' })
  @ApiForbiddenResponse({ description: 'Admin role required or invalid CSRF token.' })
  create(@Body() dto: CreateAuctionDto) {
    return this.auctionsService.create(dto);
  }

  @Post(':id/activate')
  @Roles(UserRole.ADMIN)
  @ApiOperation({ summary: 'Activate a draft auction' })
  @ApiSecurity('csrf')
  @ApiCreatedResponse({ description: 'Auction transitioned to ACTIVE.' })
  @ApiBadRequestResponse({ description: 'Auction cannot be activated.' })
  @ApiForbiddenResponse({ description: 'Admin role required or invalid CSRF token.' })
  activate(@Param('id') id: string) {
    return this.auctionsService.activate(id);
  }

  @Get()
  @Roles(UserRole.ADMIN, UserRole.BUYER)
  @ApiOperation({ summary: 'List auctions with vehicle, winner, and highest bid candidate' })
  @ApiOkResponse({ description: 'Auction list.' })
  @ApiForbiddenResponse({ description: 'Admin or buyer role required.' })
  list() {
    return this.auctionsService.list();
  }

  @Get(':id/status')
  @Roles(UserRole.BUYER)
  @ApiOperation({ summary: 'Get current auction status' })
  @ApiOkResponse({
    schema: {
      example: {
        auctionId: 'auction-1',
        status: 'ACTIVE',
        endTime: '2026-12-31T18:00:00.000Z',
        winnerBuyerId: null,
        winningBidId: null,
      },
    },
  })
  @ApiForbiddenResponse({ description: 'Buyer role required.' })
  getStatus(@Param('id') id: string) {
    return this.auctionsService.getStatus(id);
  }

  @Get(':id/highest-bid')
  @Roles(UserRole.BUYER)
  @ApiOperation({ summary: 'Get the current highest bid for an auction' })
  @ApiOkResponse({ description: 'Highest bid or null.' })
  @ApiForbiddenResponse({ description: 'Buyer role required.' })
  getHighestBid(@Param('id') id: string) {
    return this.auctionsService.getHighestBid(id);
  }

  @Post(':id/bids')
  @Roles(UserRole.BUYER)
  @ApiOperation({ summary: 'Place a bid on an active auction' })
  @ApiSecurity('csrf')
  @ApiCreatedResponse({ description: 'Bid accepted and auction current price updated.' })
  @ApiBadRequestResponse({ description: 'Auction inactive, expired, or bid below current price.' })
  @ApiForbiddenResponse({ description: 'Buyer role required or invalid CSRF token.' })
  placeBid(
    @Param('id') id: string,
    @Body() dto: PlaceBidDto,
    @Req() request: Request,
  ) {
    return this.auctionsService.placeBid(id, request.session.user!.id, dto);
  }
}
