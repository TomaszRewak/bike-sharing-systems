from statistics import mean
from typing import Dict, List

import numpy as np

from data.model.distance_functions.day_distance import DayDistance
from data.model.distance_functions.distance_function import DistanceFunction
from data.model.learning_examples.learning_example import LearningExample
from data.processing.learning.function_learning import learn_function


def process_distance_functions(learning_examples: List[Dict[str, Dict[str, LearningExample]]],
                               all_days: List[str], test_days: List[str],
                               distance_predictor_builder,
                               distance_predictor_applier) -> List[DistanceFunction]:
    distance_functions = []

    for node in learning_examples:
        train_set = [
            second_day_example
            for first_day, first_day_examples in node.items()
            for second_day, second_day_example in first_day_examples.items()
            if first_day not in test_days and second_day not in test_days
        ]

        test_set = [
            second_day_example
            for first_day, first_day_examples in node.items()
            for second_day, second_day_example in first_day_examples.items()
            if first_day in test_days or second_day in test_days
        ]

        distance_functions.append(
            learn_function(train_set, test_set, distance_predictor_builder, distance_predictor_applier)
        )

    return [
        DistanceFunction(
            day,
            [
                DayDistance(
                    other_day,
                    [
                        distance_predictor_applier(
                            distance_function,
                            np.array([learning_examples[node][day][other_day].features])
                        )[0]
                        for node, distance_function in enumerate(distance_functions)
                    ]
                )
                for other_day in all_days
            ]
        )
        for day in test_days
    ]
