import { UserRole } from '@prisma/client';

declare module 'express-session' {
  interface SessionData {
    pendingMfa?: {
      challengeId: string;
      userId: string;
    };
    csrfToken?: string;
    user?: {
      id: string;
      email: string;
      role: UserRole;
    };
  }
}
