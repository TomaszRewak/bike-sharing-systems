from typing import Dict

from data.model.day.utils.formatter import read_day_format
from data.model.features.offline_features import OfflineFeatures
from data.model.weather.weather import Weather


def process_offline_features(weather_data: Dict[str, Weather]) -> Dict[str, OfflineFeatures]:
    return {
        date
        :
        OfflineFeatures(
            weather,
            read_day_format(date)
        )
        for date, weather in weather_data.items()
    }
