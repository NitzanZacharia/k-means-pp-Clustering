import matplotlib.pyplot as plt
import numpy as np
from sklearn.datasets import load_iris
from sklearn.cluster import KMeans

#calculates the squared distance between two points
def getDistance(point1, point2):
    d = len(point1)
    dist = 0.0
    for i in range(d):
        diff = float(point1[i])-float(point2[i])
        dist += diff*diff
    return dist

def comp_inrt(x, clusts, cents):
    inertia = 0.0
    for i in range(len(x)):
        cent = cents[clusts[i]]
        dist = np.sum(getDistance(x[i], cent))
        inertia += dist
    return inertia
def main():
     
    data_set = load_iris()
    data_arr = data_set.data
    inrt = []
    for k in range(1,11):
        kmeans = KMeans(n_clusters=k, init='k-means++', random_state=0)
        kmeans.fit(data_arr)
        clusts = kmeans.labels_
        cents = kmeans.cluster_centers_
        inertia = comp_inrt(data_arr, clusts, cents)
        inrt.append(inertia)
    k_axis = range(1,11)
    plt.figure(figsize=(8, 5))
    plt.plot(k_axis, inrt, marker='o')
    plt.xlabel("K")
    plt.ylabel("Average Dispersion")
    plt.title("Elbow Method for selection of Optimal 'K' clusters")