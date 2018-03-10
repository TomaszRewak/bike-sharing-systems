from typing import List

from data.model.distance_functions.day_distance import DayDistance


class DistanceFunction:
    def __init__(self,
                 day: str,
                 distances: List[DayDistance]):
        self.day = day
        self.distances = distances
