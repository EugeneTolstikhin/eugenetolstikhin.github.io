import { MiddlewareConsumer, Module, NestModule } from '@nestjs/common';
import { SearchController } from './search/search.controller';
import { AnalyticsService } from './search/analytics.service';
import { DatabaseService } from './search/database.service';
import { SearchIndexService } from './search/search-index.service';
import { SearchService } from './search/search.service';
import { RateLimitMiddleware } from './security/rate-limit.middleware';
import { PrismaService } from './prisma/prisma.service';

@Module({
  controllers: [SearchController],
  providers: [AnalyticsService, DatabaseService, SearchIndexService, SearchService, RateLimitMiddleware, PrismaService],
})
export class AppModule implements NestModule {
  configure(consumer: MiddlewareConsumer): void {
    consumer.apply(RateLimitMiddleware).forRoutes(SearchController);
  }
}
