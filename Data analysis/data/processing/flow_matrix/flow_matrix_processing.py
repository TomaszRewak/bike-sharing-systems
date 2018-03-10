from typing import List, Dict

from data.model.flow_matrix.flow_connection import FlowConnection
from data.model.flow_matrix.flow_constraint import FlowConstraint
from data.model.flow_matrix.flow_matrix import FlowMatrix
from data.model.flow_matrix.flow_station import FlowStation
from data.model.stations.station import Station
from data.model.time_predictions.time_prediction import TimePrediction


def process_flow_constraints(time_predictions: List[TimePrediction]) -> List[FlowConstraint]:
    return sorted([
        FlowConstraint(
            time_prediction.start_time.time(),
            time_prediction.duration
        )
        for time_prediction in time_predictions
    ], key=lambda x: x.time)


def process_flow_station(time_predictions: List[TimePrediction], stations: List[Station]) -> List[FlowConnection]:
    station_mapping = {
        station.name: station
        for station in stations
    }

    reachable_stations = {
        station_mapping[time_prediction.destination]
        for time_prediction in time_predictions
    }

    prediction_groups = {
        reachable_station.index: [
            time_prediction
            for time_prediction in time_predictions
            if time_prediction.destination == reachable_station.name
        ]
        for reachable_station in reachable_stations
    }

    return [
        FlowConnection(
            station_index,
            process_flow_constraints(prediction_group)
        )
        for station_index, prediction_group in prediction_groups.items()
    ]


def process_flow_matrix(time_predictions: List[TimePrediction], stations: List[Station]) -> Dict[int, FlowStation]:
    prediction_groups = {
        station.index: [
            time_prediction
            for time_prediction in time_predictions
            if time_prediction.origin == station.name
        ]
        for station in stations
    }

    return {
        station_index: FlowStation(
            process_flow_station(prediction_group, stations)
        )
        for station_index, prediction_group in prediction_groups.items()
    }


def process_flow_matrices(time_predictions: List[TimePrediction], stations: List[Station]) -> List[FlowMatrix]:
    prediction_days = {
        time_prediction.start_time.date()
        for time_prediction in time_predictions
    }

    prediction_groups = {
        prediction_day: [
            time_prediction
            for time_prediction in time_predictions
            if time_prediction.start_time.date() == prediction_day
        ]
        for prediction_day in prediction_days
    }

    return [
        FlowMatrix(
            day,
            process_flow_matrix(prediction_group, stations)
        )
        for day, prediction_group in prediction_groups.items()
    ]
