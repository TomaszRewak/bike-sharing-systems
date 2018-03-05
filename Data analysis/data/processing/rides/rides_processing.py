from typing import List

from data.model.raw_rides.raw_ride import RawRide
from data.model.rides.ride import Ride
from data.model.stations.station import Station


def process_rides_data(raw_rides: List[RawRide], stations: List[Station]) -> List[Ride]:
    station_mapping = dict([
        (station.name, station.index)
        for station in stations
    ])

    raw_rides = [
        ride
        for ride in raw_rides
        if ride.rental_station in station_mapping and ride.return_station in station_mapping
    ]

    rides = [
        Ride(
            raw_ride.date_of_rental,
            raw_ride.date_of_return,
            station_mapping[raw_ride.rental_station],
            station_mapping[raw_ride.return_station]
        )
        for raw_ride in raw_rides
    ]

    return rides
