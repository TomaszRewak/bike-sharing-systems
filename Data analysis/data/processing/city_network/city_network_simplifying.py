from typing import List

import networkx
import osmnx
from matplotlib import pyplot

from data.model.stations.station import Station
from data.model.stations.station_connection import StationConnection
from data.processing.city_network.city_network_path_filtering import filter_common_paths
from data.processing.city_network.city_network_path_finding import get_paths
from data.processing.city_network.city_network_station_mapping import map_city_network_stations


def simplify_city_network(
        city: networkx.multidigraph,
        stations: List[Station]
) -> List[StationConnection]:
    mapped_stations = map_city_network_stations(city, stations)

    station_nodes = [
        station.network_node
        for station in mapped_stations
    ]

    connections = []

    for station in station_nodes:
        paths = get_paths(city, station, station_nodes)
        paths = filter_common_paths(paths)

        nearest_nodes = [node.to for node in paths]

        connections = connections + [
            (station, nearest_node)
            for nearest_node in nearest_nodes if (nearest_node, station) not in connections
        ]

    connections = connections + [
        (station_b, station_a)
        for (station_a, station_b) in connections
    ]

    print(len(connections))

    mapping_reversal = {
        mapped_station.network_node: mapped_station.station_name
        for mapped_station in mapped_stations
    }

    return [
        StationConnection(
            mapping_reversal[station_a],
            mapping_reversal[station_b]
        )
        for (station_a, station_b) in connections
    ]
