import { Module } from '@nestjs/common';
import { AuctionsController } from './auctions.controller';
import { AuctionsService } from './auctions.service';
import { AuctionStateMachine } from './state/auction-state-machine';

@Module({
  controllers: [AuctionsController],
  providers: [AuctionsService, AuctionStateMachine],
})
export class AuctionsModule {}
