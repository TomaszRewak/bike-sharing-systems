from typing import List

import pickle

from data.model.learning_examples.learning_example import LearningExample


def unpickle_learning_examples(
        file_path: str
) -> List[LearningExample]:
    with open(file_path, mode='rb') as file:
        return pickle.load(file)


def pickle_learning_examples(
        file_path: str,
        features: List[LearningExample]
):
    with open(file_path, mode='wb+') as pickle_file:
        pickle.dump(features, pickle_file)
