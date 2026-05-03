import { Injectable, Logger, OnModuleInit } from '@nestjs/common';
import { DatabaseService } from './database.service';
import { MediaItem } from './media.types';
import { tokenize } from './preprocess';

@Injectable()
export class SearchIndexService implements OnModuleInit {
  private readonly logger = new Logger(SearchIndexService.name);
  private readonly items = new Map<number, MediaItem>();
  private readonly index = new Map<string, Map<number, number>>();

  constructor(private readonly database: DatabaseService) {}

  async onModuleInit(): Promise<void> {
    const items = await this.database.findAllMedia();
    this.rebuild(items);
  }

  rebuild(items: MediaItem[]): void {
    this.items.clear();
    this.index.clear();

    for (const item of items) {
      this.items.set(item.id, item);
      this.addWeightedTokens(item.id, tokenize(item.suchtext), 5);
      this.addWeightedTokens(item.id, tokenize(item.fotografen), 2);
      this.addWeightedTokens(item.id, tokenize(item.bildnummer), 8);
    }

    this.logger.log(`Built in-memory search index for ${items.length} media rows.`);
  }

  allItems(): MediaItem[] {
    return Array.from(this.items.values());
  }

  score(query: string): Map<number, number> {
    const queryTokens = tokenize(query);
    const scores = new Map<number, number>();
    if (queryTokens.length === 0) {
      for (const item of this.items.values()) scores.set(item.id, 0);
      return scores;
    }

    for (const token of queryTokens) {
      for (const [indexedToken, postings] of this.index.entries()) {
        if (indexedToken === token || indexedToken.startsWith(token)) {
          for (const [id, score] of postings.entries()) {
            scores.set(id, (scores.get(id) ?? 0) + score);
          }
        }
      }
    }

    return scores;
  }

  private addWeightedTokens(id: number, tokens: string[], weight: number): void {
    for (const token of tokens) {
      const postings = this.index.get(token) ?? new Map<number, number>();
      postings.set(id, (postings.get(id) ?? 0) + weight);
      this.index.set(token, postings);
    }
  }
}
