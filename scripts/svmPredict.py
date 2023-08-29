from pandas import read_csv
import pandas as pd
import numpy as np
from sklearn import svm
from sklearn.svm import SVC

# read files
filename = "tmp/data.csv"
names = ['center_x', 'center_y', 'radius', 'orient']
data = read_csv(filename, names=names)

# data preprocessing
data = data.dropna()
data = data[data["center_x"] >= 0]

# original data
center_x_ori = data['center_x']
center_y_ori = data['center_y']
radius_ori = data['radius']
orient_ori = data['orient']

res_center_x = center_x_ori.last
res_center_y = center_y_ori.last
res_radius = radius_ori.last
res_orient = orient_ori.last

if len(center_x_ori) > 2:

    # get the Y data
    center_x_next = center_x_ori[1:].values.reshape(-1, 1)
    center_y_next = center_y_ori[1:].values.reshape(-1, 1)
    radius_next = radius_ori[1:].values.reshape(-1, 1)
    orient_next = orient_ori[1:].values.reshape(-1, 1)

    # get the X data
    center_x_ori = center_x_ori.drop(center_x_ori.index[-1]).values.reshape(-1, 1)
    center_y_ori = center_y_ori.drop(center_y_ori.index[-1]).values.reshape(-1, 1)
    radius_ori = radius_ori.drop(radius_ori.index[-1]).values.reshape(-1, 1)
    orient_ori = orient_ori.drop(orient_ori.index[-1]).values.reshape(-1, 1)

    # train data
    svm_classifier = SVC(kernel='linear')
    svm_classifier.fit(center_x_ori, center_x_next)
    svm_classifier.fit(center_y_ori, center_y_next)
    svm_classifier.fit(radius_ori, radius_next)
    svm_classifier.fit(orient_ori, orient_next)

    # predict    
    res_center_x = svm_classifier.predict(center_x_next[-1])
    res_center_y = svm_classifier.predict(center_y_next[-1])
    res_radius = svm_classifier.predict(radius_next[-1])
    res_orient = svm_classifier.predict(orient_next[-1])
    for i in range(0, 13):
        res_center_x = svm_classifier.predict(res_center_x)
        res_center_y = svm_classifier.predict(res_center_y)
        res_radius = svm_classifier.predict(res_radius)
        res_orient = svm_classifier.predict(res_orient)

# final result write
resultFilename = "tmp/result.csv"
data = {
    "center_x": [res_center_x],
    "center_y": [res_center_y],
    "radius": [res_radius],
    "orient": [res_orient]
}

df = pd.DataFrame(data)

df.to_csv(resultFilename, index=False)
