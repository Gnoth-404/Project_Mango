import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
from matplotlib import pyplot as plt
import numpy as np
xaxis = np.arange(289)
def listener(event):
    print(event.event_type)  # can be 'put' or 'patch'
    print(event.path)  # relative to the reference, it seems
    print(event.data)
    yaxis= event.data.split(',')
    plt.xlim(0,289)
    plt.xlabel('Wavelength(nm)')
    plt.plot(xaxis,yaxis)
    plt.yticks([])
    plt.show()
json_path = 'fruitapp.json'
my_app_name = 'fruitapp-vgu2020'

cred = credentials.Certificate(json_path)        
obj = firebase_admin.initialize_app(cred, {'databaseURL': 'https://fruitapp-vgu2020.firebaseio.com/'})

data= db.reference('/Realtime', app= obj).listen(listener)
