FROM ubuntu:24.04 AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends build-essential cmake libncurses-dev qt6-base-dev qt6-declarative-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --config Release

FROM ubuntu:24.04 AS runtime

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        libncurses6 \
        libqt6core6 \
        libqt6gui6 \
        libqt6qml6 \
        libqt6quick6 \
        libgl1 \
        libxcb-cursor0 \
        libxkbcommon-x11-0 \
        qml6-module-qtquick \
        qml6-module-qtquick-window \
        qml6-module-qtqml-workerscript \
        qt6-qpa-plugins \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=build /src/build/BowlingScore /app/BowlingScore
COPY entrypoint.sh /app/entrypoint.sh

RUN chmod +x /app/entrypoint.sh

EXPOSE 12002

ENTRYPOINT ["/app/entrypoint.sh"]
