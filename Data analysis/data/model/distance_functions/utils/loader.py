from typing import List

from data.model.distance_functions.distance_function import DistanceFunction


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
