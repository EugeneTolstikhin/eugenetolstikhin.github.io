$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$output = Join-Path $root "docs/submission.pdf"

$pages = @(
  @(
    "Small Image Library Submission",
    "GitHub project link:",
    "https://github.com/EugeneTolstikhin/eugenetolstikhin.github.io/tree/main/Projects/SmallImageLibrary",
    "Runnable solution: Docker Compose locally. Live hosted deployment is not included for this multi-service stack.",
    "",
    "Architecture Overview",
    "Docker Compose runs four services: Next.js frontend, NestJS API, Postgres, and Redis.",
    "Prisma owns schema migrations, typed database access, and deterministic seed orchestration.",
    "The frontend owns the search workspace. The NestJS API owns contracts, preprocessing, indexing,",
    "analytics, cache integration, and persistence. This is a deliberate service-oriented interpretation",
    "of the challenge because the target role emphasizes full-stack ownership, REST APIs, database design,",
    "Docker, and automated testing.",
    "",
    "Search Strategy",
    "Postgres stores media rows through Prisma. At API startup, NestJS loads normalized rows and builds an in-memory",
    "inverted index. suchtext is weighted 5, fotografen is weighted 2, and bildnummer is weighted 8.",
    "Token and prefix matching are supported. Results can sort by relevance, newest date, or oldest date."
  ),
  @(
    "Preprocessing",
    "Dates are parsed from DD.MM.YYYY into ISO dates. Invalid dates become null, remain searchable,",
    "and are excluded from active date-range filters. Text is accent-normalized, lowercased, tokenized,",
    "and cleaned of noisy delimiters. Restrictions such as PUBLICATIONxINxGERxSUIxAUTxONLY are extracted",
    "as structured filter values.",
    "",
    "Data And Analytics",
    "Postgres is the durable media catalog. Prisma migrations own schema changes. Redis stores total search count, recent timing samples,",
    "common keywords, and short-lived cached search responses. The cache uses a simple TTL for the demo.",
    "For continuous ingestion, cache keys would include an index version or be invalidated by update events.",
    "",
    "Security",
    "The API validates query parameters with DTO constraints, limits query and page sizes, applies Helmet",
    "headers, restricts CORS, returns structured errors, and rate-limits requests. SQL writes use parameterized",
    "database access. The frontend uses CSP and renders metadata through React text nodes rather than raw HTML.",
    "",
    "Continuous Ingestion",
    "New items would be inserted into Postgres, normalized by the API or a worker, appended to the search",
    "index without blocking readers, and published to the production search index."
  ),
  @(
    "Scaling To Millions",
    "For 10,000 rows, the in-memory index is transparent and fast. For millions of rows, Postgres remains",
    "the source of truth and OpenSearch or Elasticsearch owns ranking and retrieval. Redis continues to",
    "serve analytics, hot query caching, and operational support.",
    "",
    "Testing Approach",
    "Automated tests cover tokenization, normalization, restriction extraction, invalid dates, weighted",
    "prefix scoring, filters, sorting, pagination, and frontend snippet highlighting. Further work would",
    "add live Postgres/Redis integration tests and browser tests for debounce, loading, empty, error, and",
    "pagination states.",
    "",
    "Trade-Offs",
    "This is heavier than a single Next.js app, but it better demonstrates service boundaries and backend",
    "reasoning. The main limitation is startup index rebuild time, which is acceptable here and points",
    "cleanly to OpenSearch for production scale. Further work would add hosted deployment, ingestion,",
    "integration/e2e tests, observability, and a dedicated search engine."
  )
)

function Escape-PdfText([string]$text) {
  return $text.Replace("\", "\\").Replace("(", "\(").Replace(")", "\)")
}

$objects = New-Object System.Collections.Generic.List[string]
$objects.Add("<< /Type /Catalog /Pages 2 0 R >>")
$kids = @()
$pageObjectNumbers = @()

for ($i = 0; $i -lt $pages.Count; $i++) {
  $pageObjectNumber = 3 + ($i * 2)
  $contentObjectNumber = $pageObjectNumber + 1
  $pageObjectNumbers += $pageObjectNumber
  $kids += "$pageObjectNumber 0 R"

  $commands = New-Object System.Collections.Generic.List[string]
  $commands.Add("BT")
  $commands.Add("/F1 20 Tf")
  $commands.Add("72 760 Td")
  foreach ($line in $pages[$i]) {
    if ($line -eq "") {
      $commands.Add("0 -18 Td")
      continue
    }
    $fontSize = if ($line -in @("Small Image Library Submission", "Architecture Overview", "Search Strategy", "Preprocessing", "Data And Analytics", "Security", "Continuous Ingestion", "Scaling To Millions", "Testing Approach", "Trade-Offs")) { 14 } else { 10 }
    $leading = if ($fontSize -eq 14) { 22 } else { 14 }
    $commands.Add("/F1 $fontSize Tf")
    $commands.Add("($(Escape-PdfText $line)) Tj")
    $commands.Add("0 -$leading Td")
  }
  $commands.Add("ET")
  $stream = ($commands -join "`n")

  $objects.Add("<< /Type /Page /Parent 2 0 R /MediaBox [0 0 612 792] /Resources << /Font << /F1 << /Type /Font /Subtype /Type1 /BaseFont /Helvetica >> >> >> /Contents $contentObjectNumber 0 R >>")
  $objects.Add("<< /Length $($stream.Length) >>`nstream`n$stream`nendstream")
}

$pagesObject = "<< /Type /Pages /Kids [ $($kids -join ' ') ] /Count $($pages.Count) >>"
$objects.Insert(1, $pagesObject)

$pdf = "%PDF-1.4`n"
$offsets = @(0)
for ($i = 0; $i -lt $objects.Count; $i++) {
  $offsets += $pdf.Length
  $pdf += "$($i + 1) 0 obj`n$($objects[$i])`nendobj`n"
}

$xrefOffset = $pdf.Length
$pdf += "xref`n0 $($objects.Count + 1)`n"
$pdf += "0000000000 65535 f `n"
for ($i = 1; $i -lt $offsets.Count; $i++) {
  $pdf += ("{0:D10} 00000 n `n" -f $offsets[$i])
}
$pdf += "trailer`n<< /Size $($objects.Count + 1) /Root 1 0 R >>`nstartxref`n$xrefOffset`n%%EOF`n"

try {
  Set-Content -Path $output -Value $pdf -Encoding Ascii -NoNewline
  Write-Output $output
} catch {
  $fallback = Join-Path $root "docs/submission.updated.pdf"
  Set-Content -Path $fallback -Value $pdf -Encoding Ascii -NoNewline
  Write-Output $fallback
}
