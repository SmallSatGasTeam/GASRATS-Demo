# health_monitoring.fpp

# Define component instances (assuming they exist)
instance health
instance FlightLogic

# Connect HealthTelemetry port to FlightLogic's telemetry output port
connect health.HealthTelemetry FlightLogic.tlmOut
