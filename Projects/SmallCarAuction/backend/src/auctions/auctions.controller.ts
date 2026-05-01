import {
  Body,
  Controller,
  Get,
  Param,
  Post,
  Req,
  UseGuards,
} from '@nestjs/common';
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
export class AuctionsController {
  constructor(private readonly auctionsService: AuctionsService) {}

  @Post()
  @Roles(UserRole.ADMIN)
  create(@Body() dto: CreateAuctionDto) {
    return this.auctionsService.create(dto);
  }

  @Post(':id/activate')
  @Roles(UserRole.ADMIN)
  activate(@Param('id') id: string) {
    return this.auctionsService.activate(id);
  }

  @Get()
  @Roles(UserRole.ADMIN, UserRole.BUYER)
  list() {
    return this.auctionsService.list();
  }

  @Get(':id/status')
  @Roles(UserRole.BUYER)
  getStatus(@Param('id') id: string) {
    return this.auctionsService.getStatus(id);
  }

  @Get(':id/highest-bid')
  @Roles(UserRole.BUYER)
  getHighestBid(@Param('id') id: string) {
    return this.auctionsService.getHighestBid(id);
  }

  @Post(':id/bids')
  @Roles(UserRole.BUYER)
  placeBid(
    @Param('id') id: string,
    @Body() dto: PlaceBidDto,
    @Req() request: Request,
  ) {
    return this.auctionsService.placeBid(id, request.session.user!.id, dto);
  }
}
