from typing import List

from sklearn.ensemble import RandomForestRegressor
from sklearn.linear_model import LinearRegression
from sklearn.tree import DecisionTreeRegressor


def learn_random_forest_function(x: List[List[float]], y: List[float], min_samples_split: float):
    network = RandomForestRegressor(min_samples_split=min_samples_split)
    network.fit(x, y)

    return network


def learn_random_forest_function_config(min_samples_split: float):
    return lambda x, y: learn_random_forest_function(x, y, min_samples_split)


def apply_random_forest_function(network, x: List[List[float]]) -> List[float]:
    return network.predict(x)
