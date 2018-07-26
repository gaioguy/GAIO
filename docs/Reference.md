## Tree

```matlab
tree = Tree(C, R) 
```
creates an empty tree with an outer box of center C and radius R.
```matlab
tree = Tree('filename') 
```
loads a tree from file 'filename'.

### Tree attributes

* `tree.dim` dimension of state space
* `tree.depth` depth of the tree, i.e. number of levels
* `tree.center` center of the outer box Q
* `tree.center = c` sets center of outer box Q to c
* `tree.radius` radius of the outer box Q
* `tree.radius = r` sets the radius of outer box Q to r
* `tree.sd` vector which defines in which coordinate direction nodes/boxes on a certain depth will be subdivided. Boxes on depth i will be subdivided in coordinate direction `tree.sd[i]`
* `tree.sd = sd` sets the vector `tree.sd` to `sd`

### Tree methods

* `tree.boxes(depth)` returns the box collection on depth `depth` (use `depth = -1` for the leaves). Each column of the returned ((2*dim+2) x m)-matrix (where m is number of boxes on depth `depth`) describes one box B(c,r) in the format `[center; radius; flags; color]`
* `tree.change_flags(which, from, to, depth)` see set_flags, changes the flags at the given depth from `from` to `to`
* `tree.count(depth)` enumerates the boxes and returns the number of boxes on depth `depth`
* `tree.delete(depth)` deletes all boxes on depth `depth`, i.e. afterwards the tree has at most depth `depth-1`
* `box = tree.first_box(depth)` positions onto the first box on depth `depth` and returns its geometry in the format `[center; radius; flags; color]`
*  `tree.insert(x, depth [, flag0, flag1, color])` inserts the boxes on depth `depth` which contain the points `x`. Each newly created box gets the flag `flag0` (default=2), others the flag `flag1` (default=0). You may specify a vector c (length(x)=length(c)) determining the color of the inserted boxes. The values of c must lie between 1 and 255
* `box = tree.next_box(depth)` positions onto the next box on depth `depth` and returns its geometry in the format `[center; radius; flags; color]`
* `tree.remove(flag)` removes all leaves b (and corresponding parents) for which b->flags!=f
* `tree.save('filename')` saves the tree on disk in file 'filename'
* `tree = Tree('filename')` loads tree which was saved in file 'filename'
* `tree.search(x [, depth])` searches for the points given by the columns of x on depth `depth` (default = -1). Returns a vector of length size(x,2), each entry of this vector either denotes the number of the box the corresponding point is contained in or is -1. **You have to call `tree.count(depth)` before using `tree.search` in order to enumerate the boxes on depth `depth`.**
* `tree.search_box(center, radius [, depth])` finds all boxes on depth `depth` (default = -1) which have a nonempty intersection with the rectangle [c-r,c+r].  Returns a vector containing the numbers of the boxes found or a vector of length 0 if no boxes were found. **You have to call `tree.count(depth)` before using `tree.search` in order to enumerate the boxes on depth `depth`.
* `tree.set_flags(which, flags [, depth])` sets flags f in the i-th box on depth `depth`(default=-1) if the i-th character of string `which` is a 1. For `w=='all'` all boxes are flagged.
* `tree.set_flags(x, flags [, depth])` sets flags `flags` in those boxes on depth `depth` which contain at least one point from the columns of the matrix `x` (which is of size dim x m, where m is the number of points). Returns 1, if the flag of some box was set, otherwise 0.
* `tree.subdivide(flags)` subdivides all leaves which have the flags `flags` set. Returns the number of subdivided boxes.
* `tree.unset_flags(which, flags [, depth])` see `set_flags`, but unsets the flags.
* `tree.unsubdivide(flags)` unsubdivides all leaves which have the flags `flags` set.
