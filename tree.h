#ifndef BTREE_H
#define BTREE_H

#include <stdint.h>

#define DATA_SIZE 256
#define MAX_CHILD 256

#define CHECK_SUCCESS 0
#define CHECK_FAIL 1

#define ADD_SUCCESS 0
#define ADD_FAIL 1

#define BFS_MATCH 0
#define BFS_NO_MATCH 1

#define DFS_MATCH 0
#define DFS_NO_MATCH 1

#define INP_ERR -1
#define INP_SHOW_TABLE -2
#define INP_ADD -3
#define INP_CHECK -4

#define METHOD_BREADTH 0
#define METHOD_DEPTH 1

typedef struct tree tree_t;
typedef struct breadth_search_metadata bmeta_t;
typedef struct depth_search_metadata dmeta_t;


struct tree {

	char data[DATA_SIZE];
	uint8_t child_count;
	tree_t * child_node[MAX_CHILD];

};


struct breadth_search_metadata {

	uint8_t depth_des; //desired depth
	uint8_t depth_cur; //current depth

	uint8_t depth_des_found; //managed to reach desired depth 

};

#endif
