# ProjectThesis/views.py
from rest_framework.parsers import JSONParser
from .prediction import PartialLeastSquareModel
from .prediction import NeuralNetworkModel
from django.http import HttpResponse
from django.views.generic import TemplateView
from rest_framework.decorators import api_view, parser_classes
from ProjectThesis import Constant
from .utils import FileUtil
from django.http import JsonResponse
import json


class HomePageView(TemplateView):
    template_name = 'home.html'


class AboutPageView(TemplateView):
    template_name = 'about.html'


@api_view(['GET', 'POST'])
def plsRegression(request):
    if request.method == 'POST':
        result = PartialLeastSquareModel.predict(request.data[Constant.FIELD_NIR])
        return HttpResponse(result)
    return HttpResponse('GET method not supported')


@api_view(['GET', 'POST'])
@parser_classes((JSONParser,))
def ann(request):
    if request.method == 'POST':
        if request.data[Constant.FIELD_MODEL][-3:] == "REG":
            result, unit = NeuralNetworkModel.predictREG(request.data[Constant.FIELD_NIR],
                                                         request.data[Constant.FIELD_MODEL])
            return HttpResponse(str(round(result, 2)) + " " + unit)
        else:
            result, unit = NeuralNetworkModel.predictCLS(request.data[Constant.FIELD_NIR],
                                                         request.data[Constant.FIELD_MODEL])
            return HttpResponse(result + " " + unit)
    return HttpResponse('GET method not supported')


@api_view(['GET', 'POST'])
def predictFile(request):
    if request.method == 'POST':
        model = request.data.get(Constant.FIELD_MODEL)
        dataFile = request.FILES.get(Constant.FIELD_DATA_FILE)
        NeuralNetworkModel.filePredict(dataFile, model)
        result = open("result.csv", "rb")
        return HttpResponse(result, content_type='application/csv')
    return HttpResponse('GET method not supported')


@api_view(['GET', 'POST'])
@parser_classes((JSONParser,))
def getModelList(request):
    if request.method == 'GET':
        result = FileUtil.get_list_model()
        return JsonResponse(result, safe=False)
    return HttpResponse('POST method not supported')


@api_view(['GET', 'POST'])
def uploadModel(request):
    if request.method == 'POST':
        modelName = request.data.get(Constant.FIELD_MODEL_NAME)
        modelFile = request.FILES.get(Constant.FIELD_MODEL_FILE)
        dataFile = request.FILES.get(Constant.FIELD_DATA_FILE)
        weightFile = request.FILES.get(Constant.FIELD_WEIGHT_FILE)
        modelType = request.data.get(Constant.FIELD_MODEL_TYPE)
        FileUtil.save_model_file(modelName, dataFile, modelFile, weightFile, modelType)
        return HttpResponse("Save success")
    return HttpResponse('GET method not supported')


@api_view(['GET', 'POST'])
@parser_classes((JSONParser,))
def deleteModel(request):
    if request.method == 'POST':
        modelName = request.data.get(Constant.FIELD_MODEL)
        FileUtil.delete_model(modelName)
        return HttpResponse("Delete success")
    return HttpResponse('GET method not supported')
