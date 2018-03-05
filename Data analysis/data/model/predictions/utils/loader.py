from typing import Dict

import os

import numpy as np

from data.model.predictions.prediction import Prediction
from data.model.predictions.station_prediction import StationPrediction


def load_predictions(directory_path: str) -> Dict[str, Prediction]:
    predictions = {}

    for day_file_name in os.listdir(directory_path):
        day_file_path = os.path.join(directory_path, day_file_name)

        with open(day_file_path, 'r') as day_file:
            header_line = day_file.readline()
            [nodes, time_frames] = [int(value) for value in header_line.split()]

            station_predictions = [[] for _ in range(nodes)]

            for _ in range(time_frames):
                time_frame_predictions = [
                    float(value)
                    for value in day_file.readline().split()
                ]

                for index, value in enumerate(time_frame_predictions):
                    station_predictions[index].append(value)

            predictions[day_file_name] = Prediction([
                StationPrediction(np.array(station_prediction))
                for station_prediction in station_predictions
            ])

    return predictions
