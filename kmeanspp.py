import sys
import math
import pandas as pd
import numpy as np 
import mykmeanspp



#calculates the distance between two points
def getDistance(point1, point2):
    d = len(point1)
    dist = 0
    for i in range(d):
        diff = float(point1[i])-float(point2[i])
        dist += diff*diff
    return math.sqrt(dist)

def kmeanspp_our(points, k):
    np.random.seed(1234)
    n = len(points)
    ind = np.random.choice(n)
    cents = [points[ind]]
    inds =[ind] #inds of points already chosen

    for i in range(k-1):
        d_arr = []
        for x in points:
            min_d = min(getDistance(x, c) for c in cents)
            d_arr.append(min_d)
        sum_d = sum(d_arr)
        pr_arr = [d / sum_d for d in d_arr]
        ind_next = np.random.choice(n, p=pr_arr)
        cents.append(points[ind_next])
        inds.append(ind_next)
    return inds, cents

def check_validation(k, n ,iter, eps):
    try:
        knum = float(k)
        if ((not knum.is_integer()) or knum>=n or knum<2):
            print("Incorrect number of clusters!")
            return False
    except:
        print("Incorrect number of clusters!")
        return False
    try:
        iternum = float(iter)
        if ((not iternum.is_integer()) or iternum>=1000 or iternum<2):
            print("Incorrect number of iteration!")
            return False
    except:
        print("Incorrect number of iteration!")
        return False
    
    epsnum = float(eps)    
    if(epsnum<0):
        print("invalid epsilon!")
        return False
    return True  

def main():
    try:
        args = sys.argv
        #input_data = sys.stdin.readlines()
        
        if len(args)<5 or len(args)>6:
            print("Incorrect number of inputs!")
            return
        if len(args) == 5:
            temp = args
            args = temp[:2]+[300]+temp[2:]
            
        
        k = args[1]
        iter = args[2]
        eps = args[3]
        file1 = args[4]
        file2= args[5]
        f1 = pd.read_csv(file1)
        f2 = pd.read_csv(file2)
        joined = pd.merge(f1, f2, on = f1.columns[0], how='inner')
        n = len(joined)
        joined.sort_values(by=joined.columns[0], inplace=True) 
        #if input is not valid, print an error message and end the run
        valid_input = check_validation(k,n, iter, eps)
        if not valid_input:
            return
        k = int(float(args[1]))
        iter = int(float(args[2]))
        eps = float(args[3])    
        points = joined.iloc[:, 1:].to_numpy()
        inds, cents = kmeanspp_our(points, k)
        org_indc = joined.iloc[:, 0].to_numpy()
        firstln = [org_indc[i] for i in inds]
        print(",".join(str(x) for x in firstln))