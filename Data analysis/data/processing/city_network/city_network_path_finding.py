from typing import List

import networkx

from data.model.city_network.path import Path, PathSegment


def get_paths(graph: networkx.MultiDiGraph, source: int, targets: List[int]) -> List[Path]:
    targets = [target for target in targets if target is not source]

    paths = []
    for target in targets:
        path = networkx.shortest_path(graph, source, target, weight='length')

        segments = [
            PathSegment(
                (begin, end),
                graph[begin][end][0]['length']
            )
            for begin, end in zip(path[:-1], path[1:])
        ]

        paths.append(
            Path(
                target,
                segments
            )
        )

    return paths
