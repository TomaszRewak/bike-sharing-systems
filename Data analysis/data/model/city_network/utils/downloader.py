import osmnx


def download_city_network(location='Wrocław, Poland'):
    return osmnx.graph_from_place(location, network_type='drive')
