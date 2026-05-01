'use client';

import { KeyRound, LogIn, LogOut, MailCheck, ShieldCheck } from 'lucide-react';
import { FormEvent, useState } from 'react';
import { login, verifyOtp } from './api';
import type { SessionUser } from './types';

type Props = {
  user?: SessionUser;
  onAuthenticated: (user: SessionUser) => void;
  onLogout: () => Promise<void> | void;
};

export function AuthPanel({ user, onAuthenticated, onLogout }: Props) {
  const [email, setEmail] = useState('buyer@example.com');
  const [password, setPassword] = useState('BuyerPass123!');
  const [otp, setOtp] = useState('');
  const [phase, setPhase] = useState<'credentials' | 'otp'>('credentials');
  const [message, setMessage] = useState('');
  const [isBusy, setIsBusy] = useState(false);

  async function submitCredentials(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setIsBusy(true);
    setMessage('');

    try {
      const response = await login(email, password);
      setPhase('otp');
      setMessage(`OTP sent to ${response.email}. Open Mailpit to copy the code.`);
    } catch (error) {
      setMessage(error instanceof Error ? error.message : 'Login failed.');
    } finally {
      setIsBusy(false);
    }
  }

  async function submitOtp(event: FormEvent<HTMLFormElement>) {
    event.preventDefault();
    setIsBusy(true);
    setMessage('');

    try {
      onAuthenticated(await verifyOtp(otp));
      setOtp('');
      setMessage('Session ready.');
    } catch (error) {
      setMessage(error instanceof Error ? error.message : 'OTP verification failed.');
    } finally {
      setIsBusy(false);
    }
  }

  async function submitLogout() {
    setIsBusy(true);
    setMessage('');

    try {
      await onLogout();
      setPhase('credentials');
      setOtp('');
    } catch (error) {
      setMessage(error instanceof Error ? error.message : 'Logout failed.');
    } finally {
      setIsBusy(false);
    }
  }

  if (user) {
    return (
      <section className="auth-panel auth-panel--ready" aria-label="Current session">
        <ShieldCheck aria-hidden="true" />
        <div>
          <strong>{user.email}</strong>
          <span>{user.role} session active</span>
        </div>
        <button
          className="icon-text-button auth-panel__logout"
          type="button"
          onClick={submitLogout}
          disabled={isBusy}
        >
          <LogOut aria-hidden="true" />
          Logout
        </button>
      </section>
    );
  }

  return (
    <section className="auth-panel" aria-label="Sign in">
      <div className="auth-panel__heading">
        {phase === 'credentials' ? <LogIn aria-hidden="true" /> : <MailCheck aria-hidden="true" />}
        <div>
          <h2>{phase === 'credentials' ? 'Sign in' : 'Verify OTP'}</h2>
          <p>Use the seeded buyer account to load and bid on auctions.</p>
        </div>
      </div>

      {phase === 'credentials' ? (
        <form onSubmit={submitCredentials} className="auth-form">
          <label>
            Email
            <input value={email} onChange={(event) => setEmail(event.target.value)} />
          </label>
          <label>
            Password
            <input
              type="password"
              value={password}
              onChange={(event) => setPassword(event.target.value)}
            />
          </label>
          <button className="icon-text-button" type="submit" disabled={isBusy}>
            <KeyRound aria-hidden="true" />
            Continue
          </button>
        </form>
      ) : (
        <form onSubmit={submitOtp} className="auth-form">
          <label>
            OTP code
            <input
              value={otp}
              onChange={(event) => setOtp(event.target.value)}
              inputMode="numeric"
              maxLength={6}
              placeholder="123456"
            />
          </label>
          <button className="icon-text-button" type="submit" disabled={isBusy}>
            <ShieldCheck aria-hidden="true" />
            Verify
          </button>
        </form>
      )}

      {message && <p className="auth-panel__message">{message}</p>}
    </section>
  );
}
