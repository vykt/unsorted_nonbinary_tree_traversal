# unsorted_noninary_tree_traversal


### ABOUT:

This is an efficient implementation of unsorted nonbinary trees in C. Each node on the
tree is represented by a struct holding addresses of nodes under it. Each node can have
up to 256 subnodes. The tree uses minimal memory by only allocating space on the heap
for each used node. The table is can be traversed using either breadth first search 
(BST) or depth first search (DST), both implemented recursively.



### INSTALLATION:

Build:
```
make
```

If gcc & make are unavailable, an ELF64 executable can be found inside ./bin.



### RUN:

Run the application:
```
./tree
```



### INTERACT:

A menu is printed for the user upon launching the application. The following options
are available:
```
add <index> <string> <method> - Add the entry <string> under the first node matching
                                <index>, using <method> search algorithm where <method>
								is one of 'breadth' or 'depth' (quotes excluded).

check <index> <method>        - Check for the existence of node matching <index> using
                                <method> search algorithm, where <method> is one of
								'breadth' or 'depth' (quotes excluded).

show tree                     - Display the tree. 
```

Before the ```check``` or ```show``` commands can be run, the table needs to be
initiated by running ```add``` at least once. During initiation, the ```index``` field
is ignored, but must still be provided.

String input should not exceed 256 characters, otherwise ```scanf()``` will break. Other
methods of input validation are too tedious and detract attention from the tree itself.

Each node has a maximum of 256 children. Exceeding this limit is undefined behaviour.



### EXAMPLES:

Add node "world", under the first node matching "hello", using the breadth first search:
```
add hello world breadth
```

Check whether the node "world" is present in the tree using depth first search:
```
check world depth
```

Display the tree:
```
show tree
```
