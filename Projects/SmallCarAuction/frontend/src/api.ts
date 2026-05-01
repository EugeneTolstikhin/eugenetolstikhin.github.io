import type { Auction, BidResponse, SessionUser } from './types';

const API_BASE_URL = process.env.NEXT_PUBLIC_API_BASE_URL ?? 'http://localhost:3000';

type LoginResponse = {
  mfaRequired: true;
  delivery: 'email';
  email: string;
  expiresInSeconds: number;
};

export async function login(email: string, password: string) {
  return request<LoginResponse>('/auth/login', {
    method: 'POST',
    body: { email, password },
  });
}

export async function verifyOtp(code: string) {
  return request<SessionUser>('/auth/verify-otp', {
    method: 'POST',
    body: { code },
  });
}

export async function logout(csrfToken: string) {
  return request<{ ok: true }>('/auth/logout', {
    method: 'POST',
    csrfToken,
  });
}

export async function listAuctions() {
  return request<Auction[]>('/auctions');
}

export async function placeBid(auctionId: string, amount: number, csrfToken: string) {
  return request<BidResponse>(`/auctions/${auctionId}/bids`, {
    method: 'POST',
    body: { amount },
    csrfToken,
  });
}

async function request<T>(
  path: string,
  options: {
    method?: 'GET' | 'POST';
    body?: unknown;
    csrfToken?: string;
  } = {},
) {
  const headers = new Headers();

  if (options.body) {
    headers.set('Content-Type', 'application/json');
  }

  if (options.csrfToken) {
    headers.set('X-CSRF-Token', options.csrfToken);
  }

  const response = await fetch(`${API_BASE_URL}${path}`, {
    method: options.method ?? 'GET',
    credentials: 'include',
    headers,
    body: options.body ? JSON.stringify(options.body) : undefined,
  });

  const payload = await readPayload(response);

  if (!response.ok) {
    throw new Error(extractErrorMessage(payload, response.status));
  }

  return payload as T;
}

async function readPayload(response: Response) {
  const text = await response.text();

  if (!text) {
    return {};
  }

  try {
    return JSON.parse(text) as unknown;
  } catch {
    return text;
  }
}

function extractErrorMessage(payload: unknown, status: number) {
  if (
    payload &&
    typeof payload === 'object' &&
    'message' in payload &&
    typeof payload.message === 'string'
  ) {
    return payload.message;
  }

  return `Request failed with status ${status}`;
}
