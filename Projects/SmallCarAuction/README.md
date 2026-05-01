# SmallCarAuction

Full-stack car auction demo with a NestJS backend and Next.js frontend.

The frontend uses REST for commands and Server-Sent Events from `GET /auctions/events` for live dashboard notifications.

## Run Everything

```bash
docker compose up -d
```

- Frontend: http://localhost:5173
- API: http://localhost:3000
- API docs: http://localhost:3000/docs
- Mailpit: http://localhost:8025
- Jaeger: http://localhost:16686

## Run Backend Only

```bash
cd backend
docker compose up -d
```
