function t = Tree(arg0, arg1, arg2)

% Tree - create a tree object
%    t = Tree(C, R) creates an empty tree with an outer box
%    of center C and radius R.
%    t = Tree('foo') loads a tree from file 'foo'
%
%    NOTE: call delete(t); clear t; to delete the tree from memory.
% 
% Attributes
%    t.dim               - dimension of state space
%    t.depth             - current depth
%    t.center            - center of the outer box Q
%    t.center = c        - sets center of outer box Q to c
%    t.radius            - radius of the outer box Q
%    t.radius = r        - sets the radius of outer box Q to r
%    t.sd                - vector which defines in which coordinate direction 
%                          boxes on a certain depth will be subdivided (i.e.
%                          boxes on depth i will be subdivided in coordinate 
%                          direction t.sd[i])
%    t.sd = sd           - sets the vector t.sd to sd
% 
% Methods
%    t.boxes(d)              - the box collection on depth d (use d=-1 for 
%                              the leaves). Each column of the returned 
%                              ((2*dim+2) x m)-matrix (where m is number
%                              of boxes on depth d) describes one box B(C,R) in 
%                              the format [C; R; flags; color];
%    t.change_flags(w,f,g,d) - see set_flags, changes the flags from f to g;
%    t.count(d)              - enumerates the boxes and returns the
%                              number of boxes on depth d;
%    t.delete(d)             - deletes all boxes on depth d, i.e. afterwards 
%                              the tree has at most depth d-1;
%    box = t.first_box(d)    - positions onto the first box on
%                              depth d and returns its geometry in
%                              the format [center radius flags color]
%    t.insert(x,d[,f0,f1,c]) - inserts the boxes on depth d which contain the
%                              points x; each newly created box gets
%                              the flag f0 (default=2), others the
%                              flag f1 (default=0); you may specify 
%                              a vector c (length(x)=length(c))
%                              determining the color of the inserted boxes;
%                              the values of c must lie between 1 and 255
%    box = t.next_box(d)     - positions onto the next box on
%                              depth d and returns its geometry in
%                              the format [center radius flags color]
%    t.remove(f)             - Removes all leaves b (and corresponding parents)
%                              for which b->flags!=f;
%    t.save('tree')          - saves tree t on hard-disk in file 'tree'
%    tt = Tree('tree')       - loads tree which was saved into file 'tree' into tt
%    t.search(x [, d])       - searches for the points given by the columns
%                              of x on depth d (default = -1); returns a
%                              vector of length size(x,2), each entry
%                              of this vector either denotes the number
%                              of the box the corresponding point is contained
%                              in or is -1; !! You have to call t.count(d) once
%                              before using t.search in order to enumerate
%                              the boxes on depth d;
%    t.search_box(C,R[,d])   - finds all boxes on depth d (default = -1) of the 
%                              tree which have a nonempty intersection with the 
%                              rectangle B(C,R) with center C and radius R; 
%                              returns a vector containing the numbers of the boxes
%                              found or a vector of length 0 if no boxes were found;
%                              !! You have to call t.count(d) once before using
%                              t.search in order to enumerate the boxes on depth d;
%    t.set_flags(w, f, d)    - sets flags f in the i-th box on depth d 
%                              if the i-th character of string w is a '1'
%                              if w=='all', all boxes are considered;
%    t.set_flags(x, f, d)    - sets flags f in the boxes on depth d which
%                              contain the points x; returns 1, if the flag
%                              of some box was set, otherwise 0;
%    t.subdivide(f)          - subdivides all leaves which have the flags f 
%                              set; returns the number of subdivided boxes;
%    t.unset_flags(w, f, d)  - see set_flags, but unsets the flags;
%    t.unsubdivide(f)        - Unsubdivides all leaves which have the flag f 
%                              set; returns the number of unsubdivided boxes;

% (c) 2013 djs2 GbR

if nargin==0
  error('Too few input arguments: filename or center/radius expected.')
elseif nargin==1
  if ~ischar(arg0)
    error('filename expected.');
  end
  t.handle = load_tree(arg0);
  t = class(t, 'Tree');
elseif nargin==2
  if (length(arg1)~=length(arg0))
    error('center and radius must have the same dimension.');
  end
  t.handle = tree_(length(arg0), arg0, arg1);
  t = class(t, 'Tree');
end

