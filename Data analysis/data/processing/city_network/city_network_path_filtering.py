from typing import List

from data.model.city_network.path import Path


def has_shorter_version(
        path: Path,
        paths: List[Path]
) -> bool:
    used_segments = set([s.index for s in path.segments])

    for second_path in paths:
        if second_path is path or path.distance < second_path.distance:
            continue

        common_segments = [s for s in second_path.segments if s.index in used_segments]
        common_distance = sum([s.distance for s in common_segments]) / second_path.distance

        if common_distance > 0.2:
            return True

    return False


def filter_common_paths(paths: List[Path]) -> List[Path]:
    paths = sorted(paths, key=lambda p: p.distance)

    return [
        path
        for path in paths
        if not has_shorter_version(path, paths)
    ]
