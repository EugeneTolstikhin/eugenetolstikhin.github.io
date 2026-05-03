import type { Config } from 'tailwindcss';

const config: Config = {
  content: ['./app/**/*.{ts,tsx}', './src/**/*.{ts,tsx}'],
  theme: {
    extend: {
      colors: {
        ink: '#18202a',
        panel: '#f7f8fa',
        line: '#d7dde5',
        signal: '#2f6f73',
        berry: '#8c2f4b',
      },
    },
  },
  plugins: [],
};

export default config;
