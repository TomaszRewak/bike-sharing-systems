from typing import List

from sklearn.neural_network import MLPRegressor


def learn_nn_function(x: List[List[float]], y: List[float], hidden_layer_sizes, max_iter, activation):
    network = MLPRegressor(
        hidden_layer_sizes=hidden_layer_sizes,
        max_iter=max_iter,
        activation=activation,
        early_stopping=True)
    network.fit(x, y)

    #print('loss')
    #for value in network.loss_curve_:
    #    print('{}'.format(value))

    return network


def learn_nn_function_config(hidden_layer_sizes, max_iter, activation):
    return lambda x, y: learn_nn_function(x, y, hidden_layer_sizes, max_iter, activation)


def apply_nn_function(network, x: List[List[float]]) -> List[float]:
    return network.predict(x)
