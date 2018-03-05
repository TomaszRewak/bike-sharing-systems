from typing import List


class PathSegment:
    def __init__(self, index: (int, int), distance: float):
        self.index = index
        self.distance = distance


class Path:
    def __init__(self, to: int, segments: List[PathSegment]):
        self.to = to
        self.segments = segments
        self.distance = sum([segment.distance for segment in segments])
