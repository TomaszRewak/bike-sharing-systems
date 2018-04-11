from statistics import mean
from typing import Dict, List

import numpy as np

from data.model.distance_functions.day_distance import DayDistance
from data.model.distance_functions.distance_function import DistanceFunction
from data.processing.features.regression_examples_processing import process_regression_examples
from data.processing.learning_examples.learning_examples_grouping import LearningExampleGroup


def process_distance_function(day: str,
                              learning_examples: LearningExampleGroup,
                              distance_predictor_builder,
                              distance_predictor_applier) -> DistanceFunction:

    (x_test, y_test) = process_regression_examples(learning_examples.test_examples)
    (x_train, y_train) = process_regression_examples(learning_examples.train_examples)

    distance_predictor = distance_predictor_builder(x_train, y_train)

    y = distance_predictor_applier(distance_predictor, x_test)

    diff_test = np.mean(np.absolute(np.array(y_test) - np.array(y)))
    print(day + ': ' + str(diff_test) + '\n')

    zipped_results = list(zip(learning_examples.test_examples, y))

    other_days = {
        used_example
        for test_example in learning_examples.test_examples
        for used_example in test_example.used_examples
        if used_example != day
    }

    return DistanceFunction(
        day,
        [
            DayDistance(
                other_day,
                mean([
                    value
                    for learning_example, value in zipped_results
                    if other_day in learning_example.used_examples
                ])
            )
            for other_day in other_days
        ]
    )


def process_distance_functions(grouped_learning_examples: Dict[str, LearningExampleGroup],
                               distance_predictor_builder,
                               distance_predictor_applier) -> List[DistanceFunction]:
    return [
        process_distance_function(
            day,
            group,
            distance_predictor_builder,
            distance_predictor_applier
        )
        for day, group in grouped_learning_examples.items()
    ]
