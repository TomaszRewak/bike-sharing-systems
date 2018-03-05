import csv
from datetime import datetime
from typing import List

from data.model.raw_rides.raw_ride import RawRide


def load_raw_rides(file_path: str) -> List[RawRide]:
    with open(file_path, mode='r', encoding='utf-8') as file:
        rides = []

        reader = csv.DictReader(file, delimiter=';')
        for row in reader:
            bike_number = row['Numer roweru']
            date_of_rental = datetime.strptime(row['Data wynajmu'], "%d.%m.%Y %H:%M")
            date_of_return = datetime.strptime(row['Data zwrotu'], "%d.%m.%Y %H:%M")
            rental_station = row['Stacja wynajmu']
            return_station = row['Stacja zwrotu']

            rides.append(
                RawRide(bike_number, date_of_rental, date_of_return, rental_station, return_station)
            )

        return rides
