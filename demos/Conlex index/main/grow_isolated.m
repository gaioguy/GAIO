function I = grow_isolated(S, P, tree, depth)

% Grows an isolating neighborhood containing S.
% P = transition matrix

% initializing I (vector of box numbers)
I = S;
oI = get_nbhd(tree, I, depth);

% computing maximal invariant set (vector of box numbers) in a 
% neighborhood of I
Inv_oI = oI(find(max_inv_set(P(oI,oI))));

% if maximal invariant set in oI is not in interior of oI, expand
% neighborhood by a one box layer

while ~in_int(tree, Inv_oI, oI, depth)
  oI = get_nbhd(tree, Inv_oI, depth);
  Inv_oI = oI(find(max_inv_set(P(oI,oI))));
end

I = Inv_oI;
