from typing import List, Dict

from data.model.learning_examples.learning_example import LearningExample


class LearningExampleGroup:
    def __init__(self,
                 train_examples: List[LearningExample],
                 test_examples: List[LearningExample]):
        self.train_examples = train_examples
        self.test_examples = test_examples


def group_learning_examples(learning_examples: List[LearningExample]) -> Dict[str, LearningExampleGroup]:
    used_examples = {
        example
        for learning_example in learning_examples
        for example in learning_example.used_examples
    }

    return {
        example: LearningExampleGroup(
            [
                learning_example
                for learning_example in learning_examples if example not in learning_example.used_examples
            ],
            [
                learning_example
                for learning_example in learning_examples if example in learning_example.used_examples
            ]
        )
        for example in used_examples
    }
