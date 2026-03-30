#!/bin/sh
set -eu

cat > /app/config.cfg <<EOF
PORT: ${ADMIN_PANEL_PORT:-12002}
HOST: ${ADMIN_PANEL_HOST:-host.docker.internal}
BUFFER_LENGTH: ${ADMIN_PANEL_BUFFER_LENGTH:-256}
EOF

exec /app/AdminPanel "$@"
