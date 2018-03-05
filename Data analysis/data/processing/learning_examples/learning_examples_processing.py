from typing import Dict, List

from data.analysis.prediction_analysis import analyse_predictions_difference
from data.model.features.features import Features
from data.model.features.offline_features import OfflineFeatures
from data.model.features.online_features import OnlineFeatures
from data.model.learning_examples.learning_example import LearningExample
from data.model.predictions.prediction import Prediction


def process_learning_examples(
        predictions: Dict[str, Prediction],
        offline_features: Dict[str, OfflineFeatures]
) -> List[LearningExample]:
    prediction_items = list(predictions.items())

    return [
        LearningExample(
            analyse_predictions_difference(prediction_a, prediction_b),
            Features(offline_features[date_a], OnlineFeatures()),
            Features(offline_features[date_b], OnlineFeatures()),
            [date_a, date_b]
        )
        for date_a, prediction_a in prediction_items
        for date_b, prediction_b in prediction_items
    ]
