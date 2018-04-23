from typing import List

from data.model.demand.day_demand import DayDemand
from data.model.demand.day_demand_frame import DayDemandFrame


def save_demand(file_path: str, day_demands: List[DayDemand]):
    with open(file_path, mode='w') as file:
        file.write('{0} {1}\n'.format(
            len(day_demands),
            len(day_demands[0].frames[0].values)
        ))

        for day_demand in day_demands:
            file.write('{0} {1}\n'.format(
                day_demand.day,
                len(day_demand.frames)
            ))

            for frame in day_demand.frames:
                file.write('{0:02}:{1:02} '.format(
                    int(24 * frame.time),
                    int(1440 * frame.time % 60)
                ))

                for value in frame.values:
                    file.write('{} '.format(value))

                file.write('\n')


def load_demand(file_path: str) -> List[DayDemand]:
    with open(file_path, mode='r') as file:
        [days_str, _] = file.readline().split()

        days = int(days_str)

        demands = []

        for _ in range(days):
            [day, frames_str] = file.readline().split()

            frames = int(frames_str)
            day_frames = []

            for _ in range(frames):
                frame = file.readline().split()

                [frame_time_hour, frame_time_minute] = [float(value) for value in frame[0].split(':')]

                frame_time = frame_time_hour / 24. + frame_time_minute / 1440
                frame_values = [float(value) for value in frame[1:]]

                day_frames.append(DayDemandFrame(
                    frame_time,
                    frame_values
                ))

            demands.append(DayDemand(
                day,
                day_frames
            ))

        return demands
