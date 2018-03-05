from datetime import datetime


class Ride:
    def __init__(self,
                 date_of_rental: datetime,
                 date_of_return: datetime,
                 rental_station: int,
                 return_station: int):
        self.date_of_rental = date_of_rental
        self.date_of_return = date_of_return
        self.rental_station = rental_station
        self.return_station = return_station
