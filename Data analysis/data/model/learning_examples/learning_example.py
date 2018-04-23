from typing import List

from data.model.features.features import Features


class LearningExample:
    def __init__(self,
                 value: float,
                 features: List[float]):
        self.value = value
        self.features = features
