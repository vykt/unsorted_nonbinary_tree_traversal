#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "tree.h"

#define MAX_TREE 128


void display_tree_layer(tree_t * tree_entry, uint8_t depth) {

	int depth_child = depth + 1;
	for (int i = 0; i < depth; i++) {
		putchar(' ');
		putchar(' ');
	}

	printf("%s\n", tree_entry->data);

	//Now check sublevels. for each child
	for (int i = 0; i < tree_entry->child_count; i++) {

		display_tree_layer(tree_entry->child_node[i], depth_child);

	} //End for each child
}


void display_tree(tree_t * tree_root) {

	//First, check if tree exists
	if (tree_root == 0) {
		printf("Tree not yet initialised.\n");
	//Else it is initialised
	} else {
		printf("Tree:\n");
		//Recursively print tree
		display_tree_layer(tree_root, 0);

	} //End else
}


void display_menu() {

	//Compiler should clean this up

	puts("          --- MENU ---");
	puts("Enter string input, under 256 chars. Any of the following options:");
	puts("\nadd <index> <string> <method> - Add entry to tree at <index>, of value");
	puts("                                <string> using <method> search algorithm.");
	puts("check <index> <method>        - Check if <index> exists.");
	puts("show tree                     - Print tree");
	puts("\nE.g.: add hello world depth");
	puts("E.g.: check world breadth");
	puts("E.g.: show tree");
	puts("\nRefer to README.md for details.");
}


//Initialise new tree entry by allocating memory for it & setting some default values.
void init_entry(tree_t ** tree_entry, char inp[DATA_SIZE]) {

	*tree_entry = malloc(sizeof(tree_t));
	if (*tree_entry == NULL) {
		printf("Failed to initialise tree entry for inp: %s.\n", inp);
		exit(1);
	}

	strcpy((* tree_entry)->data, inp);
	for (int i = 0; i < MAX_CHILD; i++) {
		(* tree_entry)->child_node[i] = NULL;
	}	
}


/*
 *	The following search functions take index string, and return whether the
 *	entry exists within the tree structure. The pointer to the entry is stored
 *	inside 'entry'.
 */

int depth_search(tree_t ** tree_entry, char index[DATA_SIZE], tree_t ** entry) {


	if (!strcmp((* tree_entry)->data, index)) {
		*entry = *tree_entry;
		return DFS_MATCH;
	}

	int ret;

	//For each child
	for (int i = 0; i < (* tree_entry)->child_count; i++) {
		ret = depth_search(&(* tree_entry)->child_node[i], index, entry);
		if (ret == DFS_MATCH) return DFS_MATCH;
	} //End for each child	

	return DFS_NO_MATCH;

}


int breadth_search(tree_t ** tree_entry, bmeta_t * meta, char index[DATA_SIZE], tree_t ** entry) {

	//Run on initial call
	if (meta == NULL) {

		bmeta_t meta_def;

		meta_def.depth_des = 0;
		meta_def.depth_cur = 0;
		meta_def.depth_des_found = 0;
	
		//Check if matches (node data == index)
		if (!strcmp((* tree_entry)->data, index)) {
			*entry = *tree_entry;
			return BFS_MATCH;
		}

		int ret;

		//Keep going down layers until 
		do {
			meta_def.depth_des_found = 0;
			meta_def.depth_des = meta_def.depth_des + 1;

			for (int i = 0; i < (* tree_entry)->child_count; i++) {

				meta_def.depth_cur = meta_def.depth_cur + 1;
				ret = breadth_search(&(* tree_entry)->child_node[i], &meta_def, index, entry);
				if (ret == BFS_MATCH) return BFS_MATCH;
				meta_def.depth_cur = meta_def.depth_cur - 1;
			}

		} while (meta_def.depth_des_found == 1);
	
		return BFS_NO_MATCH;


	//Run on every recurse
	} else {

		int ret;

		//If this node is on correct depth & matches searched for term
		if (meta->depth_des == meta->depth_cur) {
			meta->depth_des_found = 1;
			//If matches
			if (!strcmp((* tree_entry)->data, index)) {
				*entry = *tree_entry;
				meta->depth_cur = meta->depth_cur - 1;
				return BFS_MATCH;
			}
		}

		//For each child
		if (meta->depth_des > meta->depth_cur) {
			meta->depth_cur = meta->depth_cur + 1;
			for (int i = 0; i < (* tree_entry)->child_count; i++) {
				meta->depth_des_found = 1;
				ret = breadth_search(&(* tree_entry)->child_node[i], meta, index, entry);
				if (ret == BFS_MATCH) {
					meta->depth_cur = meta->depth_cur - 1;
					return BFS_MATCH;
				}
			}
		}
		//meta->depth_cur = meta->depth_cur - 1;
		return BFS_NO_MATCH;

	} //End recurse section
}


//Return first appearance of string in tree
int check_entry(tree_t ** tree_root, char index[DATA_SIZE], int method) {

	//If tree uninitiated
	if (*tree_root == 0) return CHECK_FAIL;

	tree_t * entry = 0;

	//Methods to find index
	if (method == METHOD_DEPTH) {
		return depth_search(tree_root, index, &entry);

	} else if (method == METHOD_BREADTH) {
		return breadth_search(tree_root, NULL, index, &entry);

	} //End methods

	return CHECK_FAIL;
}


