import os
import sys
import math

rawdata=[map(float,s.split('=')[0].split(','))
for s in """
11550,133402500,11550,195924644=N,N*N,edge,cumtime
""".strip().split('\n')]

def getfin(argv):
  try   : return open(argv[0],'r')
  except: return sys.stdin

log2 = math.log(2.0)

with getfin(sys.argv[1:]) as fin:
  for N,Nsq,edge,cumtime in [map(float,s.split('=')[0].split(','))
                                for s in fin
                                if s.strip().endswith(',cumtime')
                               ]:
    log2N = math.log(N) / log2
    print('{0:8.0f},{1:12.0f},{2:12.4e},{3:12.4e},{4:12.4e},{5:12.4e}'.format
          (N,cumtime
          ,cumtime/Nsq,      cumtime/(Nsq*log2N)
          ,cumtime/(Nsq*N),  cumtime/(Nsq*N*log2N)
          ))
