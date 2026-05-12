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

## TUI mode

If you want to test the original `ncurses` UI in Docker, use the dedicated TUI profile.

Terminal 1:

```powershell
docker compose --profile tui up -d --build points-generator
```

Terminal 2:

```powershell
docker compose --profile tui up --build bowling-score-tui
```

Terminal 3:

```powershell
docker compose --profile tui run --rm --no-deps admin-panel-tui
```

Notes:

- `bowling-score-tui` uses the original `ncurses` view.
- It listens on `12003` so it does not collide with the default plain-text `bowling-score` service on `12002`.
- `admin-panel-tui` is pointed at `bowling-score-tui` automatically.
- For the cleanest experience, run the TUI profile without the default plain-text `bowling-score` service at the same time.

## Qt/QML mode

`BowlingScore` also has a Qt Quick score-table UI. Docker containers do not own your desktop display, so start an X server first.

Windows setup:

1. Install and start VcXsrv, X410, or another X server.
2. For VcXsrv, use `Multiple windows`, `Start no client`, and enable `Disable access control`.
3. Keep the X server running while the container is open.

Run the Qt/QML demo from `Projects`:

```powershell
docker compose --profile qt up --build bowling-score-qt
```

The compose service runs with:

- `BOWLING_VIEW=ui`
- `BOWLING_LISTENER=simulation`
- sample players `Ada`, `Linus`, and `Grace`
- `DISPLAY=host.docker.internal:0.0`

You should see a `BowlingScore` desktop window with an animated score table. The simulation plays one full game, then leaves the window open with the final scores. Close the window or press `Ctrl+C` in the terminal to stop it.

Linux note: if your host already exposes X11 through the normal Unix socket, run with your host display instead:

```powershell
$env:DISPLAY=":0"
docker compose --profile qt up --build bowling-score-qt
```

## AdminPanel Qt mode

`AdminPanel` also has a Qt desktop client. It sends the same player-name handshake as the terminal version, but through a small GUI.

Run the BowlingScore TUI server and the Qt AdminPanel together:

```powershell
docker compose --profile tui --profile qt up -d --build points-generator bowling-score-tui admin-panel-qt
```

The Qt AdminPanel is published through VNC, not HTTP. Open a VNC viewer and connect to:

```text
localhost:5901
```

In the Qt window:

1. Keep `Host` as `bowling-score-tui`.
2. Keep `Port` as `12003`.
3. Enter player names, for example `Ada Linus Grace`.
4. Click `Send Players`.

The status area should show `Server answer: Accepted`, and the BowlingScore TUI container will start the game.

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
  - `BOWLING_LISTENER`
  - `BOWLING_PORT_CLIENT`
  - `BOWLING_PORT_SERVER`
  - `BOWLING_BUFFER_LENGTH`
  - `BOWLING_POOL_SIZE`
  - `BOWLING_VIEW`
- `AdminPanel`
  - `ADMIN_PANEL_HOST`
  - `ADMIN_PANEL_PORT`
  - `ADMIN_PANEL_BUFFER_LENGTH`

Each container generates its `config.cfg` at startup from these variables, so the original local config files remain unchanged.
