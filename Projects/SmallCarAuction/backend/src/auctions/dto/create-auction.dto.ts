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
  @IsString()
  @MaxLength(80)
  brand: string;

  @IsString()
  @MaxLength(80)
  model: string;

  @IsString()
  @Length(17, 17)
  vin: string;

  @Type(() => Number)
  @IsNumber({ maxDecimalPlaces: 2 })
  @IsPositive()
  startingPrice: number;

  @Type(() => Date)
  @IsDate()
  endTime: Date;
}
