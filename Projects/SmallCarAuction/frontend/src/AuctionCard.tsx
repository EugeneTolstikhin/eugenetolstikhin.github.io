'use client';

import { ArrowLeft, ExternalLink, ImageOff, Send } from 'lucide-react';
import { FormEvent, PointerEvent, useEffect, useReducer, useRef, useState } from 'react';
import { auctionCardReducer } from './auctionCardMachine';
import { formatDateTime, formatMoney } from './format';
import type { Auction } from './types';

type Props = {
  auction: Auction;
  csrfToken?: string;
  currentUserId?: string;
  onBid: (
    auctionId: string,
    amount: number,
  ) => Promise<{ isWinningBid?: boolean } | void>;
};

export function AuctionCard({ auction, csrfToken, currentUserId, onBid }: Props) {
  const [state, dispatch] = useReducer(auctionCardReducer, { value: 'list' });
  const [amount, setAmount] = useState('');
  const [message, setMessage] = useState<{ tone: 'success' | 'error'; text: string }>();
  const [isSubmitting, setIsSubmitting] = useState(false);
  const leaveTimer = useRef<number | undefined>(undefined);
  const isSelected = state.value === 'selected';
  const isHover = state.value === 'hover';
  const topBid = auction.bids?.[0];
  const hasBids = Boolean(topBid);
  const displayPrice = topBid?.amount ?? auction.startingPrice;
  const isCurrentUserLeading = Boolean(
    currentUserId && topBid?.buyer?.id === currentUserId,
  );
  const isEnded = auction.state === 'ENDED';
  const hasWinner = Boolean(topBid && (auction.winnerBuyerId || topBid.buyer?.id));
  const didCurrentUserWin = Boolean(
    isEnded &&
      currentUserId &&
      (auction.winnerBuyerId === currentUserId || topBid?.buyer?.id === currentUserId),
  );
  const endedOutcomeLabel = didCurrentUserWin ? 'Won' : 'Lost';
  const hasAnotherLeader = Boolean(topBid && currentUserId && !isCurrentUserLeading);
  const bidStatus = getBidStatus({
    didCurrentUserWin,
    hasAnotherLeader,
    hasBids,
    isCurrentUserLeading,
    isEnded,
  });
  const title = `${auction.vehicle.brand} ${auction.vehicle.model}`;

  useEffect(() => {
    return () => {
      window.clearTimeout(leaveTimer.current);
    };
  }, []);

  function enterHover() {
    window.clearTimeout(leaveTimer.current);
    dispatch({ type: 'POINTER_ENTER' });
  }

  function goToAuction() {
    window.clearTimeout(leaveTimer.current);
    dispatch({ type: 'GO_TO_AUCTION' });
  }

  function handlePointerLeave(event: PointerEvent<HTMLElement>) {
    const nextTarget = event.relatedTarget;

    if (nextTarget instanceof Node && event.currentTarget.contains(nextTarget)) {
      return;
    }

    leaveTimer.current = window.setTimeout(() => {
      dispatch({ type: 'POINTER_LEAVE' });
    }, 120);
  }

  async function handleSubmit(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setMessage(undefined);

    const bidAmount = Number(amount);

    if (!Number.isFinite(bidAmount) || bidAmount <= 0) {
      setMessage({ tone: 'error', text: 'Enter a positive bid amount.' });
      return;
    }

    setIsSubmitting(true);

    try {
      const bidResult = await onBid(auction.id, bidAmount);
      setAmount('');
      setMessage({
        tone: 'success',
        text: bidResult?.isWinningBid
          ? 'Your bid was accepted. You are currently leading this auction.'
          : 'Your bid was accepted, but an earlier bid at the same amount is still leading. Raise your bid to take the lead.',
      });
    } catch (error) {
      setMessage({
        tone: 'error',
        text:
          error instanceof Error
            ? humanizeBidError(error.message)
            : 'The bid could not be placed. Please try again.',
      });
    } finally {
      setIsSubmitting(false);
    }
  }

  return (
    <article
      className={`auction-card auction-card--${state.value}`}
      tabIndex={0}
      onPointerEnter={enterHover}
      onPointerLeave={handlePointerLeave}
      onFocus={enterHover}
      onClick={() => {
        if (state.value !== 'selected') {
          goToAuction();
        }
      }}
      onKeyDown={(event) => {
        if (event.key === 'Enter') {
          goToAuction();
        }
      }}
      data-testid={`auction-card-${auction.id}`}
    >
      <div className="auction-card__media" aria-label={`No photo for ${title}`}>
        <ImageOff aria-hidden="true" />
        <span>No Photo</span>
      </div>

      <div className="auction-card__body">
        <div className="auction-card__topline">
          <h2>{title}</h2>
          <div className="auction-card__badges">
            {isEnded && currentUserId && hasWinner && (
              <span
                className={`outcome-badge ${
                  didCurrentUserWin ? 'outcome-badge--won' : 'outcome-badge--lost'
                }`}
              >
                {endedOutcomeLabel}
              </span>
            )}
            <StatusTag state={auction.state} />
          </div>
        </div>

        {(isHover || isSelected) && (
          <p className="auction-card__vin">VIN {auction.vehicle.vin}</p>
        )}

        {!isSelected && (
          <div className="auction-card__action-row">
            <button
              className="icon-text-button auction-card__enter"
              type="button"
              onPointerDown={goToAuction}
              onClick={(event) => {
                event.stopPropagation();
                goToAuction();
              }}
            >
              <ExternalLink aria-hidden="true" />
              Go to Auction
            </button>
          </div>
        )}

        {isSelected && (
          <>
            <button
              className="icon-text-button auction-card__return"
            type="button"
            onClick={(event) => {
              event.stopPropagation();
              dispatch({ type: 'RETURN_TO_LIST' });
            }}
          >
              <ArrowLeft aria-hidden="true" />
              Return to Auction list
            </button>

            {bidStatus && (
              <section
                className={`bid-status-tile bid-status-tile--${bidStatus.tone}`}
                aria-live="polite"
              >
                <strong>{bidStatus.title}</strong>
                <span>{bidStatus.description}</span>
              </section>
            )}

            {isEnded ? (
              <section className="readonly-auction" aria-label="Auction closed">
                <strong>Bidding is closed</strong>
                <span>
                  {!hasBids
                    ? 'This auction ended without bids.'
                    : didCurrentUserWin
                      ? 'Congratulations. Your bid won this auction.'
                      : 'This auction has ended, so new bids are no longer accepted.'}
                </span>
              </section>
            ) : (
              <form className="bid-form" onSubmit={handleSubmit}>
                <label htmlFor={`bid-${auction.id}`}>Bid amount</label>
                <div className="bid-form__row">
                  <input
                    id={`bid-${auction.id}`}
                    inputMode="decimal"
                    min="0.01"
                    step="0.01"
                    type="number"
                    value={amount}
                    onChange={(event) => setAmount(event.target.value)}
                    placeholder="27500"
                    disabled={!csrfToken || isSubmitting}
                  />
                  <button
                    className="icon-button"
                    type="submit"
                    disabled={!csrfToken || isSubmitting}
                    aria-label="Make a bid"
                    title="Make a bid"
                  >
                    <Send aria-hidden="true" />
                  </button>
                </div>
                {!csrfToken && (
                  <p className="form-note">Sign in as a buyer to place a bid.</p>
                )}
                {message && (
                  <p className={`form-message form-message--${message.tone}`} role="status">
                    {message.text}
                  </p>
                )}
              </form>
            )}
          </>
        )}

        <footer className="auction-card__footer">
          <span>
            {hasBids ? 'Highest bid' : 'Starting price'}{' '}
            <strong>{formatMoney(displayPrice)}</strong>
          </span>
          {(isHover || isSelected) && (
            <>
              <span>
                {topBid?.createdAt ? (
                  <>
                    Bid placed <strong>{formatDateTime(topBid.createdAt)}</strong>
                  </>
                ) : (
                  'No bids yet'
                )}
              </span>
              <span>
                {isEnded ? 'Ended' : 'Ends'}{' '}
                <strong>{formatDateTime(auction.endTime)}</strong>
              </span>
            </>
          )}
        </footer>
      </div>
    </article>
  );
}

