from typing import List

from data.model.learning_examples.learning_example import LearningExample


class LearningExampleGroup:
    def __init__(self,
                 train_examples: List[LearningExample],
                 test_examples: List[LearningExample]):
        self.train_examples = train_examples
        self.test_examples = test_examples
