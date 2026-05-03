import { plainToInstance } from 'class-transformer';
import { validate } from 'class-validator';
import { SearchQueryDto } from './search-query.dto';

describe('SearchQueryDto', () => {
  it('accepts a normal search query and transforms numeric fields', async () => {
    const dto = plainToInstance(SearchQueryDto, {
      q: 'Michael',
      sort: 'date_desc',
      page: '2',
      pageSize: '10',
    });

    const errors = await validate(dto);
    expect(errors).toHaveLength(0);
    expect(dto.page).toBe(2);
    expect(dto.pageSize).toBe(10);
  });

  it('rejects oversized query strings and invalid sort values', async () => {
    const dto = plainToInstance(SearchQueryDto, {
      q: '<script>alert(1)</script>'.repeat(10),
      sort: 'drop_table',
    });

    const errors = await validate(dto);
    expect(errors.map((error) => error.property)).toEqual(expect.arrayContaining(['q', 'sort']));
  });

  it('rejects malformed dates and page sizes outside the allowed range', async () => {
    const dto = plainToInstance(SearchQueryDto, {
      dateFrom: 'not-a-date',
      pageSize: '500',
    });

    const errors = await validate(dto);
    expect(errors.map((error) => error.property)).toEqual(expect.arrayContaining(['dateFrom', 'pageSize']));
  });
});