function StatusTag({ state }: { state: Auction['state'] }) {
  return <span className={`status-tag status-tag--${state.toLowerCase()}`}>{state}</span>;
}

function humanizeBidError(message: string) {
  if (message.includes('not active')) {
    return 'This auction is not active yet. Please wait for activation.';
  }

  if (message.includes('expired')) {
    return 'This auction has already ended. New bids are closed.';
  }

  if (message.includes('current price')) {
    return 'Your bid must be equal to or higher than the current price.';
  }

  return message;
}

function getBidStatus({
  didCurrentUserWin,
  hasAnotherLeader,
  hasBids,
  isCurrentUserLeading,
  isEnded,
}: {
  didCurrentUserWin: boolean;
  hasAnotherLeader: boolean;
  hasBids: boolean;
  isCurrentUserLeading: boolean;
  isEnded: boolean;
}) {
  if (!hasBids) {
    return {
      description: isEnded
        ? 'No winner was assigned.'
        : 'Place the first bid to lead this auction.',
      title: isEnded ? 'Auction ended without bids' : 'No bids yet',
      tone: 'neutral',
    } as const;
  }

  if (didCurrentUserWin) {
    return {
      description: 'Your bid is the winning bid.',
      title: 'You won this auction',
      tone: 'leading',
    } as const;
  }

  if (isEnded) {
    return {
      description: 'This auction is closed.',
      title: 'Auction won by another buyer',
      tone: 'outbid',
    } as const;
  }

  if (isCurrentUserLeading) {
    return {
      description: 'If the auction ends now, this bid wins.',
      title: 'Your bid is currently highest',
      tone: 'leading',
    } as const;
  }

  return {
    description: hasAnotherLeader
      ? 'Raise your bid above the current amount to take the lead.'
      : 'Place a bid to compete for this auction.',
    title: hasAnotherLeader ? 'Another buyer is leading' : 'No bids yet',
    tone: hasAnotherLeader ? 'outbid' : 'neutral',
  } as const;
}
