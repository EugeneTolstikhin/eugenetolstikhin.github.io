export type AuctionCardState =
  | { value: 'list' }
  | { value: 'hover' }
  | { value: 'selected' };

export type AuctionCardEvent =
  | { type: 'POINTER_ENTER' }
  | { type: 'POINTER_LEAVE' }
  | { type: 'GO_TO_AUCTION' }
  | { type: 'RETURN_TO_LIST' };

export function auctionCardReducer(
  state: AuctionCardState,
  event: AuctionCardEvent,
): AuctionCardState {
  switch (state.value) {
    case 'list':
      if (event.type === 'POINTER_ENTER') {
        return { value: 'hover' };
      }
      if (event.type === 'GO_TO_AUCTION') {
        return { value: 'selected' };
      }
      return state;

    case 'hover':
      if (event.type === 'POINTER_LEAVE') {
        return { value: 'list' };
      }
      if (event.type === 'GO_TO_AUCTION') {
        return { value: 'selected' };
      }
      return state;

    case 'selected':
      if (event.type === 'RETURN_TO_LIST') {
        return { value: 'list' };
      }
      return state;
  }
}
