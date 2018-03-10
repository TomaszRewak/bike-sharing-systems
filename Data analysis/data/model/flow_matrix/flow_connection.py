from datetime import datetime, date
from typing import List

from data.model.flow_matrix.flow_constraint import FlowConstraint


class FlowConnection:
    def __init__(self,
                 destination: int,
                 constraints: List[FlowConstraint]):
        self.destination = destination
        self.constraints = sorted(constraints, key=lambda x: x.time)

    def get_duration(self, time: datetime) -> float:
        if time <= self.constraints[0].time:
            return self.constraints[0].flow_duration

        if time >= self.constraints[-1].time:
            return self.constraints[-1].flow_duration

        constraint = 0

        while self.constraints[constraint + 1].time < time:
            constraint = constraint + 1

        constraint_a = self.constraints[constraint]
        constraint_b = self.constraints[constraint + 1]

        duration_a = constraint_a.flow_duration
        duration_b = constraint_b.flow_duration

        time_a = datetime.combine(date.today(), constraint_a.time)
        time_b = datetime.combine(date.today(), constraint_b.time)
        time_x = datetime.combine(date.today(), time)

        return duration_a + (duration_b - duration_a) * (time_x - time_a).total_seconds() / (time_b - time_a).total_seconds()
