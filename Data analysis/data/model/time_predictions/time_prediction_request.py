import datetime


class TimePredictionRequestLocation:
    def __init__(self,
                 name: str,
                 latitude: float,
                 longitude: float):
        self.name = name
        self.latitude = latitude
        self.longitude = longitude


class TimePredictionRequest:
    def __init__(self,
                 origin: TimePredictionRequestLocation,
                 destination: TimePredictionRequestLocation,
                 time: datetime.datetime):
        self.origin = origin
        self.destination = destination
        self.time = time
