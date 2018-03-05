class Station:
    def __init__(self,
                 index: int,
                 name: str,
                 latitude: float,
                 longitude: float,
                 capacity: int):
        self.index = index
        self.name = name
        self.latitude = latitude
        self.longitude = longitude
        self.capacity = capacity
