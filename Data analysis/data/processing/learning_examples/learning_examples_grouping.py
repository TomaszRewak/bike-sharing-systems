from typing import List, Dict

from data.model.learning_examples.learning_example import LearningExample
from data.model.learning_examples.learning_example_group import LearningExampleGroup


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
