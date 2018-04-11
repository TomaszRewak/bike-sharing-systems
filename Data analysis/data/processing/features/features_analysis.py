from typing import List, Dict
from pprint import pprint

import numpy as np

from data.model.features.features import Features


def analyse_features(features: Dict[str, Features]):
    values = [
        feature.vector()
        for _, feature in features.items()
    ]

    min_values = np.min(values, axis=0)
    max_values = np.max(values, axis=0)

    pprint(min_values)
    pprint(max_values)
