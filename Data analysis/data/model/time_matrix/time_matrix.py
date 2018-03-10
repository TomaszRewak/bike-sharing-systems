from datetime import datetime
from typing import List

from data.model.time_matrix.time_matrix_frame import TimeMatrixFrame


class TimeMatrix:
    def __init__(self,
                 date: datetime,
                 frames: List[TimeMatrixFrame]):
        self.date = date
        self.frames = frames
