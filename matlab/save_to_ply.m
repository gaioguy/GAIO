function  save_to_ply( t, filename, color, coordinates )

% save_to_ply(t, filename, color, coordinates)
%
% t: GAIO-Tree
% filename: e.g. myTree.ply
% color: eigenvector of the discretized Perron-Frobenius operator, e.g. max(log10(abs(V(:,1)))+20,0)
% coordinates: 3-D coordinates of the box covering, e.g. [1 2 3]
%
% Output: Polygon File Format (or the Stanford Triangle Format) of the box
% covering. Load with e.g. 'blender' or 'paraview'
%
% Adrian Ziessler
% ziessler@math.upb.de
% June 2016

% standard values
wFilename = 'defaultname.ply';
wColor = [];
wCoordinates = [1 2 3 ];
Format = 'ascii';

if nargin > 1
    wFilename = filename;
end

if nargin > 2
    wColor = color;
end

if nargin > 3
    wCoordinates = coordinates;
end


Elements = makePlywriteData(t,wColor,wCoordinates);
colorInfo = 0;

[fid,Msg] = fopen(wFilename,'wt');

if fid == -1, error(Msg); end

%% write PLY header %%
fprintf(fid,'ply\nformat %s 1.0\ncomment created by MATLAB plywrite\n',Format);
ElementNames = fieldnames(Elements);
NumElements = length(ElementNames);

for i=1:NumElements
    if strcmp(ElementNames{i},'Color')
        NumElements = NumElements-1;
        colorInfo = 1;
    end
end

noOfVertices = size(Elements.vertex,1);
noOfFaces = size(Elements.face,1);

fprintf(fid,'element vertex %d\n',noOfVertices);
fprintf(fid,'property float x\n');
fprintf(fid,'property float y\n');
fprintf(fid,'property float z\n');
if colorInfo
    fprintf(fid,'property uchar red\n');
    fprintf(fid,'property uchar green\n');
    fprintf(fid,'property uchar blue\n');
end
fprintf(fid,'element face %d\n',noOfFaces);
fprintf(fid,'property list uchar int vertex_indices\n');
fprintf(fid,'end_header\n');

fclose(fid);


%% write vertices and faces

if ~colorInfo
    dlmwrite(wFilename,Elements.vertex,'delimiter',' ','-append');
else
    dlmwrite(wFilename,[Elements.vertex Elements.Color],'delimiter',' ','-append');
end

dlmwrite(wFilename,[4*ones(noOfFaces,1) Elements.face],'delimiter',' ','precision','%d','-append');

end

function Data = makePlywriteData(t,color,coordinates)

    b = t.boxes(-1)';
    noOfBoxes = size(b,1);
    noOfFaces = 6;

    d = t.dim;

    x = [];
    y = [];
    z = [];

    if ~isempty(color)
        cmap = round(255*jet(1024));
        noOfColors = size(cmap,1);

        if min(color) < 0 | max(color) > 1

            color = color+abs(min(color));
            color = color/max(color);

        end

        color_scaled = round((noOfColors-1)*color)+1;
        rgb = cmap(color_scaled,:);
        rgb = kron(rgb,ones(4,1));
    end

    ix = coordinates(1);
    iy = coordinates(2);
    iz = coordinates(3);

    jump = 4*noOfBoxes;
    
    % Points 1 2 3 4
    xtmp=[b(:,ix)-b(:,ix+d) b(:,ix)+b(:,ix+d) b(:,ix)+b(:,ix+d) b(:,ix)-b(:,ix+d)]; 
    ytmp=[b(:,iy)-b(:,iy+d) b(:,iy)-b(:,iy+d) b(:,iy)-b(:,iy+d) b(:,iy)-b(:,iy+d)];
    ztmp=[b(:,iz)-b(:,iz+d) b(:,iz)-b(:,iz+d) b(:,iz)+b(:,iz+d) b(:,iz)+b(:,iz+d)];
    
    x = [x; reshape(xtmp',jump,1)];
    y = [y; reshape(ytmp',jump,1)];
    z = [z; reshape(ztmp',jump,1)]; 

     % Points 5 6 7 8
    xtmp=[b(:,ix)-b(:,ix+d) b(:,ix)+b(:,ix+d) b(:,ix)+b(:,ix+d) b(:,ix)-b(:,ix+d)]; 
    ytmp=[b(:,iy)+b(:,iy+d) b(:,iy)+b(:,iy+d) b(:,iy)+b(:,iy+d) b(:,iy)+b(:,iy+d)];
    ztmp=[b(:,iz)-b(:,iz+d) b(:,iz)-b(:,iz+d) b(:,iz)+b(:,iz+d) b(:,iz)+b(:,iz+d)]; 
    
    x = [x; reshape(xtmp',jump,1)];
    y = [y; reshape(ytmp',jump,1)];
    z = [z; reshape(ztmp',jump,1)];

    Data.vertex = [x y z];

    vertex_indices(6*noOfBoxes,4) = 0;
    
    for i=1:noOfBoxes
        vertex_indices((i-1)*noOfFaces+1,:) = (i-1)*4:i*4-1;
        vertex_indices((i-1)*noOfFaces+2,:) = [jump+(i-1)*4+1 jump+(i-1)*4 jump+(i-1)*4+3 jump+(i-1)*4+2];
        vertex_indices((i-1)*noOfFaces+3,:) = [(i-1)*4+1 (i-1)*4 jump+(i-1)*4 jump+(i-1)*4+1];
        vertex_indices((i-1)*noOfFaces+4,:) = [(i-1)*4+3 (i-1)*4+2 jump+(i-1)*4+2 jump+(i-1)*4+3];
        vertex_indices((i-1)*noOfFaces+5,:) = [(i-1)*4 (i-1)*4+3 jump+(i-1)*4+3 jump+(i-1)*4];
        vertex_indices((i-1)*noOfFaces+6,:) = [(i-1)*4+1 jump+(i-1)*4+1 jump+(i-1)*4+2 (i-1)*4+2];
    end    
    
    Data.face = vertex_indices;
    
    if nargin > 1
        if ~isempty(color)
            Data.Color = repmat(rgb,2,1);
        end
    end

end
