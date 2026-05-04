import type { Config } from 'tailwindcss';

const config: Config = {
  content: ['./app/**/*.{ts,tsx}', './src/**/*.{ts,tsx}'],
  theme: {
    extend: {
      colors: {
        ink: '#17212b',
        panel: '#f5f7fa',
        line: '#d9e0e8',
        signal: '#226b6f',
        plum: '#7d3151',
      },
    },
  },
  plugins: [],
};

export default config;
