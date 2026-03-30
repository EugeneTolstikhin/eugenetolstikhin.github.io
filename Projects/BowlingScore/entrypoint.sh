#!/bin/sh
set -eu

export TERM="${TERM:-xterm-256color}"

cat > /app/config.cfg <<EOF
PORT_CLIENT: ${BOWLING_PORT_CLIENT:-12001}
PORT_SERVER: ${BOWLING_PORT_SERVER:-12002}
HOST: ${BOWLING_HOST:-host.docker.internal}
BUFFER_LENGTH: ${BOWLING_BUFFER_LENGTH:-256}
POOL_SIZE: ${BOWLING_POOL_SIZE:-5}
EOF

exec /app/Bowling "$@"
