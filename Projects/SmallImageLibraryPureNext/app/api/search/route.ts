import { NextRequest, NextResponse } from 'next/server';
import { parseSearchParams, searchMedia } from '../../../src/lib/search-store';

export const dynamic = 'force-dynamic';

export function GET(request: NextRequest) {
  const query = parseSearchParams(request.nextUrl.searchParams);
  return NextResponse.json(searchMedia(query));
}
