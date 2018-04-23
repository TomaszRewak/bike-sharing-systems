from typing import List

from sklearn.linear_model import LinearRegression


def learn_linear_function(x: List[List[float]], y: List[float]):
    network = LinearRegression()
    network.fit(x, y)

    return network


def learn_linear_function_config():
    return lambda x, y: learn_linear_function(x, y)


def apply_linear_function(network, x: List[List[float]]) -> List[float]:
    return network.predict(x)
