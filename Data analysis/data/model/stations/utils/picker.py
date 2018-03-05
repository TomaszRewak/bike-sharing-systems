import pickle
from typing import TextIO, List, BinaryIO
from data.model.stations.station import Station
from data.model.stations.station_connection import StationConnection
from .loader import load_stations


def unpickle_stations(
        file_path: str
) -> List[Station]:
    with open(file_path, mode='rb') as file:
        return pickle.load(file)


def pickle_stations(
        file_path: str,
        stations: List[Station]
):
    with open(file_path, mode='wb+') as pickle_file:
        pickle.dump(stations, pickle_file)


def unpickle_station_connections(
        file_path: str
) -> List[StationConnection]:
    with open(file_path, mode='rb') as file:
        return pickle.load(file)


def pickle_station_connections(
        file_path: str,
        station_connections: List[StationConnection]
):
    with open(file_path, mode='wb+') as pickle_file:
        pickle.dump(station_connections, pickle_file)

