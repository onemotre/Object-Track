from pandas import read_csv
import pandas as pd
import numpy as np
from sklearn import svm
from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler

# read files
filename = "tmp/data.csv"
names = ['center_x', 'center_y', 'radius', 'orient']
data = read_csv(filename, names=names)

# data preprocessing
data = data.dropna()
data = data[data["center_x"] >= 0]

center_x_ori = data['center_x']

res_center_x = center_x_ori.last

if len(center_x_ori) > 2:
    # get the Y data
    center_x_next = center_x_ori[1:].array.reshape(-1, 1).ravel()
    print(center_x_next)

    # get the X data
    center_x_ori = center_x_ori.drop(center_x_ori.index[-1]).array.reshape(-1, 1)
    print(center_x_ori)

    # train data
    svm_classifier = SVC(kernel='linear', C=1.0)
    svm_classifier.fit(center_x_ori, center_x_next)

    # predict
    res_center_x = svm_classifier.predict(center_x_next[-1])
    for i in range(0, 13):
        res_center_x = svm_classifier.predict(res_center_x)

print(res_center_x)