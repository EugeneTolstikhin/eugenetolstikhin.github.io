import type { Metadata } from 'next';
import './globals.css';

export const metadata: Metadata = {
  title: 'Small Image Library Pure Next',
  description: 'Pure Next.js media metadata search challenge implementation',
};

export default function RootLayout({ children }: Readonly<{ children: React.ReactNode }>) {
  return (
    <html lang="en">
      <body>{children}</body>
    </html>
  );
}
