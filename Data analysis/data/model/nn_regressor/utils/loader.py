from sklearn.neural_network import MLPRegressor


def save_neural_network(
        file_path: str,
        network: MLPRegressor
):
    with open(file_path, mode='w+') as file:
        layer_sizes = [len(network.coefs_[0])] + list(network.hidden_layer_sizes) + [1]

        file.write(str(len(layer_sizes)) + '\n')
        file.write('\t'.join([str(layer_size) for layer_size in layer_sizes]) + '\n')

        for layer_coefficients in network.coefs_:
            for node_coefficients in layer_coefficients:
                file.write('\t'.join([str(coefficient) for coefficient in node_coefficients]) + '\n')

        for layer_intercepts in network.intercepts_:
            file.write('\t'.join([str(intercept) for intercept in layer_intercepts]) + '\n')
