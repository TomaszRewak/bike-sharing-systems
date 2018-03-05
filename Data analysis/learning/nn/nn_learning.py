from typing import List

import numpy as np
from sklearn.neural_network import MLPRegressor

from data.model.learning_examples.learning_example import LearningExample
from learning.feature_composers.seq_feature_composer import compose_features_seq


def learn_distance_function(
        train_examples: List[LearningExample],
        test_examples: List[LearningExample]
):
    x_train = [compose_features_seq(learning_example) for learning_example in train_examples]
    y_train = [learning_example.diff_value for learning_example in train_examples]

    network = MLPRegressor(hidden_layer_sizes=(40,), random_state=1, max_iter=1000, activation='relu')
    network.fit(x_train, y_train)

    x_test = [compose_features_seq(learning_example) for learning_example in test_examples]
    y_test = [learning_example.diff_value for learning_example in test_examples]

    diff_train = np.mean((np.array(y_train) - np.array(network.predict(x_train)))**2)
    diff_test = np.mean((np.array(y_test) - np.array(network.predict(x_test)))**2)
    print(str(diff_train) + ', ' + str(diff_test) + '\n')

    # test_values = [np.array([0.2, 13., 3.3, 0.2, 6., 0.12, 4., 0.1, 0.3, 2.1, 0.4, 3.1, 0.5, 6.2, 2.7, 0.2])]
    # test_value = network.predict(test_values)
    # print(test_value[0])

    return network
