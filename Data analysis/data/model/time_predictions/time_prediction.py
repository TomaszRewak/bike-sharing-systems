import datetime


class TimePrediction:
    def __init__(self,
                 origin: str,
                 destination: str,
                 duration: int,
                 start_time: datetime.datetime):
        self.origin = origin
        self.destination = destination
        self.duration = duration
        self.start_time = start_time
