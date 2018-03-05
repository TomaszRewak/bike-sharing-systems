import datetime
from typing import List

from data.model.stations.station import Station
from data.model.stations.station_connection import StationConnection
from data.model.time_predictions.time_prediction_request import TimePredictionRequest, TimePredictionRequestLocation


def prepare_time_prediction_requests(
        stations: List[Station],
        station_connections: List[StationConnection],
        times: List[datetime.datetime]
):
    station_name_mapping = {
        station.name: station
        for station in stations
    }

    return [
        TimePredictionRequest(
            TimePredictionRequestLocation(
                station_connection.source,
                station_name_mapping[station_connection.source].latitude,
                station_name_mapping[station_connection.source].longitude
            ),
            TimePredictionRequestLocation(
                station_connection.destination,
                station_name_mapping[station_connection.destination].latitude,
                station_name_mapping[station_connection.destination].longitude
            ),
            time
        )
        for station_connection in station_connections
        for time in times
    ]
