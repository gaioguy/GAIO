function [V,N] = dijkstra(A, D)

%DIJKSTRA	shortest paths
%   [V,~] = DIJKSTRA(A, D) computes the lengths of the shortest paths
%   between all nodes and the destination D, regarding the sparse matrix
%   A as a directed weighted graph, where the entry (i,j) in A defines
%   the weight of the edge j->i.
