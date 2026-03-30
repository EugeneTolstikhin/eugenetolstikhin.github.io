FROM debian:bookworm-slim AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends build-essential cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --config Release

FROM debian:bookworm-slim AS runtime

WORKDIR /app

COPY --from=build /src/build/PointsGenerator /app/PointsGenerator
COPY entrypoint.sh /app/entrypoint.sh

RUN chmod +x /app/entrypoint.sh

EXPOSE 12001

ENTRYPOINT ["/app/entrypoint.sh"]
