from typing import List


class DayDemandFrame:
    def __init__(self,
                 time: float,
                 values: List[float]):
        self.time = time
        self.values = values
