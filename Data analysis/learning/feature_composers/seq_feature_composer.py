import numpy as np

from data.model.learning_examples.learning_example import LearningExample


def compose_features_seq(learning_examples: LearningExample):
    return np.array(learning_examples.features_a.vector() + learning_examples.features_b.vector())
