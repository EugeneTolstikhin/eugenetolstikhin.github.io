import React from 'react';
import { render, screen } from '@testing-library/react';
import { describe, expect, it } from 'vitest';
import { highlightSnippet } from './highlight';

describe('highlightSnippet', () => {
  it('marks the first query token when it appears in the snippet', () => {
    render(<p>{highlightSnippet('Michael Jackson on stage', 'jackson music')}</p>);
    expect(screen.getByText('Jackson').tagName).toBe('MARK');
  });

  it('returns plain text when the query is empty', () => {
    render(<p>{highlightSnippet('Archive portrait', '')}</p>);
    expect(screen.getByText('Archive portrait')).toBeInTheDocument();
  });

  it('keeps HTML-like metadata inert instead of injecting raw markup', () => {
    render(<p>{highlightSnippet('<script>alert(1)</script> Michael', 'Michael')}</p>);
    expect(screen.getByText('<script>alert(1)</script>')).toBeInTheDocument();
    expect(document.querySelector('script')).toBeNull();
    expect(screen.getByText('Michael').tagName).toBe('MARK');
  });
});
