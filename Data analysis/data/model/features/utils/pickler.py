from typing import Dict

import pickle

from data.model.features.offline_features import OfflineFeatures


def unpickle_offline_features(
        file_path: str
) -> Dict[str, OfflineFeatures]:
    with open(file_path, mode='rb') as file:
        return pickle.load(file)


def pickle_offline_features(
        file_path: str,
        features: Dict[str, OfflineFeatures]
):
    with open(file_path, mode='wb+') as pickle_file:
        pickle.dump(features, pickle_file)
