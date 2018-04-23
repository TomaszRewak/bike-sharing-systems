from typing import List

from sklearn.linear_model import LinearRegression, SGDRegressor


def learn_sgd_function(x: List[List[float]], y: List[float]):
    network = SGDRegressor()
    network.fit(x, y)

    return network


def learn_sgd_function_config():
    return lambda x, y: learn_sgd_function(x, y)


def apply_sgd_function(network, x: List[List[float]]) -> List[float]:
    return network.predict(x)
