import { Controller, Get } from '@nestjs/common';
import { ApiOkResponse, ApiOperation, ApiTags } from '@nestjs/swagger';

@Controller()
@ApiTags('health')
export class AppController {
  @Get()
  @ApiOperation({ summary: 'Basic service status and route discovery' })
  @ApiOkResponse({
    schema: {
      example: {
        name: 'car-auction-backend',
        status: 'ok',
        routes: {
          login: 'POST /auth/login',
          verifyOtp: 'POST /auth/verify-otp',
          logout: 'POST /auth/logout',
          auctions: 'GET /auctions',
        },
      },
    },
  })
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
