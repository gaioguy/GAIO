function f = flags(I, n)

% F = flags(I, N) returns an N-vector F, for which F(I)=1 and
%     F=0 elsewhere

f = zeros(n, 1);
f(I) = 1;

