from typing import Dict

from data.model.features.offline_features import OfflineFeatures


def save_offline_features(
        file_path: str,
        features: Dict[str, OfflineFeatures]
):
    with open(file_path, mode='w+') as file:
        file.write(str(len(features)) + ' ' + str(len(next(iter(features.values())).vector())) + '\n')

        for date, feature_values in features.items():
            values = '\t'.join([
                str(value)
                for value in feature_values.vector()
            ])

            file.write(date + '\t' + values + '\n')
