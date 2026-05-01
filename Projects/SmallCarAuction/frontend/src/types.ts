export type UserRole = 'ADMIN' | 'BUYER';

export type SessionUser = {
  id: string;
  email: string;
  role: UserRole;
  csrfToken: string;
};

export type AuctionState = 'DRAFT' | 'ACTIVE' | 'ENDED' | 'CANCELLED';

export type Auction = {
  id: string;
  startingPrice: string | number;
  currentPrice: string | number;
  endTime: string;
  state: AuctionState;
  winnerBuyerId?: string | null;
  winningBidId?: string | null;
  vehicle: {
    id: string;
    brand: string;
    model: string;
    vin: string;
  };
  bids?: Array<{
    id: string;
    amount: string | number;
    createdAt: string;
    buyer?: {
      id: string;
      email: string;
    };
  }>;
};

export type Bid = NonNullable<Auction['bids']>[number];

export type BidResponse = {
  auction: Auction;
  bid: {
    id: string;
    amount: string | number;
    createdAt?: string;
  };
  highestBid?: Bid | null;
  isWinningBid?: boolean;
};
