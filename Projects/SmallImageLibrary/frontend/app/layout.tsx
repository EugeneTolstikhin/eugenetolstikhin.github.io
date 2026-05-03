import type { Metadata } from 'next';
import './globals.css';

export const metadata: Metadata = {
  title: 'Small Image Library',
  description: 'Dockerized media metadata search demo',
};

export default function RootLayout({ children }: Readonly<{ children: React.ReactNode }>) {
  return (
    <html lang="en">
      <body>{children}</body>
    </html>
  );
}
