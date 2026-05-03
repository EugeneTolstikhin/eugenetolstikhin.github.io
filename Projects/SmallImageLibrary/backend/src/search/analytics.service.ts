import { Injectable, Logger, OnModuleDestroy } from '@nestjs/common';
import Redis from 'ioredis';
import { SearchResponse } from './media.types';
import { tokenize } from './preprocess';

@Injectable()
export class AnalyticsService implements OnModuleDestroy {
  private readonly logger = new Logger(AnalyticsService.name);
  private readonly redis = new Redis(process.env.REDIS_URL ?? 'redis://localhost:6379', {
    lazyConnect: true,
    maxRetriesPerRequest: 1,
  });

  async onModuleDestroy(): Promise<void> {
    this.redis.disconnect();
  }

  async getCached(cacheKey: string): Promise<SearchResponse | null> {
    try {
      await this.connect();
      const cached = await this.redis.get(cacheKey);
      return cached ? (JSON.parse(cached) as SearchResponse) : null;
    } catch (error) {
      this.logger.warn(`Redis cache read skipped: ${(error as Error).message}`);
      return null;
    }
  }

  async setCached(cacheKey: string, response: SearchResponse): Promise<void> {
    try {
      await this.connect();
      const ttl = Number(process.env.SEARCH_CACHE_TTL_SECONDS ?? 30);
      await this.redis.set(cacheKey, JSON.stringify(response), 'EX', ttl);
    } catch (error) {
      this.logger.warn(`Redis cache write skipped: ${(error as Error).message}`);
    }
  }

  async recordSearch(query: string, timingMs: number): Promise<void> {
    try {
      await this.connect();
      const pipeline = this.redis.pipeline();
      pipeline.incr('analytics:search_count');
      pipeline.lpush('analytics:timings_ms', String(Math.round(timingMs)));
      pipeline.ltrim('analytics:timings_ms', 0, 199);
      for (const token of tokenize(query)) {
        pipeline.zincrby('analytics:keywords', 1, token);
      }
      await pipeline.exec();
    } catch (error) {
      this.logger.warn(`Redis analytics write skipped: ${(error as Error).message}`);
    }
  }

  async snapshot(): Promise<{ searches: number; timingsMs: number[]; topKeywords: Array<{ keyword: string; count: number }> }> {
    try {
      await this.connect();
      const [count, timings, keywords] = await Promise.all([
        this.redis.get('analytics:search_count'),
        this.redis.lrange('analytics:timings_ms', 0, 49),
        this.redis.zrevrange('analytics:keywords', 0, 9, 'WITHSCORES'),
      ]);
      const topKeywords: Array<{ keyword: string; count: number }> = [];
      for (let index = 0; index < keywords.length; index += 2) {
        topKeywords.push({ keyword: keywords[index], count: Number(keywords[index + 1]) });
      }
      return {
        searches: Number(count ?? 0),
        timingsMs: timings.map(Number),
        topKeywords,
      };
    } catch {
      return { searches: 0, timingsMs: [], topKeywords: [] };
    }
  }

  private async connect(): Promise<void> {
    if (this.redis.status === 'wait') {
      await this.redis.connect();
    }
  }
}
