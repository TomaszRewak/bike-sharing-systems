import math
from statistics import mean

import numpy as np

from data.model.predictions.prediction import Prediction
from data.model.predictions.station_prediction import StationPrediction


def analyse_station_prediction_difference(station_a: StationPrediction, station_b: StationPrediction) -> float:
    return float(np.mean((station_a.predicted_values-station_b.predicted_values)**2))


def analyse_predictions_difference(prediction_a: Prediction, prediction_b: Prediction) -> float:
    return mean([
        analyse_station_prediction_difference(station_a, station_b)
        for (station_a, station_b) in zip(prediction_a.station_predictions, prediction_b.station_predictions)
    ])
