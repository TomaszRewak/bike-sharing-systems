import argparse
from datetime import datetime
from shutil import copyfile

from os import path

from data.model.city_network.utils.downloader import download_city_network
from data.model.city_network.utils.loader import save_city_network, load_city_network
from data.model.day.utils.loader import load_days
from data.model.distance_functions.utils.loader import save_distance_functions, load_distance_functions
from data.model.features.utils.pickler import pickle_features, unpickle_features
from data.model.learning_examples.utils.pickler import pickle_learning_examples, unpickle_learning_examples
from data.model.predictions.utils.loader import load_predictions
from data.model.raw_rides.utils.loader import load_raw_rides
from data.model.rides.utils.pickler import pickle_rides
from data.model.rides.utils.loader import save_rides
from data.model.stations.utils.loader import load_stations
from data.model.stations.utils.picker import pickle_station_connections, unpickle_station_connections
from data.model.time_matrix.utils.loader import write_time_matrices
from data.model.time_predictions.utils.downloader import download_time_predictions
from data.model.time_predictions.utils.pickler import pickle_time_predictions, unpickle_time_predictions
from data.model.weather.utils.loader import load_weather
from data.processing.city_network.city_network_simplifying import simplify_city_network
from data.processing.distance_functions.distance_functions_processing import process_distance_functions
from data.processing.features.features_analysis import analyse_features
from data.processing.flow_matrix.flow_matrix_processing import process_flow_matrices
from data.processing.learning_examples.learning_examples_processing import process_learning_examples
from data.processing.features.offline_features_processing import process_features
from data.processing.learning_examples.learning_examples_joining import concat_feature_vectors, \
    subtract_feature_vectors, mix_feature_vectors
from data.processing.rides.rides_processing import process_rides_data
from data.processing.time_matrix.time_matrix_processing import process_time_matrices
from data.processing.time_predictions.time_prediction_grouping import group_time_predictions
from data.processing.time_predictions.time_prediction_request_preparing import prepare_time_prediction_requests
from learning.nn.nn_learning import learn_distance_nn_function, apply_distance_nn_function
from utilities.city_network.city_network_rendering import render_city_network

parser = argparse.ArgumentParser(description='City bikes distribution')
parser.add_argument('--prepare_city_data',
                    help='Download city data',
                    action='store_true')
parser.add_argument('--prepare_simplified_city_data',
                    help='Prepares simplified map for further processing',
                    action='store_true')
parser.add_argument('--display_simplified_city_data',
                    help='Displays simplified map',
                    action='store_true')
parser.add_argument('--prepare_rides_data',
                    help='Prepare examples from rides data',
                    action='store_true')
parser.add_argument('--prepare_features',
                    help='Prepare feature values for each day',
                    action='store_true')
parser.add_argument('--analyse_feature_set',
                    help='Analyse feature values',
                    action='store_true')
parser.add_argument('--prepare_learning_examples',
                    help='Prepare learning examples',
                    action='store_true')
parser.add_argument('--learn_nn_distance_function',
                    help='Learn distance function based on NN',
                    action='store_true')
parser.add_argument('--get_time_predictions',
                    help='Get time predictions from Google API',
                    action='store_true')
parser.add_argument('--analyse_time_predictions',
                    help='Analyse time predictions',
                    action='store_true')
parser.add_argument('--prepare_flow_matrix',
                    help='Prepare time prediction matrix',
                    action='store_true')


def prepare_city_data():
    city_network = download_city_network()
    save_city_network('../resources/raw/city/', 'wroclaw.graph', city_network)


def prepare_simplified_city_data():
    city_network = load_city_network('../resources/raw/city/', 'wroclaw.graph')
    stations = load_stations('../resources/raw/rides/stations.csv')

    simplified_city_network = simplify_city_network(city_network, stations)
    pickle_station_connections('./resources/pickled/selected_connections.pickle', simplified_city_network)


def display_simplified_city_data():
    city_network = load_city_network('../resources/raw/city/', 'wroclaw.graph')
    stations = load_stations('../resources/raw/rides/stations.csv')
    simplified_city_network = unpickle_station_connections('./resources/pickled/selected_connections.pickle')

    render_city_network(city_network, stations, simplified_city_network)


