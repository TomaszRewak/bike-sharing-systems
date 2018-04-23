from typing import List, Dict

import numpy as np

from data.model.demand.day_demand import DayDemand
from data.model.demand.day_demand_frame import DayDemandFrame
from data.model.learning_examples.learning_example import LearningExample
from data.processing.learning.function_learning import learn_function


def process_demand_function(learning_examples: List[Dict[str, List[LearningExample]]],
                            test_days: List[str],
                            demand_predictor_builder,
                            demand_predictor_applier) -> List[DayDemand]:
    demand_functions = []

    for node in learning_examples:
        train_set = [
            frame
            for day, frames in node.items()
            for frame in frames
            if day not in test_days
        ]

        test_set = [
            frame
            for day, frames in node.items()
            for frame in frames
            if day in test_days
        ]

        demand_functions.append(
            learn_function(train_set, test_set, demand_predictor_builder, demand_predictor_applier)
        )

    time_frames = len(list(learning_examples[0].values())[0])

    return [
        DayDemand(
            day,
            [
                DayDemandFrame(
                    time_frame / time_frames,
                    [
                        demand_predictor_applier(
                            demand_function,
                            np.array([learning_examples[node][day][time_frame].features])
                        )[0]
                        for node, demand_function in enumerate(demand_functions)
                    ]
                )
                for time_frame in range(time_frames)
            ]
        )
        for day in test_days
    ]
