from typing import List

import numpy as np


class StationPrediction:
    def __init__(self,
                 predicted_values: np.array):
        self.predicted_values = predicted_values
