from data.model.day.day import Day
from data.model.weather.weather import Weather


class OfflineFeatures:
    def __init__(self,
                 weather: Weather,
                 day: Day
                 ):
        self.weather = weather
        self.day = day

    def vector(self):
        return [
            # self.day.day,
            self.day.month,
            # self.day.year,
            self.day.day_of_the_week,
            self.weather.avg_cloud,
            self.weather.avg_wind,
            self.weather.avg_temp,
            self.weather.avg_fall
        ]


