from typing import List

import numpy as np

from data.model.learning_examples.learning_example import LearningExample


def process_regression_examples(learning_examples: List[LearningExample]) -> (List[List[float]], List[float]):
    return (
        [
            np.array(learning_example.features)
            for learning_example in learning_examples
        ],
        [
            learning_example.diff_value
            for learning_example in learning_examples
        ]
    )
