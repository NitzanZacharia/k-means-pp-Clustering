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
        dist = getDistance(x[i], cent)
        inertia += dist
    return inertia
def find_elbow(k_vals, inrts):
    k = np.array(k_vals)
    y = np.array(inrts)
    p1 = np.array([k[0], y[0]])
    p2 = np.array([k[-1], y[-1]])
    line_v = p2 - p1
    line_n = line_v / np.linalg.norm(line_v)
    distances = []
    for i in range(len(k)):
        point = np.array([k[i], y[i]])
        vp1 = point - p1
        proj = np.dot(vp1, line_n) * line_n
        pdist= vp1 - proj
        distances.append(np.linalg.norm(pdist))
    return k[np.argmax(distances)]

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
    elbow_k = find_elbow(k_axis, inrt)
    plt.figure(figsize=(8, 5))
    plt.plot(k_axis, inrt, marker='o')
    plt.xlabel("K")
    plt.ylabel("Average Dispersion")
    plt.title("Elbow Method for selection of Optimal 'K' clusters")
    plt.axvline(x=elbow_k, color='r', linestyle='--')
    plt.annotate(f"Elbow at k={elbow_k}", xy=(elbow_k, inertias[elbow_k - 1]),
                 xytext=(elbow_k + 0.5, inrt[elbow_k - 1] + 100),
                 arrowprops=dict(facecolor='red', shrink=0.05))

    plt.tight_layout()
    plt.savefig("elbow.png")

if __name__ == "__main__":
    main()