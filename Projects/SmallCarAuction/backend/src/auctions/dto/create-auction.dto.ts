import { ApiProperty } from '@nestjs/swagger';
import { Type } from 'class-transformer';
import {
  IsDate,
  IsNumber,
  IsPositive,
  IsString,
  Length,
  MaxLength,
} from 'class-validator';

export class CreateAuctionDto {
  @ApiProperty({ example: 'Toyota', maxLength: 80 })
  @IsString()
  @MaxLength(80)
  brand: string;

  @ApiProperty({ example: 'Supra', maxLength: 80 })
  @IsString()
  @MaxLength(80)
  model: string;

  @ApiProperty({ example: 'JT2MA70L0H0123456', minLength: 17, maxLength: 17 })
  @IsString()
  @Length(17, 17)
  vin: string;

  @ApiProperty({ example: 25000, minimum: 0.01 })
  @Type(() => Number)
  @IsNumber({ maxDecimalPlaces: 2 })
  @IsPositive()
  startingPrice: number;

  @ApiProperty({ example: '2026-12-31T18:00:00.000Z', type: String, format: 'date-time' })
  @Type(() => Date)
  @IsDate()
  endTime: Date;
}
