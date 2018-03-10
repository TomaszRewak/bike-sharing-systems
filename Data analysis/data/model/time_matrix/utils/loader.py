from typing import List

from data.model.time_matrix.time_matrix import TimeMatrix


def write_time_matrices(
        file_path: str,
        time_matrices: List[TimeMatrix]
):
    with open(file_path, mode='w+') as file:
        file.write('{0}\n'.format(len(time_matrices)))

        for time_matrix in time_matrices:
            file.write('{0:%d.%m.%Y} {1}\n'.format(time_matrix.date, len(time_matrix.frames)))

            for time_matrix_frame in time_matrix.frames:
                file.write('{0:%H:%M} {1}\n'.format(time_matrix_frame.date, len(time_matrix_frame.matrix)))

                for time_matrix_frame_row in time_matrix_frame.matrix:
                    for distance in time_matrix_frame_row:
                        file.write('{0:<5} '.format(round(distance)))
                    file.write('\n')
