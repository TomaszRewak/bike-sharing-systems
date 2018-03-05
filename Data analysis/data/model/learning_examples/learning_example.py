from typing import List

from data.model.features.features import Features


class LearningExample:
    def __init__(self,
                 diff_value: float,
                 features_a: Features,
                 features_b: Features,
                 used_examples: List[str]):
        self.diff_value = diff_value
        self.features_a = features_a
        self.features_b = features_b
        self.used_examples = used_examples
