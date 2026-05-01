import { Controller, Get } from '@nestjs/common';

@Controller()
export class AppController {
  @Get()
  root() {
    return {
      name: 'car-auction-backend',
      status: 'ok',
      routes: {
        login: 'POST /auth/login',
        verifyOtp: 'POST /auth/verify-otp',
        logout: 'POST /auth/logout',
        auctions: 'GET /auctions',
      },
    };
  }
}