def prepare_rides_data():
    raw_rides = load_raw_rides('../resources/raw/rides/rides 2015.csv')
    stations = load_stations('../resources/raw/rides/stations.csv')

    rides = process_rides_data(raw_rides, stations)

    pickle_rides('./resources/pickled/rides.pickle', rides)
    save_rides('../resources/processed/rides.rides', rides)


def prepare_features():
    weather = load_weather('../resources/raw/weather/s_d_t_424_2015.csv')

    features = process_features(weather)

    pickle_features('./resources/pickled/features.pickle', features)


def analyse_feature_set():
    features = unpickle_features('./resources/pickled/features.pickle')

    analyse_features(features)


def prepare_learning_examples():
    distance_functions = load_distance_functions('../resources/processed/simple_demand_distance.distance')
    features = unpickle_features('./resources/pickled/features.pickle')

    learning_examples = process_learning_examples(distance_functions, features, subtract_feature_vectors)

    pickle_learning_examples('./resources/pickled/learning_examples.pickle', learning_examples)


def learn_nn_distance_function():
    learning_examples = unpickle_learning_examples('./resources/pickled/learning_examples.pickle')
    test_days = load_days('../resources/configuration/test_examples.days')
    all_days = load_days('../resources/configuration/all_examples.days')

    distance_functions = process_distance_functions(
        learning_examples,
        all_days, test_days,
        learn_distance_nn_function,
        apply_distance_nn_function
    )

    save_distance_functions('../resources/learning/nn.model', distance_functions)


def get_time_predictions():
    # 01 00 00
    # 03 00 00
    # 06 00 00
    # 07 00 00
    # 07 30 00
    # 08 00 00
    # 08 30 00
    # 09 00 00
    # 10 30 00
    # 12 00 00
    # 13 00 00
    # 14 00 00
    # 15 00 00
    # 15 30 00
    # 16 00 00
    # 16 30 00
    # 17 00 00
    # 17 30 00
    # 18 00 00
    # 19 30 00
    # 21 00 00
    # 23 00 00
    pickle_file = './resources/pickled/predictions.pickle'
    old_predictions = []

    if path.isfile(pickle_file):
        copyfile(pickle_file, pickle_file + '_old')
        old_predictions = unpickle_time_predictions(pickle_file)

    times = [
        datetime(2019, 6, 10, 23, 0, 0),
        datetime(2019, 6, 15, 23, 0, 0)
    ]

    stations = load_stations('../resources/raw/rides/stations.csv')
    simplified_city_network = unpickle_station_connections('./resources/pickled/selected_connections.pickle')

    requests = prepare_time_prediction_requests(stations, simplified_city_network, times)

    predictions = download_time_predictions(requests)

    pickle_time_predictions(pickle_file, predictions + old_predictions)


def analyse_time_predictions():
    time_predictions = unpickle_time_predictions('./resources/pickled/predictions.pickle')

    grouped_time_predictions = group_time_predictions(time_predictions)

    return grouped_time_predictions


def prepare_flow_matrix():
    time_predictions = unpickle_time_predictions('./resources/pickled/predictions.pickle')
    stations = load_stations('../resources/raw/rides/stations.csv')

    flow_matrices = process_flow_matrices(time_predictions, stations)
    time_matrices = process_time_matrices(flow_matrices)

    write_time_matrices('../resources/processed/time_matrices.time', time_matrices)


def main():
    args = parser.parse_args()

    if args.prepare_city_data:
        prepare_city_data()
    elif args.prepare_simplified_city_data:
        prepare_simplified_city_data()
    elif args.display_simplified_city_data:
        display_simplified_city_data()
    elif args.prepare_rides_data:
        prepare_rides_data()
    elif args.prepare_features:
        prepare_features()
    elif args.analyse_feature_set:
        analyse_feature_set()
    elif args.prepare_learning_examples:
        prepare_learning_examples()
    elif args.learn_nn_distance_function:
        learn_nn_distance_function()
    elif args.get_time_predictions:
        get_time_predictions()
    elif args.analyse_time_predictions:
        analyse_time_predictions()
    elif args.prepare_flow_matrix:
        prepare_flow_matrix()
    else:
        parser.print_help()

    pass


if __name__ == '__main__':
    main()
