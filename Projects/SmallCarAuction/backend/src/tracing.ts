import { getNodeAutoInstrumentations } from '@opentelemetry/auto-instrumentations-node';
import { OTLPTraceExporter } from '@opentelemetry/exporter-trace-otlp-http';
import { resourceFromAttributes } from '@opentelemetry/resources';
import { NodeSDK } from '@opentelemetry/sdk-node';
import { ATTR_SERVICE_NAME, ATTR_SERVICE_VERSION } from '@opentelemetry/semantic-conventions';

const tracingEnabled = process.env.TRACING_ENABLED === 'true';

if (tracingEnabled) {
  const sdk = new NodeSDK({
    resource: resourceFromAttributes({
      [ATTR_SERVICE_NAME]: process.env.OTEL_SERVICE_NAME ?? 'small-car-auction-api',
      [ATTR_SERVICE_VERSION]: process.env.npm_package_version ?? '0.1.0',
    }),
    traceExporter: new OTLPTraceExporter({
      url:
        process.env.OTEL_EXPORTER_OTLP_TRACES_ENDPOINT ??
        'http://jaeger:4318/v1/traces',
    }),
    instrumentations: [getNodeAutoInstrumentations()],
  });

  sdk.start();

  process.on('SIGTERM', () => {
    void sdk.shutdown();
  });
}
