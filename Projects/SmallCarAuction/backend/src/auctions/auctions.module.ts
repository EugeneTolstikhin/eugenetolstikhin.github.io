import { Module } from '@nestjs/common';
import { AuctionsController } from './auctions.controller';
import { AuctionEventsService } from './auction-events.service';
import { AuctionsService } from './auctions.service';
import { AuctionStateMachine } from './state/auction-state-machine';

@Module({
  controllers: [AuctionsController],
  providers: [AuctionsService, AuctionStateMachine, AuctionEventsService],
})
export class AuctionsModule {}
