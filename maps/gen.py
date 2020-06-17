#!/usr/bin/env python3

import random as rnd

def rndel(s):
    k = rnd.randint(0, len(s))
    for el, i in s:
        if i == k:
            return el

n = rnd.randint(10, 20)

unused = set(range(1, n))

edges = []

directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]

def dfs(v, d):
    tmp_directions = directions.copy()
    if not v == 0:
        tmp_directions.remove(d)
    rnd.shuffle(tmp_directions)
    for d in tmp_directions:
        stop = rnd.randint(1, len(directions))
        if stop == len(directions) or len(unused) == 0:
            return
        [x, y] = d
        u = unused.pop()
        edges.append((v, u, x, y))
        dfs(u, (-x, y))


dfs(0, (0,0))
print(n, len(edges))
for (v, u, dx, dy) in edges :
    print(v + 1, u + 1 dx, dy) 
