from typing import List

from data.model.predictions.station_prediction import StationPrediction


class Prediction:
    def __init__(self,
                 station_predictions: List[StationPrediction]):
        self.station_predictions = station_predictions
