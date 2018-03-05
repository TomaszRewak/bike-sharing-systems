from typing import List

from data.model.flow_matrix.flow_matrix import FlowMatrix


def write_flow_matrix(
        file_path: str,
        flow_matrices: List[FlowMatrix]
):
    with open(file_path, mode='w+') as file:
        file.write('{0}\n'.format(len(flow_matrices)))

        for flow_matrix in flow_matrices:
            file.write('{0:%d.%m.%Y}\n'.format(flow_matrix.date))

            file.write('{0}\n'.format(len(flow_matrix.stations)))

            for station_flows in flow_matrix.stations:
                file.write('{0}\n'.format(station_flows.station))

                file.write('{0}\n'.format(len(station_flows.connections)))

                for connection in station_flows.connections:
                    file.write('{0}\n'.format(connection.destination))

                    file.write('{0}\n'.format(len(connection.constraints)))

                    for constraint in connection.constraints:
                        file.write('{0:%H:%M} {1}\n'.format(
                            constraint.time,
                            constraint.flow_duration
                        ))
