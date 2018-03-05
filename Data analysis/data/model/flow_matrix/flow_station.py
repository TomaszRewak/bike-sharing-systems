from typing import List

from data.model.flow_matrix.flow_connection import FlowConnection
from data.model.stations.station import Station


class FlowStation:
    def __init__(self,
                 station: int,
                 connections: List[FlowConnection]):
        self.station = station
        self.connections = connections

