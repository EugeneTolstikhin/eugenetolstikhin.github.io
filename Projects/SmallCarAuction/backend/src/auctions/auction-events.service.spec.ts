import { firstValueFrom } from 'rxjs';
import { AuctionEventsService } from './auction-events.service';

describe('AuctionEventsService', () => {
  it('streams published auction events as SSE messages', async () => {
    const service = new AuctionEventsService();
    const nextEvent = firstValueFrom(service.stream());

    service.publish('auction.highestBidChanged', 'auction-1');

    await expect(nextEvent).resolves.toMatchObject({
      type: 'auction.highestBidChanged',
      data: {
        type: 'auction.highestBidChanged',
        auctionId: 'auction-1',
      },
    });
  });
});
