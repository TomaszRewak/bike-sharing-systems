from typing import List

from data.model.flow_matrix.flow_station import FlowStation


class FlowMatrix:
    def __init__(self,
                 date: str,
                 stations: List[FlowStation]):
        self.date = date
        self.stations = stations
