from typing import List, Dict

import pickle

from data.model.learning_examples.learning_example import LearningExample
from data.model.learning_examples.learning_example_group import LearningExampleGroup


def unpickle_learning_examples(
        file_path: str
) -> Dict[str, LearningExampleGroup]:
    with open(file_path, mode='rb') as file:
        return pickle.load(file)


def pickle_learning_examples(
        file_path: str,
        features: Dict[str, LearningExampleGroup]
):
    with open(file_path, mode='wb+') as pickle_file:
        pickle.dump(features, pickle_file)
