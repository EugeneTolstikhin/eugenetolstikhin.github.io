$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$output = Join-Path $root "docs/submission.pdf"

$pages = @(
  @(
    "Small Image Library Pure Next Submission",
    "",
    "Architecture Overview",
    "Single Dockerized Next.js TypeScript application. The frontend lives in app/page.tsx,",
    "and the backend search layer uses App Router route handlers under app/api/*.",
    "This version follows the challenge wording closely: the search layer is inside Next.js.",
    "There are no separate NestJS, Postgres, or Redis services.",
    "",
    "Search Strategy",
    "The app uses the two provided media examples plus generated fixtures to create 10,000 items.",
    "Each item is normalized and loaded into an in-memory inverted index at runtime.",
    "suchtext is weighted 5, fotografen is weighted 2, and bildnummer is weighted 8.",
    "The index supports token and prefix matching, with date and relevance sorting."
  ),
  @(
    "Preprocessing",
    "German dates are parsed from DD.MM.YYYY to ISO dates. Invalid dates become null, remain",
    "keyword-searchable, and are excluded from active date-range filters. Text is accent-normalized,",
    "lowercased, tokenized, and cleaned of noisy delimiters. Restrictions such as",
    "PUBLICATIONxINxGERxSUIxAUTxONLY are extracted as structured filter labels.",
    "",
    "API And UI",
    "The app exposes GET /api/search, GET /api/facets, and GET /api/analytics.",
    "The search response includes items, page, pageSize, total, totalPages, and timingMs.",
    "The Tailwind UI includes debounced search, credit filter, date range controls, restriction chips,",
    "sorting, result cards, snippets, loading, empty, error, and pagination states.",
    "",
    "Analytics",
    "The demo tracks total searches, recent query timings, and common normalized keywords in memory."
  ),
  @(
    "Scaling And Continuous Ingestion",
    "For 10,000 records, an in-memory index is simple, transparent, and fast enough for the challenge.",
    "For millions of records, I would keep the API contract stable but move search indexing and ranking",
    "to Meilisearch, OpenSearch, or Elasticsearch. New items would be preprocessed, stored durably,",
    "indexed asynchronously, and trigger cache invalidation or index versioning.",
    "",
    "Testing Approach",
    "Automated tests cover date parsing, invalid dates, restriction extraction, accent normalization,",
    "tokenization, search, filters, sorting, pagination, parameter clamping, and XSS-safe snippets.",
    "",
    "Trade-Offs",
    "This version is directly aligned with the prompt and easy to run, but less production-like.",
    "Data and analytics are in memory. A production version needs durable storage, async ingestion,",
    "a dedicated search backend, observability, integration tests, CI/CD, and hosted deployment."
  )
)

function Escape-PdfText([string]$text) {
  return $text.Replace("\", "\\").Replace("(", "\(").Replace(")", "\)")
}

$objects = New-Object System.Collections.Generic.List[string]
$objects.Add("<< /Type /Catalog /Pages 2 0 R >>")
$kids = @()

for ($i = 0; $i -lt $pages.Count; $i++) {
  $pageObjectNumber = 3 + ($i * 2)
  $contentObjectNumber = $pageObjectNumber + 1
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
    $fontSize = if ($line -in @("Small Image Library Pure Next Submission", "Architecture Overview", "Search Strategy", "Preprocessing", "API And UI", "Analytics", "Scaling And Continuous Ingestion", "Testing Approach", "Trade-Offs")) { 14 } else { 10 }
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

Set-Content -Path $output -Value $pdf -Encoding Ascii -NoNewline
Write-Output $output
