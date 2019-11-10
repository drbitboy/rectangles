from datetime import datetime,timedelta
from math import sqrt, log
from random import uniform as ru

results = list()
N = 3
for i in range(20):
    N = ((N * 120) / 100) + 10
    edge = int(sqrt(N)*1.25) + 1
    pairs = list(set([(int(ru(0,edge)),int(ru(0,edge)),) for i in range(N)]))
    t0=datetime.now()
    st = set()
    count = 0
    while pairs:
        x,y = pairs.pop()
        for ox,oy in pairs:
            lsquared = (x-ox)*(x-ox) + (y-oy)*(y-oy)
            triple = (x+ox,y+oy,lsquared)
            if triple in st: count += 1
            else           : st.add(triple)
    results.append((N,N*N,edge,(datetime.now()-t0).total_seconds(),count))
    print(results[-1])

