import { Injectable } from '@nestjs/common';
import { MediaItem } from './media.types';
import { PrismaService } from '../prisma/prisma.service';

type PrismaMediaItem = Awaited<ReturnType<PrismaService['mediaItem']['findMany']>>[number];

@Injectable()
export class DatabaseService {
  constructor(private readonly prisma: PrismaService) {}

  async findAllMedia(): Promise<MediaItem[]> {
    const rows = await this.prisma.mediaItem.findMany({
      orderBy: { id: 'asc' },
    });

    return rows.map((row) => this.toMediaItem(row));
  }

  async getFacets(): Promise<{ credits: string[]; restrictions: string[] }> {
    const rows = await this.prisma.mediaItem.findMany({
      select: {
        fotografen: true,
        restrictions: true,
      },
    });

    return {
      credits: Array.from(new Set(rows.map((row) => row.fotografen))).sort(),
      restrictions: Array.from(new Set(rows.flatMap((row) => row.restrictions))).sort(),
    };
  }

  private toMediaItem(row: PrismaMediaItem): MediaItem {
    return {
      id: Number(row.id),
      suchtext: row.suchtext,
      bildnummer: row.bildnummer,
      fotografen: row.fotografen,
      datum: row.datum,
      hoehe: row.hoehe,
      breite: row.breite,
      dateIso: row.dateIso ? row.dateIso.toISOString().slice(0, 10) : null,
      restrictions: row.restrictions,
      searchableText: row.searchableText,
      tokens: row.tokens,
    };
  }
}
