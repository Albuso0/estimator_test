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

    y = np.exp(1j*x)
    mu_base = np.mean(x[nQ:,:], axis=0)
    mu_naive = np.mean(x, axis=0)
    # print(mu_base, '\n')
    m = y.T.conjugate() @ y / n
    m1 = (m-np.eye(d))*np.exp(1)+np.eye(d)

    # m1 = np.ones((d,d))
    # print(m1,'\n')


    z = lead_eigvec(m1)
    mu = np.angle(z)

    # print(mu,'\n')

    dtrig = dist(mu,np.zeros(d))
    dbase = dist(mu_base,np.zeros(d))
    excess = dtrig - dbase

    return dtrig, dbase, excess


for d in range(20,401,5):
    n = d*10
    eps = 0.1
    dtrig, dbase, excess = exp(n,d,eps)
    print(d, n, dtrig, dbase, excess, excess/np.sqrt(d))
