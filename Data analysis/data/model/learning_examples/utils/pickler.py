from typing import List, Dict

import pickle

from data.model.learning_examples.learning_example import LearningExample


def unpickle_learning_examples(
        file_path: str
) -> List[Dict[str, Dict[str, LearningExample]]]:
    with open(file_path, mode='rb') as file:
        return pickle.load(file)


def pickle_learning_examples(
        file_path: str,
        learning_examples: List[Dict[str, Dict[str, LearningExample]]]
):
    with open(file_path, mode='wb+') as pickle_file:
        pickle.dump(learning_examples, pickle_file)
