from typing import List

import networkx
import osmnx
from matplotlib import pyplot

from data.model.stations.station import Station
from data.model.stations.station_connection import StationConnection
from data.processing.city_network.city_network_station_mapping import map_city_network_stations


def render_city_network(
        city: networkx.multidigraph,
        stations: List[Station],
        station_connections: List[StationConnection]
):
    mapped_stations = map_city_network_stations(city, stations)
    station_mapping = {
        mapped_station.station_name: mapped_station.network_node
        for mapped_station in mapped_stations
    }

    station_nodes = [
        station.network_node
        for station in mapped_stations
    ]

    nc = ['r' if node in station_nodes else '#5588bb' for node in city]
    ns = [50 if node in station_nodes else 8 for node in city]

    fig, ax = osmnx.plot_graph(city, node_size=ns, node_color=nc, node_zorder=2, show=False, close=False)

    for station_connection in station_connections:
        pyplot.plot([
            city.nodes[station_mapping[station_connection.source]]['x'],
            city.nodes[station_mapping[station_connection.destination]]['x'],
        ], [
            city.nodes[station_mapping[station_connection.source]]['y'],
            city.nodes[station_mapping[station_connection.destination]]['y'],
        ], 'k', lw=2)

    # for station in stations:
    #     ax.text(
    #         station.longitude,
    #         station.latitude,
    #         station.name,
    #         color='purple'
    #     )

    pyplot.show()
