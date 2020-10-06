from ProjectThesis.utils.ChemUtils import GlobalStandardScaler, EmscScaler
from keras.models import model_from_json
import pandas as pd
import numpy as np
import os.path
from keras import backend as K


def predictREG(nir, model):
    K.clear_session()
    data = pd.read_csv(os.path.dirname(__file__) + "/../resource/" + model + ".csv")
    unit = data.columns[288]
    features = np.array(data.values[:, 0:288], dtype=np.int32)
    featuresscaler = GlobalStandardScaler()
    featuresscaler.fit_transform(features)
    label = np.array(data.values[:, -1], dtype=np.int32)
    labelscaler = GlobalStandardScaler()
    labelscaler.fit_transform(label)
    json_file = open(os.path.dirname(__file__) + '/../resource/' + model + '.json', 'r')
    loaded_model_json = json_file.read()
    json_file.close()
    loaded_model = model_from_json(loaded_model_json)
    # load weights into new model
    loaded_model.load_weights(os.path.dirname(__file__) + "/../resource/" + model + ".h5")
    nir_list = nir.split(",")
    if len(nir_list) > 288:
        del nir_list[288]
    nir_data = np.reshape(featuresscaler.transform(list(map(int, nir_list))), (1, 288))
    predicted_value = np.squeeze(labelscaler.inverse_transform(loaded_model.predict(nir_data))).item(0)
    K.clear_session()
    return predicted_value, unit


def predictCLS(nir, model):
    K.clear_session()
    data = pd.read_csv(os.path.dirname(__file__) + "/../resource/" + model + ".csv")
    unit = data.columns[288]
    features = np.array(data.values[:, 0:288], dtype=np.int32)
    featuresscaler = GlobalStandardScaler()
    featuresscaler.fit_transform(features)
    emsc = EmscScaler()
    emsc.fit(features)
    labels = data.iloc[:, 288].unique()
    json_file = open(os.path.dirname(__file__) + '/../resource/' + model + '.json', 'r')
    loaded_model_json = json_file.read()
    json_file.close()
    loaded_model = model_from_json(loaded_model_json)
    # load weights into new model
    loaded_model.load_weights(os.path.dirname(__file__) + "/../resource/" + model + ".h5")
    nir_list = nir.split(",")
    if len(nir_list) > 288:
        del nir_list[288]
    nir_data = emsc.transform(featuresscaler.transform(list(map(int, nir_list))).reshape(1, 288))
    a = loaded_model.predict(nir_data)
    index = np.argmax(a)
    if index == labels.size - 1:
        index = -1
    K.clear_session()
    return labels[index + 1], unit


def filePredict(file, model):
    K.clear_session()
    data = pd.read_csv(os.path.dirname(__file__) + "/../resource/" + model + ".csv")
    labels = data.iloc[:, data.shape[1] - 1].unique()
    data_file = pd.read_csv(file)
    json_file = open(os.path.dirname(__file__) + '/../resource/' + model + '.json', 'r')
    loaded_model_json = json_file.read()
    json_file.close()
    loaded_model = model_from_json(loaded_model_json)
    # load weights into new model
    loaded_model.load_weights(os.path.dirname(__file__) + "/../resource/" + model + ".h5")
    array = []
    for i in range(data_file.shape[0]):

        a = loaded_model.predict(data_file.iloc[[i]])
        index = np.argmax(a)
        if index == labels.size - 1:
            index = -1
        array.append(labels[index + 1])
    K.clear_session()
    result = np.asarray(array)
    pd.DataFrame(result).to_csv("result.csv")
