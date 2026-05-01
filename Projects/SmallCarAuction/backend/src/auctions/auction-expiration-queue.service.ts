import { Injectable, OnModuleDestroy } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { Job, Queue, Worker } from 'bullmq';

type ExpireAuctionJob = {
  auctionId: string;
};

type ExpirationProcessor = (auctionId: string) => Promise<void>;

@Injectable()
export class AuctionExpirationQueueService implements OnModuleDestroy {
  private readonly queue: Queue<ExpireAuctionJob>;
  private readonly worker: Worker<ExpireAuctionJob>;
  private processor?: ExpirationProcessor;

  constructor(config: ConfigService) {
    const connection = parseRedisUrl(config.getOrThrow<string>('REDIS_URL'));

    this.queue = new Queue<ExpireAuctionJob>('auction-expiration', {
      connection,
    });
    this.worker = new Worker<ExpireAuctionJob>(
      'auction-expiration',
      (job) => this.process(job),
      { connection },
    );
  }

  registerProcessor(processor: ExpirationProcessor) {
    this.processor = processor;
  }

  async scheduleExpiration(auctionId: string, endTime: Date) {
    const delay = Math.max(0, endTime.getTime() - Date.now());

    await this.queue.add(
      'expire-auction',
      { auctionId },
      {
        delay,
        jobId: `auction-expiration-${auctionId}`,
        removeOnComplete: true,
        removeOnFail: 100,
      },
    );
  }

  async onModuleDestroy() {
    await this.worker.close();
    await this.queue.close();
  }

  private async process(job: Job<ExpireAuctionJob>) {
    if (!this.processor) {
      throw new Error('Auction expiration processor is not registered');
    }

    await this.processor(job.data.auctionId);
  }
}

function parseRedisUrl(redisUrl: string) {
  const url = new URL(redisUrl);

  return {
    host: url.hostname,
    port: Number(url.port || 6379),
    username: url.username || undefined,
    password: url.password || undefined,
    db: url.pathname ? Number(url.pathname.slice(1) || 0) : 0,
  };
}
