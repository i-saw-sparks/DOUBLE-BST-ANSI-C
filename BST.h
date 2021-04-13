

#ifndef BST_BST_H
#define BST_BST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct item_node_struct
{
    char name[32];
    int count;
    struct item_node_struct *left, *right;
};
typedef struct item_node_struct item_node;

struct tree_name_node_struct
{
    char treeName[32];
    struct tree_name_node_struct *left, *right;
    item_node *theTree;
};
typedef struct tree_name_node_struct tree_name_node;

tree_name_node* treeInit(const char[]);

tree_name_node* newTreeNameNode(const char[]);
item_node* newItemNode(const char[], int);

tree_name_node* insertTreeNameNode(tree_name_node*, const char[]);
void insertItemNode(tree_name_node*, const char[], int);
item_node *insertItemNodeAux(item_node*, const char[], int);

tree_name_node* search_for_name_node(tree_name_node*, const char*);
item_node* search_in_name_node(tree_name_node*, const char*);
item_node* searchItemAux(item_node*, const char[]);
int getItemCountBefore(tree_name_node*, item_node*);
int getItemCountBeforeAux(item_node*, item_node*);
int getRightHeight(tree_name_node*);
int getLeftHeight(tree_name_node*);
int getHeight(item_node*);
int getCount(tree_name_node*);
int getTreeCount(tree_name_node*);
int getCountAux(item_node*);
item_node *get_item_before(item_node*, item_node*);
tree_name_node *get_tree_before(tree_name_node* , tree_name_node*);

int item_is_leaf(item_node*);
int tree_node_is_leaf(tree_name_node *);

item_node* get_highest(item_node*);
item_node* get_lowest(item_node*);

tree_name_node * get_highest_tree(tree_name_node *);
tree_name_node * get_lowest_tree(tree_name_node *);

void deleteItemNode(tree_name_node*, item_node*);
void deleteTreeNameNode(tree_name_node*, tree_name_node*);
void deleteAll(tree_name_node*);

#endif //BST_BST_H
