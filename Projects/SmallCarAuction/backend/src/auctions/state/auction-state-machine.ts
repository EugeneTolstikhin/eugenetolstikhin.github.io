import { BadRequestException, Injectable } from '@nestjs/common';
import { AuctionState } from '@prisma/client';

export type AuctionEvent = 'ACTIVATE' | 'EXPIRE' | 'CANCEL';

const transitions: Record<AuctionState, Partial<Record<AuctionEvent, AuctionState>>> = {
  DRAFT: {
    ACTIVATE: AuctionState.ACTIVE,
    CANCEL: AuctionState.CANCELLED,
  },
  ACTIVE: {
    EXPIRE: AuctionState.ENDED,
    CANCEL: AuctionState.CANCELLED,
  },
  ENDED: {},
  CANCELLED: {},
};

@Injectable()
export class AuctionStateMachine {
  transition(currentState: AuctionState, event: AuctionEvent): AuctionState {
    const nextState = transitions[currentState][event];

    if (!nextState) {
      throw new BadRequestException(
        `Cannot apply ${event} while auction is ${currentState}`,
      );
    }

    return nextState;
  }
}
