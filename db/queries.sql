.headers on
.mode column
SELECT * FROM readings ORDER BY n, sensor;
SELECT sensor, MIN(value) AS min_value, ROUND(AVG(value), 2) AS avg_value,
       MAX(value) AS max_value FROM readings GROUP BY sensor;
SELECT * FROM events WHERE type IN ('THRESHOLD', 'FAULT') ORDER BY t_s;
SELECT type, COUNT(*) AS amount FROM events GROUP BY type;
