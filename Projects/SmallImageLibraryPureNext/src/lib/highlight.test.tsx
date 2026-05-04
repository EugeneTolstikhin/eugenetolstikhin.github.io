import React from 'react';
import { render, screen } from '@testing-library/react';
import { describe, expect, it } from 'vitest';
import { highlightSnippet } from './highlight';

describe('highlightSnippet', () => {
  it('marks the first matching token', () => {
    render(<p>{highlightSnippet('Michael Jackson on stage', 'jackson')}</p>);
    expect(screen.getByText('Jackson').tagName).toBe('MARK');
  });

  it('keeps HTML-like metadata inert', () => {
    render(<p>{highlightSnippet('<script>alert(1)</script> Michael', 'Michael')}</p>);
    expect(screen.getByText('<script>alert(1)</script>')).toBeInTheDocument();
    expect(document.querySelector('script')).toBeNull();
  });
});
