from typing import List

from data.model.flow_matrix.flow_connection import FlowConnection
from data.model.stations.station import Station


class FlowStation:
    def __init__(self,
                 connections: List[FlowConnection]):
        self.connections = connections

