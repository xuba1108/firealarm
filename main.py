import time
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import firebase_admin
from firebase_admin import db, credentials
from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
from sklearn.preprocessing import LabelEncoder

# Authenticate to Firebase
cred = credentials.Certificate("D:\IU\Sem2-Nam 3\Capstone 2\Project\python\Firebase Json\cap2-firebaseusa-firebase-adminsdk-ywlga-ee7b517fa3.json")
firebase_admin.initialize_app(cred, {"databaseURL": "https://cap2-firebaseusa-default-rtdb.firebaseio.com/"})

# Load the dataset
df=pd.read_csv('D:\IU\Sem2-Nam 3\Capstone 2\Project\python\Database\Data28_06.csv')

# Preprocess the data
le = LabelEncoder()
df['Fire'] = le.fit_transform(df['Fire'])

# Split data into features and target
x = df[['MQ7', 'MQ2', 'Humidity', 'Temp']]
y = df['Fire']
X_train, X_test, y_train, y_test = train_test_split(x, y, test_size=0.2, random_state=1000)

# Train the Naive Bayes model
nb_model = GaussianNB()
nb_model.fit(X_train, y_train)

while True:
    try:
        # Retrieve sensor data from Firebase
        MQ7 = db.reference("/Node 1/CO Sensor").get()
        MQ2 = db.reference("/Node 1/Smoke Sensor").get()
        Humidity = db.reference("Node 1/Humidity").get()
        Temp = db.reference("Node 1/Temp").get()

        # Organize inputs into a list
        input_data = [MQ7, MQ2, Humidity, Temp]
        print('input data of node 1:', input_data)
        # Make a prediction
        prediction = nb_model.predict([input_data])

        # Update state based on prediction
        if prediction[0] == 1:
            db.reference("Node 1/State").set("LA2.201_is_fire")
            #db.reference("Node 1/State Number").set("1")
            print('Fire Detected at Node 1')
        else:
            db.reference("Node 1/State").set("Normal_State")
            #db.reference("Node 1/State Number").set("0")
            print('No Fire Detected at Node 1')

        time.sleep(1)  # Adjust the time interval as per your requirement
        MQ7 = db.reference("/Node 2/CO Sensor").get()
        MQ2 = db.reference("/Node 2/Smoke Sensor").get()
        Humidity = db.reference("Node 2/Humidity").get()
        Temp = db.reference("Node 2/Temp").get()
        # Organize inputs into a list
        input_data = [MQ7, MQ2, Humidity, Temp]
        # Make a prediction
        prediction = nb_model.predict([input_data])
        print('input data of node 2:',input_data)
        # Update state based on prediction
        if prediction[0] == 1:
            db.reference("Node 2/State").set("LA2.202_is_fire")
            print('Fire Detected at Node 2')
        else:
            db.reference("Node 2/State").set("Normal_State")
            print('No Fire Detected at Node 2')
        #time.sleep(1)  # Adjust the time interval as per your requirement
        #Node 3
        MQ7 = db.reference("/Node 3/CO Sensor").get()
        MQ2 = db.reference("/Node 3/Smoke Sensor").get()
        Humidity = db.reference("Node 3/Humidity").get()
        Temp = db.reference("Node 3/Temp").get()
        # Organize inputs into a list
        input_data = [MQ7, MQ2, Humidity, Temp]
        prediction = nb_model.predict([input_data])
        print('input data of node 3:',input_data)
        if prediction[0] == 1:
            db.reference("Node 3/State").set("LA2.208_is_fire")
            print('Fire Detected at Node 3')
        else:
            db.reference("Node 3/State").set("Normal_State")
            print('No Fire Detected at Node 3')

    except Exception as e:
        print(f"Error: {e}")
        time.sleep(3000)  # Wait before retrying
 