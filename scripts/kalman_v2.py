import numpy as np
import pandas as pd
from pandas import read_csv

# kalman filter's parameters
# @parm: A is the state transition matrix
# @parm: H is the observation matrix
# @parm: E is the identity matrix
# @parm: Q is the process noise covariance matrix
# @parm: R is the measurement noise covariance matrix

A = np.array([
    [1, 1, 0, 0],
    [0, 1, 0, 0],
    [0, 0, 1, 1],
    [0, 0, 0, 1]
])
H = np.array([
    [1, 0, 0, 0],
    [0, 0, 1, 0]
])
E = np.array([
    [1, 0, 0, 0],
    [0, 1, 0, 0],
    [0 ,0, 1, 0],
    [0, 0, 0, 1]
])
Q = np.array([
    [1000, 0, 0, 0],
    [0, 1000, 0, 0],
    [0, 0, 0.01, 0],
    [0, 0, 0, 0.01]
])
R = np.array([
    [0.00001, 0],
    [0, 0.0004]
])

# get the data from csv
def readData():
    filename = "../tmp/data.csv"
    names = ["center_x", "center_y", "radius", "orient"]
    datalist = pd.read_csv(filename, names=names)
    return datalist

# get measure data from csv
def get_Zk(datalist):
    for item in datalist.values:
        x = item[0]
        y = item[1]
        yield np.array([[x], [y]])

# predict the next data
def predict(xkp, pkp, Kk):
    while True:
        # predict function
        xk_ = A * xkp
        pk_ = A * pkp * A.T + Q
        # calibrate function
        zk = np.dot(H, xkp)
        xk = xk_ + np.dot(Kk, (zk - np.dot(H, xk_)))
        pk = np.dot((E - np.dot(Kk, H)), pk_)
        Kk = np.dot(np.dot(pk_, H.T), np.linalg.inv(np.dot(np.dot(H, pk_), H.T) + R))
        # update and save the parm
        xkp = xk
        pkp = pk
        yield xk

# write data to result.csv
def writeCsv(preDataX, preDataY, oriDataR, oriDataO):
    filename = "../tmp/result.csv"
    result = pd.DataFrame({
        "x" : preDataX,
        "y" : preDataY,
        "radius" : oriDataR,
        "orient" : oriDataO
    })
    result.to_csv(filename)

# Kalman Filter
def kalman():
    # define the begin state
    xkp = np.array([0, 1, 0, 1])
    pkp = np.array([
        [1, 0, 0, 0],
        [0, 1, 0, 0],
        [0 ,0, 1, 0],
        [0, 0, 0, 1]
    ])
    # get the original data
    datalist = readData()
    size = datalist.shape[0]
    z = get_Zk(datalist)

    # storage the data
    preDataX = []
    preDataY = []
    oriDataR = datalist["radius"].values.tolist()
    oriDataO = datalist["orient"].values.tolist()
    for t in range(1, size + 1):
        # predict function
        xk_ = A * xkp
        pk_ = A * pkp * A.T + Q

        # calibrate function
        Kk = np.dot(np.dot(pk_, H.T), np.linalg.inv(np.dot(np.dot(H, pk_), H.T) + R))
        zk = next(z)
        xk = xk_ + np.dot(Kk, (zk - np.dot(H, xk_)))
        pk = np.dot((E - np.dot(Kk, H)), pk_)

        # update and save the parm
        xkp = xk
        pkp = pk

        # predict data next
        x = predict(xk, pk, Kk)
        for i in range(1,15):
            xk = next(x)

        preDataX.append(xk[0][0])
        preDataY.append(xk[2][0])

    # write data to the csv file
    writeCsv(preDataX, preDataY, oriDataR, oriDataO)

if __name__ == "__main__":
    # start the kalman filter
   kalman()
