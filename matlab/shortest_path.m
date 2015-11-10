function o = shortest_path(P, s, d)

[V,N] = dijkstra(P',d);

o = s;
for k = 1:V(s), 
    o = [o; N(o(end))];
end
