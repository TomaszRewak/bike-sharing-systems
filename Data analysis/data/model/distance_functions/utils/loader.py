from typing import List

from data.model.distance_functions.distance_function import DistanceFunction
from data.model.distance_functions.day_distance import DayDistance


def save_distance_functions(
        file_path: str,
        distance_functions: List[DistanceFunction]
):
    with open(file_path, mode='w+') as file:
        file.write(str(len(distance_functions)) + '\n')

        for distance_function in distance_functions:
            file.write(distance_function.day + ' ' + str(len(distance_function.distances)) + '\n')

            for distance in distance_function.distances:
                file.write(distance.day + ' ' + str(distance.distance) + '\n')


def load_distance_functions(
        file_path: str
) -> List[DistanceFunction]:
    with open(file_path, mode='r') as file:
        header_line = file.readline()

        days_number = int(header_line)

        result = []
        for _ in range(days_number):
            day_header_line = file.readline()

            [base_day, compared_days_number_str] = day_header_line.split()

            distances = []
            for _ in range(int(compared_days_number_str)):
                day_distance_line = file.readline()

                [compared_day, distance_str] = day_distance_line.split()

                distances.append(DayDistance(
                    compared_day,
                    float(distance_str)
                ))

            result.append(DistanceFunction(
                base_day,
                distances
            ))

        return result
