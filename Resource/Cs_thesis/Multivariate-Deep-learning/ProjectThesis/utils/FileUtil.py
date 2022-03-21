import os
from django.core.files.base import ContentFile


path = os.path.dirname(__file__) + "/../resource/"


def get_list_model():
    files = []
    # r=root, d=directories, f = files
    for r, d, f in os.walk(path):
        for file in f:
            if '.h5' in file:
                files.append(file[:-3])
    return files


def save_model_file(modelName, dataFile, modelFile, weightFile, modelType):
    type = ""
    if modelType == "regression":
        type = "REG"
    else:
        type = "CLS"
    with open(path + modelName + '_' + type + '.csv', 'wb') as f:
        f.write(dataFile.read())
    with open(path + modelName + '_' + type + '.json', 'wb') as f:
        f.write(modelFile.read())
    with open(path + modelName + '_' + type + '.h5', 'wb') as f:
        f.write(weightFile.read())


def delete_model(modelName):
    os.remove(path + modelName + '.csv')
    os.remove(path + modelName + '.json')
    os.remove(path + modelName + '.h5')

