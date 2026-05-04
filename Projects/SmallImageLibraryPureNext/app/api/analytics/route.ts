import { NextResponse } from 'next/server';
import { getAnalytics } from '../../../src/lib/search-store';

export const dynamic = 'force-dynamic';

export function GET() {
  return NextResponse.json(getAnalytics());
}
