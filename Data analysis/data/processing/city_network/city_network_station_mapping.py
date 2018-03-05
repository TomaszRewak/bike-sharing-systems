from typing import List

import networkx
import osmnx

from data.model.city_network.city_network_station import CityNetworkStation
from data.model.stations.station import Station


def map_city_network_stations(
        city: networkx.multidigraph,
        stations: List[Station]
) -> List[CityNetworkStation]:
    return [
        CityNetworkStation(
            station.name,
            osmnx.utils.get_nearest_node(city, (station.latitude, station.longitude))
        )
        for station in stations
    ]
