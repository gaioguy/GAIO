function [G, C, Length_comp] = conn_comp_map(I, tree, depth, P)

%% Computes graph G (from P) on connected components of I, 
%% on component C(:,i), with length Length_comp(i)

b = tree.boxes(depth);
n = tree.count(depth);

% Partitioning into connected components;
no_comp = 1;
N = nbhd_graph(tree, depth);
Comp = conn_comp(N, I, I(1));
add = length(I) - length(Comp);
Comp_full = [Comp'; zeros(add,1)];
C = Comp_full;
Length_comp = zeros(length(I));
Length_comp(no_comp) = length(Comp);
remaining = setdiff(I, Comp);



while (~isempty(remaining))
  Comp = conn_comp(N, I, remaining(1));
  add = length(I) - length(Comp);
  Comp_full = [Comp'; zeros(add,1)];
  C = [C Comp_full];
  remaining = setdiff(remaining, Comp);
  no_comp = no_comp+1;
  Length_comp(no_comp) = length(Comp);
end

G = zeros(no_comp, no_comp);

for j = 1:no_comp
   Comp = C(1:Length_comp(j),j);
   FC = find(P*flags(Comp,n));
   for i = 1:no_comp
     if (~isempty(intersect(C(:,i), FC))), G(i,j)=1;
     end
   end
end
   
