#!/bin/sh
set -eu

cat > /app/config.cfg <<EOF
PORT: ${POINTS_GENERATOR_PORT:-12001}
HOST: ${POINTS_GENERATOR_HOST:-0.0.0.0}
BUFFER_LENGTH: ${POINTS_GENERATOR_BUFFER_LENGTH:-256}
EOF

exec /app/PointsGenerator "$@"
