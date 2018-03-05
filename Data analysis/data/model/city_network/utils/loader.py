import networkx
import osmnx


def load_city_network(
        folder: str,
        path: str
) -> networkx.multidigraph:
    return osmnx.load_graphml(path, folder)


def save_city_network(
        folder: str,
        path: str,
        graph: networkx.multidigraph
):
    osmnx.save_graphml(graph, path, folder)
