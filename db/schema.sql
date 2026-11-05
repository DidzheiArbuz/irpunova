PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS readings (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    n INTEGER NOT NULL,
    t_s INTEGER NOT NULL,
    sensor TEXT NOT NULL CHECK (sensor IN ('s1', 's2', 's3')),
    value REAL NOT NULL,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS events (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    n INTEGER NOT NULL,
    t_s INTEGER NOT NULL,
    type TEXT NOT NULL CHECK (type IN ('THRESHOLD', 'NORMAL', 'FAULT')),
    message TEXT,
    created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX IF NOT EXISTS idx_readings_sensor ON readings(sensor);
CREATE INDEX IF NOT EXISTS idx_readings_time ON readings(t_s);
CREATE INDEX IF NOT EXISTS idx_events_type ON events(type);
