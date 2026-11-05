"""Python-система мониторинга помещения, вариант 11."""
from __future__ import annotations

import argparse
import sqlite3
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DB_PATH = ROOT / "db" / "variant11.db"
LOG_PATH = ROOT / "data" / "python_log.txt"
PERIOD_SECONDS = 15
LIGHT_THRESHOLD = 30.0


@dataclass
class Reading:
    number: int
    light: float
    temperature: float
    motion: int


def init_db(connection: sqlite3.Connection) -> None:
    connection.executescript((ROOT / "db" / "schema.sql").read_text(encoding="utf-8"))


def save_reading(connection: sqlite3.Connection, reading: Reading, previous_alarm: bool | None) -> bool:
    t_s = reading.number * PERIOD_SECONDS
    connection.executemany(
        "INSERT INTO readings(n,t_s,sensor,value) VALUES (?,?,?,?)",
        ((reading.number, t_s, sensor, value) for sensor, value in (
            ("s1", reading.light), ("s2", reading.temperature), ("s3", reading.motion)
        )),
    )
    alarm = reading.motion == 1 and reading.light < LIGHT_THRESHOLD
    if previous_alarm is not None and alarm != previous_alarm:
        event_type = "THRESHOLD" if alarm else "NORMAL"
        message = "Движение при освещённости ниже 30 %" if alarm else "Условие тревоги исчезло"
        connection.execute(
            "INSERT INTO events(n,t_s,type,message) VALUES (?,?,?,?)",
            (reading.number, t_s, event_type, message),
        )
    connection.commit()
    return alarm


def run_demo() -> None:
    samples = [
        Reading(1, 62.4, 22.1, 0), Reading(2, 41.0, 22.3, 1),
        Reading(3, 24.8, 22.4, 1), Reading(4, 20.6, 22.5, 1),
        Reading(5, 36.2, 22.6, 0),
    ]
    DB_PATH.parent.mkdir(exist_ok=True)
    LOG_PATH.parent.mkdir(exist_ok=True)
    with sqlite3.connect(DB_PATH) as connection, LOG_PATH.open("w", encoding="utf-8") as log:
        init_db(connection)
        connection.execute("DELETE FROM readings")
        connection.execute("DELETE FROM events")
        connection.commit()
        previous_alarm = None
        for reading in samples:
            alarm = save_reading(connection, reading, previous_alarm)
            data = f"DATA n={reading.number} s1={reading.light:.1f} s2={reading.temperature:.1f} s3={reading.motion}"
            print(data); log.write(data + "\n")
            if previous_alarm is not None and alarm != previous_alarm:
                event = f"EVENT n={reading.number} type={'THRESHOLD' if alarm else 'NORMAL'}"
                print(event); log.write(event + "\n")
            previous_alarm = alarm


if __name__ == "__main__":
    argparse.ArgumentParser(description="Мониторинг помещения, вариант 11").parse_args()
    run_demo()
