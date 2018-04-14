from typing import Dict, List
from random import shuffle

from data.model.distance_functions.distance_function import DistanceFunction
from data.model.features.features import Features
from data.model.learning_examples.learning_example import LearningExample


def process_learning_examples(
        distance_functions: List[DistanceFunction],
        features: Dict[str, Features],
        vector_joiner
) -> List[Dict[str, Dict[str, LearningExample]]]:
    nodes = 74

    return [
        {
            distance_function.day: {
                day_distance.day: LearningExample(
                    day_distance.distances[node],
                    vector_joiner(features[distance_function.day].vector(), features[day_distance.day].vector()),
                )
                for day_distance in distance_function.distances
            }
            for distance_function in distance_functions
        }
        for node in range(nodes)
    ]
