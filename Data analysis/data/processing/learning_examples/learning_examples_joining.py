from typing import List

import numpy as np

from data.model.learning_examples.learning_example import LearningExample


def concat_feature_vectors(vector_a: List[float], vector_b: List[float]):
    return vector_a + vector_b


def subtract_feature_vectors(vector_a: List[float], vector_b: List[float]):
    return list(np.array(vector_a) - np.array(vector_b))


def mix_feature_vectors(vector_a: List[float], vector_b: List[float]):
    return vector_a + subtract_feature_vectors(vector_b, vector_a)
