import React from 'react';

export function highlightSnippet(snippet: string, query: string): React.ReactNode {
  const token = query.trim().split(/\s+/).filter(Boolean)[0];
  if (!token) return snippet;
  const index = snippet.toLowerCase().indexOf(token.toLowerCase());
  if (index < 0) return snippet;

  return (
    <>
      {snippet.slice(0, index)}
      <mark>{snippet.slice(index, index + token.length)}</mark>
      {snippet.slice(index + token.length)}
    </>
  );
}
