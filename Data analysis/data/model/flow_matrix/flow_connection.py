from typing import List

from data.model.flow_matrix.flow_constraint import FlowConstraint
from data.model.stations.station import Station


class FlowConnection:
    def __init__(self,
                 destination: int,
                 constraints: List[FlowConstraint]):
        self.destination = destination
        self.constraints = constraints
