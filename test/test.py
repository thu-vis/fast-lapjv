import unittest
import numpy as np
import os
import math
from time import time

from fastlapjv import fastlapjv
from lapjv import lapjv
from numpy import random, meshgrid, linspace, dstack, sqrt, array, \
    float32, float64
from scipy.spatial.distance import cdist

def exact_lapjv():
    X = np.load("test\X.npy")
    t = time()
    X -= X.min(axis=0)
    X /= X.max(axis=0)
    num = X.shape[0]
    square_len = math.ceil(np.sqrt(num))
    N = square_len * square_len
    grids = np.dstack(np.meshgrid(np.linspace(0, 1 - 1.0 / square_len, square_len),
                                  np.linspace(0, 1 - 1.0 / square_len, square_len))) \
        .reshape(-1, 2) + 0.5 / square_len
    original_cost_matrix = cdist(grids, X, "euclidean")
    dummy_points = np.ones((N - original_cost_matrix.shape[1], 2)) * 0.5
    # dummy at [0.5, 0.5]
    dummy_vertices = (1 - cdist(grids, dummy_points, "euclidean")) * 100
    cost_matrix = np.concatenate((original_cost_matrix, dummy_vertices), axis=1)
    cost_matrix[cost_matrix==0]  = 1000000
    row_asses, col_asses, info = lapjv(cost_matrix)
    cost = original_cost_matrix[col_asses[:num],
                                np.array(range(N))[:num]].sum()

    grid_X = grids[col_asses[:num]]
    return time() - t, cost

def approximated_lapjv():
    X = np.load("test\X.npy")
    t = time()
    X -= X.min(axis=0)
    X /= X.max(axis=0)
    num = X.shape[0]
    square_len = math.ceil(np.sqrt(num))
    N = square_len * square_len
    grids = np.dstack(np.meshgrid(np.linspace(0, 1 - 1.0 / square_len, square_len),
                                  np.linspace(0, 1 - 1.0 / square_len, square_len))) \
        .reshape(-1, 2) + 0.5 / square_len
    original_cost_matrix = cdist(grids, X, "euclidean")
    dummy_points = np.ones((N - original_cost_matrix.shape[1], 2)) * 0.5
    # dummy at [0.5, 0.5]
    dummy_vertices = (1 - cdist(grids, dummy_points, "euclidean")) * 100
    cost_matrix = np.concatenate((original_cost_matrix, dummy_vertices), axis=1)
    cost_matrix[cost_matrix==0]  = 1000000
    row_asses, col_asses, info = fastlapjv(cost_matrix, k_value=50)
    cost = original_cost_matrix[col_asses[:num],
                                np.array(range(N))[:num]].sum()
    return time() - t, cost


if __name__ == "__main__":
        # exact LAPJV
        exact_time, exact_cost = exact_lapjv()

        # approximated LAPJV
        appro_time, appro_cost = approximated_lapjv()
        
        print("computional time of exact method is: ", exact_time)
        print("cost of exact method is:", exact_cost)
        print("computional time of approximated method is: ", appro_time)
        print("cost of approximated method is: ", appro_cost)
        print("****************************************")
        print("the computational time is reduced by: {0:.2f}%".format((exact_time - appro_time) / exact_time * 100))
        print("the error of cost is: {0:.2f}%".format((appro_cost - exact_cost) / exact_cost * 100))
