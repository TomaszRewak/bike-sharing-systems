from typing import Dict, List

from data.model.distance_functions.distance_function import DistanceFunction
from data.model.features.features import Features
from data.model.learning_examples.learning_example import LearningExample


def process_learning_examples(
        distance_functions: List[DistanceFunction],
        features: Dict[str, Features]
) -> List[LearningExample]:

    return [
        LearningExample(
            day_distance.distance,
            features[distance_function.day],
            features[day_distance.day],
            [distance_function.day, day_distance.day]
        )
        for distance_function in distance_functions
        for day_distance in distance_function.distances
    ]

