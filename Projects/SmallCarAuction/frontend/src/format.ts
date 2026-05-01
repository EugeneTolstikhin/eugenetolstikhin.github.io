export function formatMoney(value: string | number | null | undefined) {
  const amount = Number(value ?? 0);

  return new Intl.NumberFormat('en-US', {
    style: 'currency',
    currency: 'USD',
    maximumFractionDigits: 2,
  }).format(Number.isFinite(amount) ? amount : 0);
}

export function formatDateTime(value: string) {
  return new Intl.DateTimeFormat('en-US', {
    dateStyle: 'medium',
    timeStyle: 'short',
  }).format(new Date(value));
}

export function timeUntil(value: string) {
  const target = new Date(value).getTime();
  const diffMs = target - Date.now();
  const absMs = Math.abs(diffMs);
  const minutes = Math.round(absMs / 60_000);

  if (minutes < 1) {
    return diffMs >= 0 ? 'ends in less than a minute' : 'ended just now';
  }

  if (minutes < 60) {
    return diffMs >= 0 ? `ends in ${minutes} min` : `ended ${minutes} min ago`;
  }

  const hours = Math.round(minutes / 60);

  if (hours < 24) {
    return diffMs >= 0 ? `ends in ${hours} hr` : `ended ${hours} hr ago`;
  }

  const days = Math.round(hours / 24);
  return diffMs >= 0 ? `ends in ${days} days` : `ended ${days} days ago`;
}
