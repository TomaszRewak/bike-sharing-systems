from typing import List

from sklearn.linear_model import LinearRegression
from sklearn.svm import SVR


def learn_svr_function(x: List[List[float]], y: List[float], max_iter, kernel: str, c: float):
    network = SVR(max_iter=max_iter, kernel=kernel, degree=5, C=c)
    network.fit(x, y)

    return network


def learn_svr_function_config(max_iter: int, kernel: str, c: float):
    return lambda x, y: learn_svr_function(x, y, max_iter, kernel, c)


def apply_svr_function(network, x: List[List[float]]) -> List[float]:
    return network.predict(x)
