import csv
from typing import Dict

from data.model.weather.weather import Weather


def load_weather(file_path: str) -> Dict[str, Weather]:
    with open(file_path, mode='r') as file:
        reader = csv.reader(file, delimiter=',')

        weather = {
            '-'.join(row[2:5])
            :
            Weather(
                float(row[5]),
                float(row[7]),
                float(row[9]),
                float(row[19]) + float(row[21])
            )
            for row in reader
        }

        return weather
