# Docker setup for test projects

This folder now contains one `Dockerfile` per project and a shared `docker-compose.yml` so you can run the apps either one-by-one or as a connected set.

## Projects

- `PointsGenerator`: socket server on `12001`
- `BowlingScore`: main score application, listens on `12002` and connects to `PointsGenerator`
- `AdminPanel`: client for `BowlingScore`

## Build and run all projects together

From `Projects`:

```powershell
docker compose up --build
```

This starts all containers, but `BowlingScore` is an `ncurses` terminal UI. Because of that, the most reliable way to test the full flow is to give it its own terminal instead of multiplexing it together with another attached service.

## Recommended test workflow

Terminal 1: start the points service in the background

```powershell
docker compose up -d --build points-generator
```

Terminal 2: run `BowlingScore` as a normal log-streaming service

```powershell
docker compose up --build bowling-score
```

Terminal 3: send player names from the admin client

```powershell
docker compose run --rm --no-deps admin-panel
```

After `Accepted`, the `BowlingScore` terminal should print plain-text game progress.
In Docker, `BowlingScore` now uses a plain-text view and the local socket listener so the full three-project flow is visible in terminal output.

## Alternative startup

```powershell
docker compose up --build points-generator bowling-score
```

This now works cleanly because Docker uses the plain-text view instead of `ncurses`.

This starts:

- `points-generator`
- `bowling-score`
- `admin-panel`

Compose wires the container hostnames automatically:

- `bowling-score -> points-generator`
- `admin-panel -> bowling-score`

## Run one project independently

Build a single image:

```powershell
docker compose build points-generator
docker compose build bowling-score
docker compose build admin-panel
```

Run a single service:

```powershell
docker compose up points-generator
docker compose up bowling-score
docker compose run --rm --no-deps admin-panel
```

For true standalone testing against a custom host, override the runtime env vars:

```powershell
docker run --rm -it -e BOWLING_HOST=host.docker.internal -p 12002:12002 et-test-bowling-score:latest
docker run --rm -it -e ADMIN_PANEL_HOST=host.docker.internal et-test-admin-panel:latest
docker run --rm -it -p 12001:12001 et-test-points-generator:latest
```

## Runtime environment variables

- `PointsGenerator`
  - `POINTS_GENERATOR_HOST`
  - `POINTS_GENERATOR_PORT`
  - `POINTS_GENERATOR_BUFFER_LENGTH`
- `BowlingScore`
  - `BOWLING_HOST`
  - `BOWLING_PORT_CLIENT`
  - `BOWLING_PORT_SERVER`
  - `BOWLING_BUFFER_LENGTH`
  - `BOWLING_POOL_SIZE`
- `AdminPanel`
  - `ADMIN_PANEL_HOST`
  - `ADMIN_PANEL_PORT`
  - `ADMIN_PANEL_BUFFER_LENGTH`

Each container generates its `config.cfg` at startup from these variables, so the original local config files remain unchanged.
