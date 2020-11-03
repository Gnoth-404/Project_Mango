# -*- coding: utf-8 -*-
"""
Created on Wed Sep 30 12:17:53 2020
@author: ACER
"""
import firebase_admin
from firebase_admin import db
from firebase_admin import credentials
import csv


# Fetch the service account key JSON file contents
cred = credentials.Certificate('Credentials.json')

# Initialize the app with a service account, granting admin privileges
default_app = firebase_admin.initialize_app(cred, {'databaseURL': 'https://fruit-classification-57e9d.firebaseio.com'})

# As an admin, the app has access to read and write all data, regradless of Security Rules
ref = db.reference()

users_ref = ref.child('admin')
users_ref.set({
    'khang1': {
        'DOB': 'october 26, 2001',
        'nickname': 'wie gehts?'
    },
    'khang2': {
        'DOB': 'october 26, 2001',
        'nickname': 'wie gehts?'
    },
    'khang3': {
        'DOB': 'october 26, 2001',
        'nickname': 'wie gehts?'
    }
}
)

sample_Data_ref = ref.child('Mango Sample Data')
sample_Data_ref.set(dict(sample1={
    "no": "1",
    "mangocultivars": "cengkir",
    "vitaminc": "62.51267",
    "sscobrix": "8.695"
}))

# retrieve data
ref = db.reference('data')
snapshot = ref.get()

#ccommand
with open('test.csv', 'w') as outfile:
    for key in snapshot.keys():
        outfile.write("{},{}\n".format(key[2::], snapshot[key]))
