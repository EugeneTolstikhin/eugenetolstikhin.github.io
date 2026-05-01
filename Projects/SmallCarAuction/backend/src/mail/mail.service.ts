import { Injectable, Logger } from '@nestjs/common';
import { ConfigService } from '@nestjs/config';
import { Socket } from 'net';

type MailMessage = {
  to: string;
  subject: string;
  text: string;
};

@Injectable()
export class MailService {
  private readonly logger = new Logger(MailService.name);

  constructor(private readonly config: ConfigService) {}

  async send(message: MailMessage) {
    const host = this.config.get<string>('MAIL_SMTP_HOST', 'mailpit');
    const port = this.config.get<number>('MAIL_SMTP_PORT', 1025);
    const from = this.config.get<string>(
      'MAIL_FROM',
      'Small Car Auction <no-reply@small-car-auction.test>',
    );

    await this.sendSmtp({ host, port, from, ...message });
    this.logger.log(`Sent email to ${message.to}: ${message.subject}`);
  }

  private async sendSmtp({
    host,
    port,
    from,
    to,
    subject,
    text,
  }: MailMessage & { host: string; port: number; from: string }) {
    const socket = new Socket();

    await new Promise<void>((resolve, reject) => {
      socket.once('error', reject);
      socket.connect(port, host, resolve);
    });

    const session = new SmtpSession(socket);

    try {
      await session.expect(220);
      await session.command('EHLO small-car-auction.local', 250);
      await session.command(`MAIL FROM:<${this.extractAddress(from)}>`, 250);
      await session.command(`RCPT TO:<${to}>`, 250);
      await session.command('DATA', 354);
      await session.writeData(this.formatMessage({ from, to, subject, text }));
      await session.expect(250);
      await session.command('QUIT', 221);
    } finally {
      socket.end();
    }
  }

  private extractAddress(value: string) {
    const match = value.match(/<([^>]+)>/);
    return match?.[1] ?? value;
  }

  private formatMessage({ from, to, subject, text }: MailMessage & { from: string }) {
    const escapedText = text.replace(/^\./gm, '..');

    return [
      `From: ${from}`,
      `To: ${to}`,
      `Subject: ${subject}`,
      'Content-Type: text/plain; charset=utf-8',
      '',
      escapedText,
      '.',
      '',
    ].join('\r\n');
  }
}

class SmtpSession {
  private buffer = '';
  private waiters: Array<{
    code: number;
    resolve: () => void;
    reject: (error: Error) => void;
  }> = [];

  constructor(private readonly socket: Socket) {
    socket.on('data', (data) => {
      this.buffer += data.toString('utf8');
      this.flush();
    });
    socket.on('error', (error) => this.rejectAll(error));
  }

  command(command: string, expectedCode: number) {
    this.socket.write(`${command}\r\n`);
    return this.expect(expectedCode);
  }

  writeData(data: string) {
    this.socket.write(data);
  }

  expect(code: number) {
    return new Promise<void>((resolve, reject) => {
      this.waiters.push({ code, resolve, reject });
      this.flush();
    });
  }

  private flush() {
    const response = this.readResponse();

    if (!response || !this.waiters.length) {
      return;
    }

    const waiter = this.waiters.shift()!;

    if (response.code !== waiter.code) {
      waiter.reject(
        new Error(`SMTP expected ${waiter.code}, got ${response.code}: ${response.text}`),
      );
      return;
    }

    waiter.resolve();
    this.flush();
  }

  private readResponse() {
    const lines = this.buffer.split(/\r?\n/);
    const completeLineIndex = lines.findIndex((line) => /^\d{3} /.test(line));

    if (completeLineIndex === -1) {
      return undefined;
    }

    const responseLines = lines.slice(0, completeLineIndex + 1);
    this.buffer = lines.slice(completeLineIndex + 1).join('\n');

    return {
      code: Number(responseLines[completeLineIndex].slice(0, 3)),
      text: responseLines.join('\n'),
    };
  }

  private rejectAll(error: Error) {
    while (this.waiters.length) {
      this.waiters.shift()!.reject(error);
    }
  }
}
