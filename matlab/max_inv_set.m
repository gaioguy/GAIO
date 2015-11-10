function M = max_inv_set(P)

% MAX_INV_SET	maximal invariant set.
%   MAX_INV_SET(P) computes the maximal invariant set of the transition
%   matrix P.
%

  M = fwd_inv_set(P) & fwd_inv_set(P');
