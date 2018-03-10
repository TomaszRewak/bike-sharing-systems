from datetime import datetime
from typing import List, Dict

from data.model.flow_matrix.flow_station import FlowStation


class FlowMatrix:
    def __init__(self,
                 date: datetime,
                 stations: Dict[int, FlowStation]):
        self.date = date
        self.stations = stations
