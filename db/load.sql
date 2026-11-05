BEGIN;
DELETE FROM readings;
DELETE FROM events;

INSERT INTO readings(n,t_s,sensor,value) VALUES
 (1,15,'s1',62.4),(1,15,'s2',22.1),(1,15,'s3',0),
 (2,30,'s1',41.0),(2,30,'s2',22.3),(2,30,'s3',1),
 (3,45,'s1',24.8),(3,45,'s2',22.4),(3,45,'s3',1),
 (4,60,'s1',20.6),(4,60,'s2',22.5),(4,60,'s3',1),
 (5,75,'s1',36.2),(5,75,'s2',22.6),(5,75,'s3',0);

INSERT INTO events(n,t_s,type,message) VALUES
 (3,45,'THRESHOLD','Обнаружено движение при освещённости ниже 30 %'),
 (5,75,'NORMAL','Аварийное условие исчезло');
COMMIT;
