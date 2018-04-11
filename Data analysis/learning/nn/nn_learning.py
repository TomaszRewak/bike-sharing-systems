from typing import List

from sklearn.neural_network import MLPRegressor


def learn_distance_nn_function(x: List[List[float]], y: List[float]):
    network = MLPRegressor(hidden_layer_sizes=(20,), random_state=1, activation='tanh')
    network.fit(x, y)

    return network


def apply_distance_nn_function(network, x: List[List[float]]) -> List[float]:
    return network.predict(x)
