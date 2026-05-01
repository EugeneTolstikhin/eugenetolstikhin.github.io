import { Injectable, MessageEvent } from '@nestjs/common';
import { Observable, Subject, map } from 'rxjs';

export type AuctionEventType =
  | 'auction.created'
  | 'auction.activated'
  | 'auction.highestBidChanged'
  | 'auction.expired';

export type AuctionEvent = {
  type: AuctionEventType;
  auctionId: string;
  occurredAt: string;
};

@Injectable()
export class AuctionEventsService {
  private readonly events = new Subject<AuctionEvent>();

  publish(type: AuctionEventType, auctionId: string) {
    this.events.next({
      type,
      auctionId,
      occurredAt: new Date().toISOString(),
    });
  }

  stream(): Observable<MessageEvent> {
    return this.events.asObservable().pipe(
      map((event) => ({
        type: event.type,
        data: event,
      })),
    );
  }
}
