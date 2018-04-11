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
            # self.day.day,
            (self.day.month - 1.) / 11.,
            # self.day.year,
            (self.day.day_of_the_week - 0.) / 6.,
            (self.weather.avg_cloud - 0.) / 8.,
            (self.weather.avg_wind - 1.) / 8.,
            (self.weather.avg_temp + 3.1) / 32.5,
            (self.weather.avg_fall - 0.) / 31.7
        ]


