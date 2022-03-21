# ProjectThesis/urls.py
from django.urls import path
from . import views

urlpatterns = [
    path('', views.HomePageView.as_view(), name='home'),
    path('about/', views.AboutPageView.as_view(), name='about'),
    path('plsRegression/', views.plsRegression, name='plsRegression'),
    path('ann/', views.ann, name='ann'),
    path('modelList/', views.getModelList, name='modelList'),
    path('uploadModel/', views.uploadModel, name='uploadModel'),
    path('deleteModel/', views.deleteModel, name='deleteModel'),
    path('predictFile/', views.predictFile, name='predictFile'),

]
