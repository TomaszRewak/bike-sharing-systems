from typing import List

from data.model.features.features import Features


class LearningExample:
    def __init__(self,
                 diff_value: float,
                 features: List[float]):
        self.diff_value = diff_value
        self.features = features
