export type SortMode = 'relevance' | 'date_asc' | 'date_desc';

export interface RawMediaItem {
  suchtext: string;
  bildnummer: string;
  fotografen: string;
  datum: string;
  hoehe: string;
  breite: string;
}

export interface MediaItem extends RawMediaItem {
  id: number;
  dateIso: string | null;
  restrictions: string[];
  searchableText: string;
  tokens: string[];
}

export interface SearchQuery {
  q: string;
  credit?: string;
  dateFrom?: string;
  dateTo?: string;
  restrictions: string[];
  sort: SortMode;
  page: number;
  pageSize: number;
}

export interface SearchResultItem {
  id: number;
  suchtext: string;
  bildnummer: string;
  fotografen: string;
  datum: string;
  dateIso: string | null;
  hoehe: string;
  breite: string;
  restrictions: string[];
  score: number;
  snippet: string;
}

export interface SearchResponse {
  items: SearchResultItem[];
  page: number;
  pageSize: number;
  total: number;
  totalPages: number;
  timingMs: number;
}