//Add entry to tree
int add_entry(tree_t ** tree_root, char index[DATA_SIZE],
			  char addition[DATA_SIZE], int method) {

	//If tree uninitiated
	if (*tree_root == 0) {
		init_entry(tree_root, addition);
		return ADD_SUCCESS;
	}

	int ret;
	tree_t * entry = 0;

	//Methods to find index
	if (method == METHOD_DEPTH) {

		ret = depth_search(tree_root, index, &entry);
		if (ret == DFS_MATCH) {
			init_entry(&entry->child_node[entry->child_count], addition);
			entry->child_count = entry->child_count + 1;
			return ADD_SUCCESS;
		} else if (ret == DFS_NO_MATCH) {
			return ADD_FAIL;
		}

	} else if (method == METHOD_BREADTH) {

		ret = breadth_search(tree_root, NULL, index, &entry);
		if (ret == BFS_MATCH) {
			init_entry(&entry->child_node[entry->child_count], addition);
			entry->child_count = entry->child_count + 1;
			return ADD_SUCCESS;
		} else if (ret == BFS_NO_MATCH) {
			return ADD_FAIL;
		}

	} //End methods
	return ADD_FAIL;
}


int get_inp(char index[DATA_SIZE], char string[DATA_SIZE], int * method_int) {

	char add_or_check = 1;
	char command[DATA_SIZE] = {0};
	char method[DATA_SIZE] = {0};
	
	//Zero out input buffers
	for (int i = 0; i < DATA_SIZE; i++) {
		index[i] = 0;
		string[i] = 0;
	}

	fflush(stdin); //Remnant of trying to save the poor input buffer
				   //from the inadequacies of scanf(). Vainful effort.

	printf("input > ");

	//Get input
	if (scanf("%255s", command) == EOF) return INP_ERR;
	if (scanf("%255s", index) == EOF) return INP_ERR;

	//Check for SHOW TABLE
	if (!strcmp(command, "show")) {
		if (!strcmp(index, "tree")) {
			return INP_SHOW_TABLE;
		} else {
			return INP_ERR;
		}
	}

	//Get string term if ADD
	if (!strcmp(command, "add")) {
		if(scanf("%255s", string) == EOF) return INP_ERR;
		add_or_check = 0;
	}

	//Decide on search method
	if (scanf("%255s", method) == EOF) return INP_ERR;


	if (!strcmp(method, "breadth")) {
		*method_int = METHOD_BREADTH;	
	} else if (!strcmp(method, "depth")) {
		*method_int = METHOD_DEPTH;
	} else {
		return INP_ERR;
	}

	//Decide whether to return ADD or CHECK
	if (!add_or_check) {
		return INP_ADD;
	} else {
		return INP_CHECK;
	}
}


int main(int argc, char ** argv) {

	//Define tree
	tree_t * tree_root = 0;
	
	char index[DATA_SIZE] = {0};
	char string[DATA_SIZE] = {0};
	int method = 0; //depth = 1, breadth = 0
	int ret_inp;
	int ret_func;

	printf("Press Ctrl+C to terminate.\n\n");	
	display_menu();

	while (1) {

		ret_inp = get_inp(index, string, &method);

		if (ret_inp == INP_ERR) puts("Invalid input.");
		if (ret_inp == INP_SHOW_TABLE) display_tree(tree_root);
		if (ret_inp == INP_ADD) {
			ret_func = add_entry(&tree_root, index, string, method);
			switch(ret_func) {
				case ADD_SUCCESS:
					puts("Number added.");
					break;
				case ADD_FAIL:
					puts("Failed to add number. Index not found.");
			}
		} else if (ret_inp == INP_CHECK) {
			ret_func = check_entry(&tree_root, index, method);
			switch(ret_func) {
				case CHECK_SUCCESS:
					puts("Found in tree.");
					break;
				case CHECK_FAIL:
					puts("Not found in tree, or tree is not initiated.");
					break;
				default:
					puts("Check option default. This should never happen.");
					break;
			}
		}

	} // While end

	/*
	ret = get_inp(index, string, &method);
	printf("ret: %d, index: %s, string: %s, method: %d\n", ret, index, string, method);


	char d1[DATA_SIZE] = {0};
	char d2[DATA_SIZE] = {0};
	char d3[DATA_SIZE] = {0};
	char d4[DATA_SIZE] = {0};

	strcpy(d1, "hello");
	strcpy(d2, "world");
	strcpy(d3, "gentoo");
	strcpy(d4, "uwu");

	init_entry(&tree_root, d1);
	init_entry(&tree_root->child_node[0], d2);
	init_entry(&tree_root->child_node[1], d3);
	init_entry(&tree_root->child_node[0]->child_node[0], d4);

	tree_root->child_count = 2;
	tree_root->child_node[0]->child_count = 1;

	//printf("Root node: %s\n", tree_root->data);

	//Call with NULL meta first to init search
	//ret = depth_search(&tree_root, string, &entry);
	ret = add_entry(&tree_root, index, string, method);
	//tree_root->child_node[1]->child_count = 1;
	printf("ret: %d\n", ret);

	//printf("entry data: %s\n", entry->data);
	display_tree(tree_root);
	*/


	return 0;
}
