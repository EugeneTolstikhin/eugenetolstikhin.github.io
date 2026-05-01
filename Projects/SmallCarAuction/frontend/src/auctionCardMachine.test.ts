import { describe, expect, it } from 'vitest';
import { auctionCardReducer } from './auctionCardMachine';

describe('auctionCardReducer', () => {
  it('moves from list to hover and back', () => {
    const hover = auctionCardReducer({ value: 'list' }, { type: 'POINTER_ENTER' });

    expect(hover).toEqual({ value: 'hover' });
    expect(auctionCardReducer(hover, { type: 'POINTER_LEAVE' })).toEqual({
      value: 'list',
    });
  });

  it('opens and returns from selected state', () => {
    const selected = auctionCardReducer({ value: 'hover' }, { type: 'GO_TO_AUCTION' });

    expect(selected).toEqual({ value: 'selected' });
    expect(auctionCardReducer(selected, { type: 'RETURN_TO_LIST' })).toEqual({
      value: 'list',
    });
  });

  it('keeps selected state stable on pointer leave', () => {
    expect(
      auctionCardReducer({ value: 'selected' }, { type: 'POINTER_LEAVE' }),
    ).toEqual({ value: 'selected' });
  });
});
