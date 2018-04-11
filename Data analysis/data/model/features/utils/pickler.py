from typing import Dict

import pickle

from data.model.features.features import Features


def unpickle_features(
        file_path: str
) -> Dict[str, Features]:
    with open(file_path, mode='rb') as file:
        return pickle.load(file)


def pickle_features(
        file_path: str,
        features: Dict[str, Features]
):
    with open(file_path, mode='wb+') as pickle_file:
        pickle.dump(features, pickle_file)
