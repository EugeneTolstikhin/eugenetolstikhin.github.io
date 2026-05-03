CREATE TABLE IF NOT EXISTS "media_items" (
  "id" BIGSERIAL PRIMARY KEY,
  "suchtext" TEXT NOT NULL,
  "bildnummer" TEXT NOT NULL UNIQUE,
  "fotografen" TEXT NOT NULL,
  "datum" TEXT NOT NULL,
  "hoehe" TEXT NOT NULL,
  "breite" TEXT NOT NULL,
  "date_iso" DATE NULL,
  "restrictions" TEXT[] NOT NULL DEFAULT '{}',
  "searchable_text" TEXT NOT NULL,
  "tokens" TEXT[] NOT NULL DEFAULT '{}',
  "created_at" TIMESTAMPTZ NOT NULL DEFAULT now()
);

CREATE INDEX IF NOT EXISTS "idx_media_items_credit" ON "media_items" ("fotografen");
CREATE INDEX IF NOT EXISTS "idx_media_items_date_iso" ON "media_items" ("date_iso");
CREATE INDEX IF NOT EXISTS "idx_media_items_restrictions" ON "media_items" USING GIN ("restrictions");
CREATE INDEX IF NOT EXISTS "idx_media_items_tokens" ON "media_items" USING GIN ("tokens");
