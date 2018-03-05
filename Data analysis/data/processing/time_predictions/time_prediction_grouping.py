from typing import List

from data.model.time_predictions.time_prediction import TimePrediction


def group_time_predictions(time_predictions: List[TimePrediction]):
    return {
        prediction.origin + ' =to= ' + prediction.destination: sorted([
            selected_prediction
            for selected_prediction in time_predictions
            if selected_prediction.origin == prediction.origin and selected_prediction.destination == prediction.destination
        ], key=lambda x: x.start_time)
        for prediction in time_predictions
    }
