.headers on
.mode column

-- 1. 10 последних показаний (таблица readings).
SELECT n, t_s, sensor, value FROM readings ORDER BY id DESC LIMIT 10;

-- 2. Минимум, среднее и максимум по каждому датчику (таблица readings).
SELECT sensor, MIN(value) AS min_value, ROUND(AVG(value), 2) AS avg_value, MAX(value) AS max_value
FROM readings GROUP BY sensor ORDER BY sensor;

-- 3. Все события по порядку времени (таблица events).
SELECT n, t_s, type, message FROM events ORDER BY t_s, id;
