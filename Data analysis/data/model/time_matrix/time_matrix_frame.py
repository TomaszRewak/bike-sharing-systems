from datetime import datetime
from typing import List


class TimeMatrixFrame:
    def __init__(self,
                 date: datetime,
                 matrix: List[List[int]]):
        self.date = date
        self.matrix = matrix
