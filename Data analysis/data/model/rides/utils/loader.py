from typing import List

from data.model.rides.ride import Ride


def save_rides(
        file_path: str,
        rides: List[Ride]
):
    with open(file_path, mode='w+') as file:
        file.write('{0}\n'.format(len(rides)))

        for ride in rides:
            file.write('{0:%d.%m.%Y %H:%M} {1:%d.%m.%Y %H:%M} {2} {3}\n'.format(
                ride.date_of_rental,
                ride.date_of_return,
                ride.rental_station,
                ride.return_station
            ))
