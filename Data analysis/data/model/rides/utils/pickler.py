from typing import List
import pickle

from data.model.rides.ride import Ride


def unpickle_rides(
        file_path: str
) -> List[Ride]:
    with open(file_path, mode='rb') as file:
        return pickle.load(file)


def pickle_rides(
        file_path: str,
        rides: List[Ride]
):
    with open(file_path, mode='wb+') as pickle_file:
        pickle.dump(rides, pickle_file)
