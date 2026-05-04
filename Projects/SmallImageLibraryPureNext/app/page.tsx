'use client';

import { useEffect, useMemo, useState } from 'react';
import { searchMediaClient, fetchFacets } from '../src/lib/client-api';
import { highlightSnippet } from '../src/lib/highlight';
import { FacetsResponse, SearchResponse, SortMode } from '../src/lib/types';
import { useDebouncedValue } from '../src/lib/useDebouncedValue';

const initialResults: SearchResponse = {
  items: [],
  page: 1,
  pageSize: 12,
  total: 0,
  totalPages: 1,
  timingMs: 0,
};

export default function Home() {
  const [query, setQuery] = useState('Michael Jackson');
  const [credit, setCredit] = useState('');
  const [dateFrom, setDateFrom] = useState('');
  const [dateTo, setDateTo] = useState('');
  const [sort, setSort] = useState<SortMode>('relevance');
  const [page, setPage] = useState(1);
  const [selectedRestrictions, setSelectedRestrictions] = useState<string[]>([]);
  const [facets, setFacets] = useState<FacetsResponse>({ credits: [], restrictions: [] });
  const [results, setResults] = useState<SearchResponse>(initialResults);
  const [status, setStatus] = useState<'loading' | 'ready' | 'error'>('loading');
  const debouncedQuery = useDebouncedValue(query, 300);

  useEffect(() => {
    const controller = new AbortController();
    fetchFacets(controller.signal)
      .then(setFacets)
      .catch(() => undefined);
    return () => controller.abort();
  }, []);

  useEffect(() => {
    const controller = new AbortController();
    setStatus('loading');
    searchMediaClient(
      {
        q: debouncedQuery,
        credit,
        dateFrom,
        dateTo,
        restrictions: selectedRestrictions,
        sort,
        page,
        pageSize: 12,
      },
      controller.signal,
    )
      .then((response) => {
        setResults(response);
        setStatus('ready');
      })
      .catch((error: Error) => {
        if (error.name !== 'AbortError') setStatus('error');
      });
    return () => controller.abort();
  }, [credit, dateFrom, dateTo, debouncedQuery, page, selectedRestrictions, sort]);

  const summary = useMemo(() => {
    if (status === 'loading') return 'Loading results';
    if (status === 'error') return 'Search service unavailable';
    return `${results.total.toLocaleString()} matches in ${results.timingMs} ms`;
  }, [results.timingMs, results.total, status]);

  function resetPage(callback: () => void) {
    setPage(1);
    callback();
  }

  function toggleRestriction(restriction: string) {
    resetPage(() =>
      setSelectedRestrictions((current) =>
        current.includes(restriction)
          ? current.filter((item) => item !== restriction)
          : [...current, restriction],
      ),
    );
  }

  return (
    <main className="min-h-screen bg-panel">
      <div className="mx-auto flex w-full max-w-7xl flex-col gap-5 px-4 py-5 sm:px-6 lg:px-8">
        <header className="flex flex-col gap-2 border-b border-line pb-4 sm:flex-row sm:items-end sm:justify-between">
          <div>
            <h1 className="text-3xl font-semibold text-ink">Small Image Library Pure Next</h1>
            <p className="mt-1 max-w-3xl text-sm text-slate-600">
              Minimal Next.js implementation with App Router API routes, in-memory indexing, and Tailwind UI.
            </p>
          </div>
          <p className="text-sm font-medium text-signal" aria-live="polite">
            {summary}
          </p>
        </header>

        <section className="grid gap-4 rounded border border-line bg-white p-4 shadow-sm lg:grid-cols-[minmax(0,1fr)_220px_180px]">
          <label className="flex flex-col gap-1 text-sm font-medium text-ink">
            Search
            <input
              className="h-11 rounded border border-line px-3 outline-none focus:border-signal focus:ring-2 focus:ring-signal/20"
              value={query}
              onChange={(event) => resetPage(() => setQuery(event.target.value))}
              placeholder="Search people, places, credits, image numbers"
            />
          </label>

          <label className="flex flex-col gap-1 text-sm font-medium text-ink">
            Credit
            <select
              className="h-11 rounded border border-line px-3 outline-none focus:border-signal focus:ring-2 focus:ring-signal/20"
              value={credit}
              onChange={(event) => resetPage(() => setCredit(event.target.value))}
            >
              <option value="">All credits</option>
              {facets.credits.map((item) => (
                <option key={item} value={item}>
                  {item}
                </option>
              ))}
            </select>
          </label>

          <label className="flex flex-col gap-1 text-sm font-medium text-ink">
            Sort
            <select
              className="h-11 rounded border border-line px-3 outline-none focus:border-signal focus:ring-2 focus:ring-signal/20"
              value={sort}
              onChange={(event) => resetPage(() => setSort(event.target.value as SortMode))}
            >
              <option value="relevance">Relevance</option>
              <option value="date_desc">Newest first</option>
              <option value="date_asc">Oldest first</option>
            </select>
          </label>

          <label className="flex flex-col gap-1 text-sm font-medium text-ink">
            From
            <input
              className="h-11 rounded border border-line px-3 outline-none focus:border-signal focus:ring-2 focus:ring-signal/20"
              type="date"
              value={dateFrom}
              onChange={(event) => resetPage(() => setDateFrom(event.target.value))}
            />
          </label>

          <label className="flex flex-col gap-1 text-sm font-medium text-ink">
            To
            <input
              className="h-11 rounded border border-line px-3 outline-none focus:border-signal focus:ring-2 focus:ring-signal/20"
              type="date"
              value={dateTo}
              onChange={(event) => resetPage(() => setDateTo(event.target.value))}
            />
          </label>

          <button
            className="h-11 self-end rounded border border-line bg-white px-4 text-sm font-semibold text-ink hover:border-plum focus:outline-none focus:ring-2 focus:ring-plum/30"
            onClick={() => {
              setQuery('');
              setCredit('');
              setDateFrom('');
              setDateTo('');
              setSort('relevance');
              setSelectedRestrictions([]);
              setPage(1);
            }}
          >
            Reset
          </button>
        </section>

        <section aria-label="Restriction filters" className="flex flex-wrap gap-2">
          {facets.restrictions.map((restriction) => {
            const selected = selectedRestrictions.includes(restriction);
            return (
              <button
                key={restriction}
                className={`rounded border px-3 py-2 text-xs font-semibold focus:outline-none focus:ring-2 focus:ring-signal/30 ${
                  selected ? 'border-signal bg-signal text-white' : 'border-line bg-white text-slate-700 hover:border-signal'
                }`}
                onClick={() => toggleRestriction(restriction)}
                aria-pressed={selected}
              >
                {restriction}
              </button>
            );
          })}
        </section>

        {status === 'error' && (
          <section className="rounded border border-plum bg-white p-5 text-plum" role="alert">
            Search failed. Confirm the Next.js Docker service is running on port 3010.
          </section>
        )}

        {status === 'ready' && results.items.length === 0 && (
          <section className="rounded border border-line bg-white p-8 text-center text-slate-600">
            No media matched the current query and filters.
          </section>
        )}

        <section className="grid gap-3 md:grid-cols-2 xl:grid-cols-3" aria-busy={status === 'loading'}>
          {results.items.map((item) => (
            <article key={item.id} className="grid min-h-64 grid-rows-[120px_auto] rounded border border-line bg-white shadow-sm">
              <div className="flex items-center justify-center bg-slate-200 text-sm font-semibold text-slate-600">
                {item.breite} x {item.hoehe}
              </div>
              <div className="flex flex-col gap-3 p-4">
                <div className="flex items-start justify-between gap-3">
                  <div>
                    <h2 className="text-base font-semibold text-ink">{item.bildnummer}</h2>
                    <p className="mt-1 text-sm text-slate-600">{item.fotografen}</p>
                  </div>
                  <span className="shrink-0 rounded bg-panel px-2 py-1 text-xs font-semibold text-slate-700">{item.datum}</span>
                </div>
                <p className="text-sm leading-6 text-slate-700">{highlightSnippet(item.snippet, query)}</p>
                <div className="flex flex-wrap gap-2">
                  {item.restrictions.map((restriction) => (
                    <span key={restriction} className="rounded bg-panel px-2 py-1 text-xs text-slate-600">
                      {restriction}
                    </span>
                  ))}
                </div>
              </div>
            </article>
          ))}
        </section>

        <nav className="flex items-center justify-between border-t border-line pt-4" aria-label="Pagination">
          <button
            className="rounded border border-line bg-white px-4 py-2 text-sm font-semibold disabled:cursor-not-allowed disabled:opacity-50"
            disabled={results.page <= 1 || status === 'loading'}
            onClick={() => setPage((current) => Math.max(1, current - 1))}
          >
            Previous
          </button>
          <span className="text-sm text-slate-600">
            Page {results.page} of {results.totalPages}
          </span>
          <button
            className="rounded border border-line bg-white px-4 py-2 text-sm font-semibold disabled:cursor-not-allowed disabled:opacity-50"
            disabled={results.page >= results.totalPages || status === 'loading'}
            onClick={() => setPage((current) => current + 1)}
          >
            Next
          </button>
        </nav>
      </div>
    </main>
  );
}
