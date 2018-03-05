from datetime import datetime


class FlowConstraint:
    def __init__(self,
                 time: datetime,
                 flow_duration: int):
        self.time = time
        self.flow_duration = flow_duration
