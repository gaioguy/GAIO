function h = boxplot2(B, varargin)

%BOXPLOT2   2d box plot.
%
%   BOXPLOT2(B) plots the boxes given by B. B can be matrix, then each row
%   defines one box (columns 1:size(B,2)/2 define  the centers, the 
%   remaining columns the radii of the boxes) or a Tree, then the leaves of the tree are
%   plotted.
%   
%   BOXPLOT2(B,'depth',d) plots the boxes given by the nodes at depth d
%
%   BOXPLOT2(B,'color',c) uses color c (with the standard Matlab colors)
%
%   BOXPLOT2(B,'coords',[i j]) uses the coordinates i and j (default is i=1
%   and j=2) for higher dimensional boxes
%
%   BOXPLOT2(B,'density',v) colors the boxes according to the colors
%   specified in the vector v
%
%   BOXPLOT2(B,'alpha',a) sets the opacity of the boxes to a
%
%   These options can be combined arbitrarily.

p = inputParser;
defaultColor = 'r';
defaultCoords = [1 2];
defaultDepth = -1;
defaultAlpha = 1; alpha_edge = 1;
defaultEdgeColor = 'k';

addRequired(p,'B');
addParamValue(p,'color',defaultColor,@ischar);
addParamValue(p,'coords',defaultCoords,@isvector);
addParamValue(p,'depth',defaultDepth,@isnumeric);
addParamValue(p,'density',[],@isvector);
addParamValue(p,'alpha',defaultAlpha,@isnumeric);
addParamValue(p,'edgecolor',defaultEdgeColor,@ischar);

parse(p, B, varargin{:});
ix    = p.Results.coords(1);
iy    = p.Results.coords(2);
depth = p.Results.depth;
alpha = p.Results.alpha;
edgecolor = p.Results.edgecolor;
if alpha < 1,
    alpha_edge = 0;
end
if isempty(p.Results.density)
    col = p.Results.color;
else
    col = p.Results.density(:)';
end

if isa(B,'Tree')
    b = B.boxes(depth)';
    dim = B.dim;
else
    b = B;
    dim = size(b,2)/2;
end

x = [b(:,ix)-b(:,ix+dim) b(:,ix)+b(:,ix+dim) b(:,ix)+b(:,ix+dim) b(:,ix)-b(:,ix+dim)];
y = [b(:,iy)-b(:,iy+dim) b(:,iy)-b(:,iy+dim) b(:,iy)+b(:,iy+dim) b(:,iy)+b(:,iy+dim)];
h = patch(x',y',col); set(h,'edgeColor',edgecolor,'FaceAlpha',alpha,'EdgeAlpha',alpha_edge);

