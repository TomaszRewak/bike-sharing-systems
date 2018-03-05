from data.model.features.offline_features import OfflineFeatures
from data.model.features.online_features import OnlineFeatures


class Features:
    def __init__(self,
                 offline_features: OfflineFeatures,
                 online_features: OnlineFeatures):
        self.offline_features = offline_features
        self.online_features = online_features

    def vector(self):
        return self.offline_features.vector() + self.online_features.vector()
