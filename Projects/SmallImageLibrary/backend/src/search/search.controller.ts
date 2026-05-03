import { Controller, Get, Query } from '@nestjs/common';
import { SearchQueryDto } from './dto/search-query.dto';
import { SearchService } from './search.service';

@Controller('api')
export class SearchController {
  constructor(private readonly searchService: SearchService) {}

  @Get('search')
  search(@Query() query: SearchQueryDto) {
    return this.searchService.search(this.searchService.fromDto(query));
  }

  @Get('facets')
  facets() {
    return this.searchService.facets();
  }

  @Get('analytics')
  analytics() {
    return this.searchService.analyticsSnapshot();
  }
}
