import { FacetsResponse, SearchResponse, SortMode } from './types';

const API_BASE_URL = process.env.NEXT_PUBLIC_API_BASE_URL ?? 'http://localhost:3001';

export interface SearchParams {
  q: string;
  credit: string;
  dateFrom: string;
  dateTo: string;
  restrictions: string[];
  sort: SortMode;
  page: number;
  pageSize: number;
}

export async function searchMedia(params: SearchParams, signal?: AbortSignal): Promise<SearchResponse> {
  const query = new URLSearchParams({
    q: params.q,
    sort: params.sort,
    page: String(params.page),
    pageSize: String(params.pageSize),
  });
  if (params.credit) query.set('credit', params.credit);
  if (params.dateFrom) query.set('dateFrom', params.dateFrom);
  if (params.dateTo) query.set('dateTo', params.dateTo);
  if (params.restrictions.length > 0) query.set('restrictions', params.restrictions.join(','));

  const response = await fetch(`${API_BASE_URL}/api/search?${query.toString()}`, { signal });
  if (!response.ok) throw new Error(`Search failed with ${response.status}`);
  return response.json() as Promise<SearchResponse>;
}

export async function fetchFacets(signal?: AbortSignal): Promise<FacetsResponse> {
  const response = await fetch(`${API_BASE_URL}/api/facets`, { signal });
  if (!response.ok) throw new Error(`Facets failed with ${response.status}`);
  return response.json() as Promise<FacetsResponse>;
}
