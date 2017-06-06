%% script for compiling the C-core of GAIO
% You only need to call this script if you work on a (currently)
% unsupported platform, i.e. there is no need to call this script
% on Mac/Intel, Linux/Intel and Windows/Intel platforms.
%
% Note: Your Matlab mex compiler needs to be be setup first ('mex -setup')

cd matlab
mex -largeArrayDims -D_MEASURE_ -I../lib tree_.c ../lib/*.c
mex -largeArrayDims -D_MEASURE_ -I../lib subshift.c ../lib/*.c
mex -largeArrayDims -D_MEASURE_ -I../lib dijkstra.c 
mex -largeArrayDims -D_MEASURE_ -I../lib scc.c
mex -largeArrayDims -D_MEASURE_ -I../lib fwd_inv_set.c
cd ../@Tree
mex -D_MEASURE_ -I../lib subsref.c "../lib/*.c"
mex -D_MEASURE_ -I../lib subsasgn.c "../lib/*.c"
mex -D_MEASURE_ -I../lib delete.c "../lib/*.c"
cd ..

