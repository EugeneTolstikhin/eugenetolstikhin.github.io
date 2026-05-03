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

export interface FacetsResponse {
  credits: string[];
  restrictions: string[];
}

export type SortMode = 'relevance' | 'date_desc' | 'date_asc';
