from data.model.day.day import Day
from data.model.weather.weather import Weather


class Features:
    def __init__(self,
                 weather: Weather,
                 day: Day
                 ):
        self.weather = weather
        self.day = day

    def vector(self):
        return [
            (self.day.month - 1.) / 11.,
            (self.day.day_of_the_week - 0.) / 6.,
            0. if self.day.day_of_the_week <= 4 else 1.,
            (self.weather.avg_cloud - 0.) / 8.,
            (self.weather.avg_wind - 1.) / 8.,
            (self.weather.avg_temp + 3.1) / 32.5,
            (self.weather.avg_fall - 0.) / 31.7,
        ]
