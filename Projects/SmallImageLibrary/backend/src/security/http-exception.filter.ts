import { ArgumentsHost, Catch, ExceptionFilter, HttpException, HttpStatus } from '@nestjs/common';
import { Request, Response } from 'express';

@Catch()
export class HttpExceptionFilter implements ExceptionFilter {
  catch(exception: unknown, host: ArgumentsHost): void {
    const context = host.switchToHttp();
    const response = context.getResponse<Response>();
    const request = context.getRequest<Request>();
    const status = exception instanceof HttpException ? exception.getStatus() : HttpStatus.INTERNAL_SERVER_ERROR;
    const body = exception instanceof HttpException ? exception.getResponse() : null;

    response.status(status).json({
      statusCode: status,
      error: this.errorName(status),
      message: this.message(body),
      path: request.url,
      timestamp: new Date().toISOString(),
    });
  }

  private message(body: unknown): string | string[] {
    if (typeof body === 'string') return body;
    if (body && typeof body === 'object' && 'message' in body) {
      const message = (body as { message: unknown }).message;
      if (Array.isArray(message)) return message.map(String);
      if (typeof message === 'string') return message;
    }
    return 'Unexpected server error';
  }

  private errorName(status: number): string {
    if (status >= 500) return 'Internal Server Error';
    if (status === 429) return 'Too Many Requests';
    if (status === 400) return 'Bad Request';
    return 'Request Error';
  }
}
