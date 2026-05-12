# Ievgen Tolstikhin Engineering Library

Static Nuxt/Vue portfolio-library for `eugenetolstikhin.github.io`.

The site is intentionally not a classic CV page. It presents skills, languages, technologies, local test projects, and public GitHub repositories as a searchable engineering evidence map.

Visible portfolio content is employer-neutral and photo-free. This keeps the page suitable for remote and international applications where photos are discouraged or disallowed.

## Source Material

- Local project library: `Projects/`
- Public GitHub snapshot: `https://api.github.com/users/EugeneTolstikhin/repos?per_page=100&sort=updated`

## GitHub Repository Data

The site ships with a checked fallback snapshot of public repositories and refreshes it in the browser from the public GitHub API. On GitHub Pages, newly created, updated, or removed public repositories are reflected at page load when the browser can reach GitHub. If the API is unavailable or rate-limited, the fallback snapshot remains visible.

## Language Model

The UI currently supports English and German through localized data objects. More languages can be added by extending `localeOptions`, `uiByLocale`, and the localized text fields in `data/portfolio.ts`.

## Docker-Only Local Workflow

No local `npm` or local `python` commands are required.

```powershell
docker compose run --rm portfolio-check
docker compose run --rm portfolio-check sh -c "npm ci && npm run generate"
docker compose up portfolio
```

Open `http://localhost:3000`.

Optional browser smoke check after the dev server is running:

```powershell
docker run --rm -v "${PWD}:/work" -v eugenetolstikhingithubio_portfolio_node_modules:/work/node_modules -w /work -e PORTFOLIO_URL=http://host.docker.internal:3000 mcr.microsoft.com/playwright:v1.57.0-jammy node tests/browser-smoke.mjs
```

## Deployment Readiness

The GitHub Pages workflow is manual-only for now. After local DoD is accepted, run the workflow from GitHub Actions or add a push trigger.

The generated artifact is `.output/public`.
