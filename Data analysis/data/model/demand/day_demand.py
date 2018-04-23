from typing import List

from data.model.demand.day_demand_frame import DayDemandFrame


class DayDemand:
    def __init__(self,
                 day: str,
                 frames: List[DayDemandFrame]):
        self.day = day
        self.frames = frames