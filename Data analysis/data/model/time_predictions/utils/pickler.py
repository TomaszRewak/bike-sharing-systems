from typing import List

import pickle

from data.model.time_predictions.time_prediction import TimePrediction


def unpickle_time_predictions(
        file_path: str
) -> List[TimePrediction]:
    with open(file_path, mode='rb') as file:
        return pickle.load(file)


def pickle_time_predictions(
        file_path: str,
        features: List[TimePrediction]
):
    with open(file_path, mode='wb+') as pickle_file:
        pickle.dump(features, pickle_file)
