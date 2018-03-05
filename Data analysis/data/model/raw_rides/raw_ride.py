from datetime import datetime


class RawRide:
    def __init__(self,
                 bike_number: int,
                 date_of_rental: datetime,
                 date_of_return: datetime,
                 rental_station: str,
                 return_station: str):
        self.bike_number = bike_number
        self.date_of_rental = date_of_rental
        self.date_of_return = date_of_return
        self.rental_station = rental_station
        self.return_station = return_station
