
The core of GAIO is a binary tree for representing the box collections. A GAIO tree is initialized by specifying a rectangle via its center and its radius:
```matlab
>> center = [0 0]
>> radius = [2 2]
>> tree = Tree(center, radius)
```
The resulting tree only consists of the root node:
```matlab
>> tree.depth
ans =
     0
>> depth = 0
>> tree.count(depth)
ans =
     1
```
The `count` method returns the number of nodes on the specified `depth`of the tree.  `depth = -1` will return the number of leaves of the tree. In this case, since the tree only consists of the root node, this would give the same result:
```matlab
>> tree.count(-1)
ans =
     1
```
### boxplot
You can visualize the boxes on a certain `depth` in 2D or 3D with `boxplot2`/`boxplot3`:
```matlab
>> boxplot2(t)
```
<img src="boxplot_1.png" width="600px"/>

Since the tree only contains the root node, only a single box is shown. You can also output the boxes in matrix form:
```matlab
>> b = tree.boxes(0)
b =
     0
     0
     2
     2
     0
     1
```
Each column of the matrix `b` corresponds to one box, with the first d entries specifying the center (here [0 0]), the next d entries the radius (here [2 2]), the next entry some flags (here 0) and the last entry some additional information (largely unused).

### Flags

A node in the tree can be given different flags which can later be used in order to, e.g., subdivide a node/box:
```matlab
>> sd = 8;
>> t.set_flags('all', sd);
>> tree.subdivide(sd);
>> tree.depth
ans = 
     1
```
Here, all leaves of the tree (which is only the root so far) are given the flag `sd` (which has arbitrarily been set to `8` here).  Then, all leaves which carry the flag `sd` are subdivided, i.e. for each leaf which carries the flag `sd`, two children are generated.  In this case, since we started with the tree consisting of the root node only, two children of the root are generated, so that the tree has depth 1 now, with two leaves existing at depth 1:
```matlab
>> boxplot2(t)
```
<img src="boxplot_2.png" width="600px"/>

You can also set flags in certain nodes only. The most common case is that you have a set of points and you would like to set some flag in each node/box which contains at least one of these points:
```matlab
>> x = [1 1];
>> tree.set_flags(x, sd);
>> tree.boxes(1)
ans =
         -0.5          0.5
            0            0
          0.5          0.5
            1            1
            0            8
            1            1
```
This is particularly useful if you would like to selectively remove certain nodes/boxes. For example, in our situation, we could selectively remove all nodes/boxes which *do not* have the flag `sd` set:
```matlab
>> tree.remove(sd)
>> tree.boxes(1)
ans =
          0.5
            0
          0.5
            1
           16
            1
```
Internally, the flags of a node are stored in a single byte which is interpreted as a bitstring of 0/1 flags.  When using `tree.set_flags('all', sd)`, the least significant byte of `sd` is logically ORed with the flag byte of each node, i.e. `tree.set_flags('all',8)` yields the same result as `tree.set_flags('all',520)`.

### Looping over nodes

The tree methods `first_box` and `next_box`enable an iteration over the nodes at a given depth of the tree.  In our case, the iteration will loop over the two nodes which we just generated at depth 1. For example, we could output the centers and the flags of the boxes at depth 1:
```matlab
>> depth = 1; dim = 2;
>> b = tree.first_box(depth);                         
>> while (~isempty(b))
      center = b(1:dim)
      flags = b(2*dim+1)                      
      b = tree.next_box(depth);                      
   end
center =
    -1
     0
flags =
     0
center =
     1
     0
flags =
     0
```
A complete list of attributes and methods of the GAIO tree data structures is given at ...
