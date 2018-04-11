from typing import Dict

from data.model.day.utils.formatter import read_day_format
from data.model.features.features import Features
from data.model.weather.weather import Weather


def process_features(weather_data: Dict[str, Weather]) -> Dict[str, Features]:
    return {
        date
        :
        Features(
            weather,
            read_day_format(date)
        )
        for date, weather in weather_data.items()
    }
