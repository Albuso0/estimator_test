import numpy as np
from numpy.random import randn
from numpy.linalg import eigh, norm

def dist(u,v):
    d = len(u)
    c = -np.sum(u-v)/d
    return norm(u-v+c*np.ones(d))


def lead_eigvec(m):
    w, v = eigh(m)
    idx = w.argsort()[::-1]
    w = w[idx]
    v = v[:,idx]
    return v[:,0]


def exp(n,d,eps):
    x = randn(n,d)

    vQ = np.ones(d)
    for i in range(0,d,2):
        vQ[i]=-1

    nQ = int(n*eps)

    Q = np.tile(vQ,(nQ,1))

    x[0:nQ,:] = x[0:nQ,:] + Q

    mu_base = np.mean(x[nQ:,:], axis=0)
    mu_naive = np.mean(x, axis=0)
    # print(mu_base, '\n')
    
    y = np.exp(1j*x)
    m = y.T @ y.conjugate() / n
    m1 = (m-np.eye(d))*np.exp(1)+np.eye(d)

    # m1 = np.ones((d,d))
    # print(m1,'\n')


    z = lead_eigvec(m1)
    mu = np.angle(z)
    for i in range(d):
        if mu[i]<-3:
            mu[i] += 2*np.pi

    # print(mu,'\n')

    dtrig = dist(mu,np.zeros(d))
    dbase = dist(mu_base,np.zeros(d))
    excess = dtrig - dbase

    return dtrig, dbase, excess

def population_exp(d, eps):
    vQ = np.ones(d)
    for i in range(0,d,2):
        vQ[i]=-1
    zQ = np.exp(1j*vQ)
    mQ = np.outer(zQ, zQ.conjugate())

    m = (1-eps)* (  (np.ones((d,d)) - np.eye(d))*np.exp(-1)+np.eye(d) ) + eps * (  (mQ - np.eye(d))*np.exp(-1)+np.eye(d) )
    m1 = (m-np.eye(d))*np.exp(1)+np.eye(d)

    # print(m1)
    z = lead_eigvec(m1)
    mu = np.angle(z)
    for i in range(d):
        if mu[i]<-3:
            mu[i] += 2*np.pi
    # print(mu)
    
    dtrig = dist(mu,np.zeros(d))

    return dtrig

    
# d=2
# n=50000
# eps = 0.
# dtrig, dbase, excess = exp(n,d,eps)
# dtrig_p = population_exp(d,eps)    
# print(d, n, dtrig, dbase, excess, excess/np.sqrt(d), dtrig_p, dtrig_p/np.sqrt(d))



## test estimation error scaling with d

for d in range(20,401,5):
    n = d*10
    eps = 0.1
    dtrig, dbase, excess = exp(n,d,eps)
    dtrig_p = population_exp(d,eps)
    
    print(d, n, dtrig, dbase, excess, excess/np.sqrt(d), dtrig_p, dtrig_p/np.sqrt(d))







