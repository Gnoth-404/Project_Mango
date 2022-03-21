import numpy as np
import pandas as pd
from sklearn.cross_decomposition import PLSRegression
import os.path


def predict(nir):
    data = pd.read_csv(os.path.dirname(__file__) + "/../resource/Banana.csv")
    data = data.T.iloc[1:]
    X = data.drop(288, axis=1).values
    Y = data[288].values
    pls = PLSRegression(n_components=2)
    pls.fit(X, Y)
    nir_list = nir.split(",")
    if len(nir_list) > 288:
        del nir_list[288]
    nir_data = np.reshape(list(map(int, nir_list)), (1, 288))
    return pls.predict(nir_data).item(0)
