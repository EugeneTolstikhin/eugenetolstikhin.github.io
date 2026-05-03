import { Transform } from 'class-transformer';
import { IsIn, IsISO8601, IsInt, IsOptional, IsString, Max, MaxLength, Min } from 'class-validator';
import { SortMode } from '../media.types';

function first(value: unknown): unknown {
  return Array.isArray(value) ? value[0] : value;
}

export class SearchQueryDto {
  @IsOptional()
  @IsString()
  @MaxLength(120)
  @Transform(({ value }) => String(first(value) ?? '').trim())
  q = '';

  @IsOptional()
  @IsString()
  @MaxLength(120)
  @Transform(({ value }) => {
    const normalized = String(first(value) ?? '').trim();
    return normalized || undefined;
  })
  credit?: string;

  @IsOptional()
  @IsISO8601({ strict: true })
  @Transform(({ value }) => {
    const normalized = String(first(value) ?? '').trim();
    return normalized || undefined;
  })
  dateFrom?: string;

  @IsOptional()
  @IsISO8601({ strict: true })
  @Transform(({ value }) => {
    const normalized = String(first(value) ?? '').trim();
    return normalized || undefined;
  })
  dateTo?: string;

  @IsOptional()
  @IsString()
  @MaxLength(400)
  @Transform(({ value }) => String(first(value) ?? '').trim())
  restrictions = '';

  @IsOptional()
  @IsIn(['relevance', 'date_asc', 'date_desc'])
  @Transform(({ value }) => String(first(value) ?? 'relevance'))
  sort: SortMode = 'relevance';

  @IsOptional()
  @IsInt()
  @Min(1)
  @Transform(({ value }) => Number(first(value) ?? 1))
  page = 1;

  @IsOptional()
  @IsInt()
  @Min(1)
  @Max(50)
  @Transform(({ value }) => Number(first(value) ?? 12))
  pageSize = 12;
}
