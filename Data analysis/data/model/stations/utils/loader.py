import csv
from typing import List
from data.model.stations.station import Station


def load_stations(
        file_path='../resources/raw/stations.csv'
) -> List[Station]:
    with open(file_path, mode='r', encoding='utf-8') as file:
        stations = []

        reader = csv.DictReader(file, delimiter=';')

        for index, row in enumerate(reader):
            name = row['Lokalizacja']
            latitude = float(row['Szer. geograficzna'])
            longitude = float(row['Dł. geograficzna'])
            capacity = 10

            stations.append(
                Station(index, name, latitude, longitude, capacity)
            )

        return stations
