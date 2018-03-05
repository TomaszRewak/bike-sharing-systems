from typing import List

import googlemaps
from googlemaps import distance_matrix

from data.model.time_predictions.time_prediction import TimePrediction
from data.model.time_predictions.time_prediction_request import TimePredictionRequest


def download_time_predictions(requests: List[TimePredictionRequest]):
    api_key = 'AIzaSyD_iwBsgQAqVYV4X89hy9oZRbq1cwXaO00'
    googlemaps_client = googlemaps.Client(api_key)

    predictions = []

    for request in requests:
        response = distance_matrix.distance_matrix(
            googlemaps_client,
            (request.origin.latitude, request.origin.longitude),
            (request.destination.latitude, request.destination.longitude),
            mode="driving",
            units="metric",
            departure_time=request.time
        )

        predictions.append(TimePrediction(
            request.origin.name,
            request.destination.name,
            response['rows'][0]['elements'][0]['duration_in_traffic']['value'],
            request.time
        ))

    return predictions
