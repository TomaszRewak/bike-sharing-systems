from statistics import mean
from typing import Dict, List

import numpy as np

from data.model.distance_functions.day_distance import DayDistance
from data.model.distance_functions.distance_function import DistanceFunction
from data.model.learning_examples.learning_example import LearningExample


def prepare_learning_sets(examples: List[LearningExample]):
    return (
        np.array([
            example.features
            for example in examples
        ]),
        np.array([
            example.diff_value
            for example in examples
        ])
    )


def learn_distance_function(train_examples: List[LearningExample],
                            test_examples: List[LearningExample],
                            distance_predictor_builder,
                            distance_predictor_applier):
    (x_train, y_train) = prepare_learning_sets(train_examples)
    (x_test, y_test) = prepare_learning_sets(test_examples)

    distance_predictor = distance_predictor_builder(x_train, y_train)

    y_train_prediction = distance_predictor_applier(distance_predictor, x_train)
    y_test_prediction = distance_predictor_applier(distance_predictor, x_test)

    error_train = np.mean(np.absolute(np.array(y_train) - np.array(y_train_prediction)))
    error_test = np.mean(np.absolute(np.array(y_test) - np.array(y_test_prediction)))

    print(str(error_train) + '\t' + str(error_test))

    return distance_predictor


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
            learn_distance_function(train_set, test_set, distance_predictor_builder, distance_predictor_applier)
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
