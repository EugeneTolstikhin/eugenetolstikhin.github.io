import { ValidationPipe } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { NestFactory } from '@nestjs/core';
import RedisStore from 'connect-redis';
import { json, urlencoded } from 'express';
import session from 'express-session';
import helmet from 'helmet';
import Redis from 'ioredis';
import { AppModule } from './app.module';
import { createRateLimitMiddleware } from './security/rate-limit.middleware';

async function bootstrap() {
  const app = await NestFactory.create(AppModule, { bodyParser: false });
  const config = app.get(ConfigService);
  const redis = new Redis(config.getOrThrow<string>('REDIS_URL'));
  const isProduction = config.get<string>('NODE_ENV') === 'production';
  const sessionCookieName = config.get<string>(
    'SESSION_COOKIE_NAME',
    isProduction ? '__Host-small-car-auction.sid' : 'small-car-auction.sid',
  );
  const expressApp = app.getHttpAdapter().getInstance();

  if (config.get<string>('TRUST_PROXY') === 'true') {
    expressApp.set('trust proxy', 1);
  }

  expressApp.set('json escape', true);
  expressApp.disable('x-powered-by');
  app.use(json({ limit: config.get<string>('REQUEST_BODY_LIMIT', '100kb') }));
  app.use(
    urlencoded({
      extended: false,
      limit: config.get<string>('REQUEST_BODY_LIMIT', '100kb'),
    }),
  );
  app.use(
    helmet({
      contentSecurityPolicy: {
        directives: {
          defaultSrc: ["'none'"],
          frameAncestors: ["'none'"],
          formAction: ["'none'"],
          baseUri: ["'none'"],
        },
      },
      crossOriginEmbedderPolicy: false,
      frameguard: { action: 'deny' },
      referrerPolicy: { policy: 'no-referrer' },
    }),
  );
  app.enableCors({
    credentials: true,
    origin: buildCorsOriginValidator(config),
    methods: ['GET', 'POST', 'OPTIONS'],
    allowedHeaders: ['Content-Type', 'X-CSRF-Token'],
    maxAge: 600,
  });
  app.use(
    createRateLimitMiddleware({
      redis,
      windowSeconds: parsePositiveInt(config.get<string>('RATE_LIMIT_WINDOW_SECONDS'), 60),
      maxRequests: parsePositiveInt(config.get<string>('RATE_LIMIT_MAX_REQUESTS'), 300),
      authWindowSeconds: parsePositiveInt(
        config.get<string>('AUTH_RATE_LIMIT_WINDOW_SECONDS'),
        300,
      ),
      authMaxRequests: parsePositiveInt(
        config.get<string>('AUTH_RATE_LIMIT_MAX_REQUESTS'),
        10,
      ),
    }),
  );

  app.use(
    session({
      name: sessionCookieName,
      store: new RedisStore({ client: redis }),
      secret: config.getOrThrow<string>('SESSION_SECRET'),
      resave: false,
      saveUninitialized: false,
      proxy: config.get<string>('TRUST_PROXY') === 'true',
      cookie: {
        httpOnly: true,
        secure: isProduction,
        sameSite: 'strict',
        path: '/',
        maxAge:
          parsePositiveInt(config.get<string>('SESSION_MAX_AGE_SECONDS'), 3600) *
          1000,
      },
    }),
  );

  app.useGlobalPipes(
    new ValidationPipe({
      whitelist: true,
      forbidNonWhitelisted: true,
      transform: true,
    }),
  );

  await app.listen(config.get<number>('PORT', 3000));
}

void bootstrap();

function buildCorsOriginValidator(config: ConfigService) {
  const allowedOrigins = config
    .get<string>('CORS_ORIGINS', 'http://localhost:3000,http://localhost:5173')
    .split(',')
    .map((origin) => origin.trim())
    .filter(Boolean);

  return (origin: string | undefined, callback: (error: Error | null, allow?: boolean) => void) => {
    if (!origin || allowedOrigins.includes(origin)) {
      callback(null, true);
      return;
    }

    callback(null, false);
  };
}

function parsePositiveInt(value: string | undefined, fallback: number) {
  const parsed = Number.parseInt(value ?? '', 10);

  return Number.isInteger(parsed) && parsed > 0 ? parsed : fallback;
}
