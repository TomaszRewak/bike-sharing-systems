from datetime import datetime, timedelta, date
from typing import List

import sys

from data.model.flow_matrix.flow_matrix import FlowMatrix
from data.model.time_matrix.time_matrix import TimeMatrix
from data.model.time_matrix.time_matrix_frame import TimeMatrixFrame


def process_time_matrix_frame_row(station: int, time_frame: datetime.time, flow_matrix: FlowMatrix) -> List[int]:
    stations = len(flow_matrix.stations)

    distances = [
        sys.maxsize
        for _ in range(stations)
    ]

    distances[station] = 0

    current_nodes = {station}

    while len(current_nodes) > 0:
        current_node = min(current_nodes, key=lambda node: distances[node])
        current_nodes.remove(current_node)

        current_node_time = (datetime.combine(date.today(), time_frame) + timedelta(seconds=distances[current_node])).time()

        for connection in flow_matrix.stations[current_node].connections:
            flow_duration = connection.get_duration(current_node_time)

            if distances[connection.destination] > distances[current_node] + flow_duration:
                distances[connection.destination] = distances[current_node] + flow_duration
                current_nodes.add(connection.destination)

    return distances


def process_time_matrix_frame(time_frame: datetime, flow_matrix: FlowMatrix) -> TimeMatrixFrame:
    stations = len(flow_matrix.stations)

    return TimeMatrixFrame(time_frame, [
        process_time_matrix_frame_row(station, time_frame, flow_matrix)
        for station in range(stations)
    ])


def process_time_matrix(flow_matrix: FlowMatrix) -> TimeMatrix:
    time_frames = {
        constrain.time
        for station, station_flow in flow_matrix.stations.items()
        for connection in station_flow.connections
        for constrain in connection.constraints
    }

    time_frames = sorted(list(time_frames))

    return TimeMatrix(
        flow_matrix.date,
        [
            process_time_matrix_frame(time_frame, flow_matrix)
            for time_frame in time_frames
        ]
    )


def process_time_matrices(flow_matrices: List[FlowMatrix]) -> List[TimeMatrix]:
    return [
        process_time_matrix(flowMatrix)
        for flowMatrix in flow_matrices
    ]
