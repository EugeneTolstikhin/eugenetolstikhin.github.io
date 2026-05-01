import { Module } from '@nestjs/common';
import { ConfigModule } from '@nestjs/config';
import { AuctionsModule } from './auctions/auctions.module';
import { AuthModule } from './auth/auth.module';
import { AppController } from './app.controller';
import { PrismaModule } from './prisma/prisma.module';

@Module({
  imports: [
    ConfigModule.forRoot({ isGlobal: true }),
    PrismaModule,
    AuthModule,
    AuctionsModule,
  ],
  controllers: [AppController],
})
export class AppModule {}
