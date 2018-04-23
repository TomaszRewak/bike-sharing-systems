from typing import List

import numpy as np

from data.model.learning_examples.learning_example import LearningExample


def prepare_learning_sets(examples: List[LearningExample]):
    return (
        np.array([
            example.features
            for example in examples
        ]),
        np.array([
            example.value
            for example in examples
        ])
    )


def learn_function(train_examples: List[LearningExample],
                   test_examples: List[LearningExample],
                   distance_predictor_builder,
                   distance_predictor_applier):
    (x_train, y_train) = prepare_learning_sets(train_examples)
    (x_test, y_test) = prepare_learning_sets(test_examples)

    predictor = distance_predictor_builder(x_train, y_train)

    y_train_prediction = distance_predictor_applier(predictor, x_train)
    y_test_prediction = distance_predictor_applier(predictor, x_test)

    error_train = np.mean(np.power(np.array(y_train) - np.array(y_train_prediction), 2))
    error_test = np.mean(np.power(np.array(y_test) - np.array(y_test_prediction), 2))

    print(str(error_train) + '\t' + str(error_test))

    return predictor
