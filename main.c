#include <stdio.h>
#include <stdlib.h>

struct item_node_struct {
    char name[32];
    int count;
    struct item_node_struct *left, *right;
};
typedef struct item_node_struct item_node;

struct tree_name_node_struct {
    char treeName[32];
    struct tree_name_node_struct *left, *right;
    item_node *theTree;
};
typedef struct tree_name_node_struct tree_name_node;

void executeCommand(tree_name_node *, char *, char *, char *);
void printTreeNotFound(char *);
void printItemNotFound(char *, char *);





tree_name_node *treeInit(const char[]);

tree_name_node *newTreeNameNode(const char[]);

item_node *newItemNode(const char[], int);

tree_name_node *insertTreeNameNode(tree_name_node *, const char[]);

void insertItemNode(tree_name_node *, const char[], int);

item_node *insertItemNodeAux(item_node *, const char[], int);

tree_name_node *search_for_name_node(tree_name_node *, const char *);

item_node *search_in_name_node(tree_name_node *, const char *);

item_node *searchItemAux(item_node *, const char[]);

int getItemCountBefore(tree_name_node *, item_node *);

int getItemCountBeforeAux(item_node *, item_node *);

int getRightHeight(tree_name_node *);

int getLeftHeight(tree_name_node *);

int getHeight(item_node *);

int getCount(tree_name_node *);

int getTreeCount(tree_name_node *);

int getCountAux(item_node *);

void traverse_in_order(tree_name_node *, int);

void print_tree_in_order(item_node *);

void print_name_tree_in_order(tree_name_node *);

item_node *get_item_before(item_node *, item_node *);

tree_name_node *get_tree_before(tree_name_node *, tree_name_node *);

int item_is_leaf(item_node *);

int tree_node_is_leaf(tree_name_node *);

item_node *get_highest(item_node *);

item_node *get_lowest(item_node *);

tree_name_node *get_highest_tree(tree_name_node *);

tree_name_node *get_lowest_tree(tree_name_node *);

void deleteItemNodeAux(tree_name_node *, item_node *, item_node *);

void deleteItemNode(tree_name_node *, item_node *);

void deleteTreeNameNode(tree_name_node *, tree_name_node *);

void deleteTreeNameNodeAux(tree_name_node *, tree_name_node *, tree_name_node *);

void deleteAll(tree_name_node *);

int main() {
    FILE *fptr;
    tree_name_node *tree = NULL;
    char fileName[100], line[63];
    int treeN = 0, itemsN = 0, commandsN = 0, lineCounter = 0, wasPrinted = 0;

    fputs("Please, type the name of the input file: ", stdout);
    fgets(fileName, 100, stdin);
    fileName[strcspn(fileName, "\n")] = 0;

    //Opens the file and chekc if it is open
    if ((fptr = fopen(fileName, "r")) == NULL) {
        printf("Error ... main(): File not found");
        exit(1);
    }

    //For every line in the file, the line will be read and processed in the while loop
    while (fgets(line, sizeof(line), fptr) != NULL) {
        lineCounter++;
        line[strcspn(line, "\n")] = 0;

        /*
         * if line counter == 1: scans the first 3 numbers of the file (number of trees, number of items and number of commands
         */
        if (lineCounter == 1) {
            sscanf(line, "%d%d%d", &treeN, &itemsN, &commandsN);
        } else if (lineCounter - 1 <= treeN) {
            /*
             * This block is were the trees are read and inserted. The first inserted tree
             * will be the "main" tree, and the reference for every other tree.
             */
            if (tree == NULL) {
                tree = treeInit(line);
            } else {
                insertTreeNameNode(tree, line);
            }
        } else if (lineCounter - 1 - treeN <= itemsN) {
            /*
             * This block is were the items are read and inserted. With the function strtok
             * the code splits the string, and gets the tree name, the item name and the
             * counter of every item.
             */
            char treeName[32], itemName[32], counterStr[32];
            int counter;
            strcpy(treeName, strtok(line, " "));
            strcpy(itemName, strtok(NULL, " "));
            strcpy(counterStr, strtok(NULL, " "));
            sscanf(counterStr, "%d", &counter);

            /*
             * The code searches for the tree name. If it is found, the item is inserted.
             */
            tree_name_node *aux = search_for_name_node(tree, treeName);
            if (aux == NULL){
                printTreeNotFound(treeName);
            }else{
                insertItemNode(search_for_name_node(tree, treeName), itemName, counter);
            }

        } else if (lineCounter - 1 - treeN - itemsN <= itemsN) {
            /*
             * This block is were the commands are read. If it is
             * the first time in this block, the code prints all the trees
             * with the traverse_in_order function.
             */
            if (wasPrinted == 0) {
                traverse_in_order(tree, 1);
                fputs("=====Processing Commands=====\n", stdout);

                wasPrinted = 1;
            }

            char cmd[15], treeName[32], itemName[32];
            sscanf(line, "%s%s%s", cmd, treeName, itemName);
            executeCommand(tree, treeName, itemName, cmd);
        }
    }
    deleteAll(tree);
    fclose(fptr);
}

/*
 * This function is were every command is interpretated and executed
 */
void executeCommand(tree_name_node *root, char *treeName, char *itemName, char *cmd) {
    if (strncmp(cmd, "search", 6) == 0) {
        /*
         * This block is where the "search" command is executed. It searches for the tree
         * with the search_for_name_node function and the name with tht search_in_name_node.
         * If the item was found, it shows the count of the item.
         */
        tree_name_node *tree = search_for_name_node(root, treeName);
        if (tree == NULL) {
            printTreeNotFound(treeName);
        } else {
            item_node *node = search_in_name_node(tree, itemName);
            if (node == NULL) {
                printItemNotFound(treeName, itemName);
            } else {
                printf("%i ", node->count);
                fputs(itemName, stdout);
                fputs(" found in ", stdout);
                fputs(treeName, stdout);
                fputs("\n", stdout);
            }
        }
    } else if (strncmp(cmd, "item_before", 11) == 0) {
        /*
         * This block is where the "item_before" command is executed. It checks if
         * the tree and item exists. If both exist print the count of items before
         * calculated with the function getItemCountBefore.
         */
        tree_name_node *tree = search_for_name_node(root, treeName);
        if (tree == NULL) {
            printTreeNotFound(treeName);
        } else {
            item_node *node = search_in_name_node(tree, itemName);
            if (node == NULL) {
                printItemNotFound(treeName, itemName);
            } else {
                fputs("items before ", stdout);
                fputs(itemName, stdout);
                printf(": %d\n", getItemCountBefore(tree, node));
            }
        }
    } else if (strncmp(cmd, "height_balance", 14) == 0) {
        /*
         * This block is where the "height_balance" command is executed. It checks if
         * the tree exists. The function gets the right height and the left height of the
         * tree, and calculates the difference.
         */
        tree_name_node *tree = search_for_name_node(root, treeName);
        if (tree == NULL) {
            printTreeNotFound(treeName);
        } else {
            int rightHeight = getRightHeight(tree);
            int leftHeight = getLeftHeight(tree);
            int difference = rightHeight - leftHeight;
            fputs(treeName, stdout);
            printf(": left height %d, right height %d, difference %d, ", leftHeight, rightHeight, difference);
            if (difference == 0) {
                fputs("balanced\n", stdout);
            } else {
                fputs("not balanced\n", stdout);
            }
        }
    }  else if (strncmp(cmd, "count", 5) == 0) {
        /*
         * This block is where the "count" command is executed. It checks if
         * the tree exists. The function gets the total count of the items it
         * is holding with the function getCount.
         */
        tree_name_node *tree = search_for_name_node(root, treeName);
        if (tree == NULL) {
            printTreeNotFound(treeName);
        } else {
            fputs(treeName, stdout);
            printf(" count %d\n", getCount(tree));
        }
    } else if (strncmp(cmd, "delete_tree", 11) == 0) {
        /*
         * This block is where the "delete_tree" command is executed. It searches
         * the tree exists. The function deletes the name node with the function
         * deleteTreeNameNode
         */
        tree_name_node *tree = search_for_name_node(root, treeName);
        if (tree == NULL) {
            printTreeNotFound(treeName);
        } else {
            deleteTreeNameNode(root, tree);
            fputs(treeName, stdout);
            fputs(" deleted\n", stdout);
        }
    } else if (strncmp(cmd, "delete", 6) == 0) {
        /*
         * This block is where the "delete" command is executed. It searches
         * the tree and the item exists. The function deletes the item node with the function
         * deleteItemNode
         */
        tree_name_node *tree = search_for_name_node(root, treeName);
        if (tree == NULL) {
            printTreeNotFound(treeName);
        } else {
            item_node *node = search_in_name_node(tree, itemName);
            if (node == NULL) {
                printItemNotFound(treeName, itemName);
            } else {
                deleteItemNode(tree, node);
                fputs(itemName, stdout);
                fputs(" deleted from ", stdout);
                fputs(treeName, stdout);
                fputs("\n", stdout);
            }
        }
    }
}

/*
 * This function prints an error message when a tree
 * was not found
 */
void printTreeNotFound(char *treeName) {
    fputs(treeName, stdout);
    fputs(" does not exist\n", stdout);
}

/*
 * This function prints an error message when an item
 * was not found
 */
void printItemNotFound(char *treeName, char *itemName) {
    fputs(itemName, stdout);
    fputs(" not found in ", stdout);
    fputs(treeName, stdout);
    fputs("\n", stdout);
}

#include "BST.h"

/*
 * This function creates a tree and returns it
 */
tree_name_node *treeInit(const char name[]) {
    tree_name_node *name_node = newTreeNameNode(name);
    return name_node;
}

/*
 * This function allocates a new tree, initilize its values
 * and returns it.
 */
tree_name_node *newTreeNameNode(const char name[]) {
    tree_name_node *name_node;
    name_node = (tree_name_node *) malloc(sizeof(tree_name_node));

    if (name_node == NULL) {
        printf("Error ... newTreeNameNode(): Not enough RAM memory");
        fflush(stdout);
        exit(1);
    }

    name_node->left = NULL;
    name_node->right = NULL;
    name_node->theTree = NULL;
    strcpy(name_node->treeName, name);

    return name_node;
}

/*
 * This function allocates a new item, initilize its values
 * and returns it.
 */
item_node *newItemNode(const char name[], int count) {
    item_node *node;
    node = (item_node *) malloc(sizeof(item_node));

    if (node == NULL) {
        printf("Error ... newItemNode(): Not enough RAM memory");
        fflush(stdout);
        exit(1);
    }

    node->left = NULL;
    node->right = NULL;
    node->count = count;
    strcpy(node->name, name);

    return node;
}

/*
 * This function is inserts a new tree name node in another tree name node that is
 * received as parameter.
 * If the name of the node that will be inserted is minor (lexicographically) than
 * the name of the node passed as argument, the left node of the reference node will be the
 * return value of the same function called with the left node as argument.
 * This means that the tree will be scanned, searching for a node that can contain the new node
 * keeping the lexicographic order unchanged.
 */
tree_name_node *insertTreeNameNode(tree_name_node *self, const char *name) {
    if (self == NULL) {
        return newTreeNameNode(name);
    } else {
        if (strcmp(name, self->treeName) < 0) {
            self->left = insertTreeNameNode(self->left, name);
        } else {
            self->right = insertTreeNameNode(self->right, name);
        }
    }
    return self;
}


/*
 * This function is the starting point of the item insertion process.
 * It is the "interface" method that takes the tree where the item will
 * be inserted
 */
void insertItemNode(tree_name_node *self, const char *name, int count) {
    if (self == NULL) {
        return;
    }
    self->theTree = insertItemNodeAux(self->theTree, name, count);
}

/*
 * This function is the recursvie part of the item insertion process.
 * the function searches for a node that can contain the new node
 * keeping the lexicographic order unchanged.
 */
item_node *insertItemNodeAux(item_node *self, const char *name, int count) {
    if (self == NULL) {
        return newItemNode(name, count);
    } else {
        if (strcmp(name, self->name) < 0) {
            self->left = insertItemNodeAux(self->left, name, count);
        } else {
            self->right = insertItemNodeAux(self->right, name, count);
        }
    }
    return self;
}

/*
 * This function is a recursive function that searched for a name node in the tree.
 * It does not check every node of the tree, instead it comparess the name recevied
 * as a parameter with the name of the node recevied as a parameter.
 * If the name is less than the node name, it calls itself with the left sub-tree of
 * the node as argument. If the name is grater, it calls itself with the right sub-tree of
 * the node as argument.
 */
tree_name_node *search_for_name_node(tree_name_node *node, const char *name) {
    if (node == NULL) {
        return NULL;
    } else {
        int n = strlen(name);
        /*
         * if the node name and the name coincide, the function returns the node.
         */
        if (strncmp(node->treeName, name, n) == 0) {
            return node;
        } else {
            if (strcmp(name, node->treeName) < 0) {
                if (node->left == NULL) {
                    return NULL;
                } else {
                    return search_for_name_node(node->left, name);
                }
            } else {
                if (node->right == NULL) {
                    return NULL;
                } else {
                    return search_for_name_node(node->right, name);
                }
            }
        }
    }
}

/*
 * This function is the starting point of the item searching process.
 * It is the "interface" method that takes the tree where the item will
 * be searched.
 */
item_node *search_in_name_node(tree_name_node *subTree, const char *name) {
    if (subTree == NULL) {
        return NULL;
    }
    return searchItemAux(subTree->theTree, name);
}

/*
 * This function is the recursvie part of the item searching process.
 * It follows the same logic as the search_for_name_node function, but
 * instead of using tree_name_node now item_node is used.
 */
item_node *searchItemAux(item_node *node, const char *name) {
    if (node == NULL) {
        return NULL;
    } else {
        int n = strlen(name);
        if (strncmp(node->name, name, n) == 0) {
            return node;
        } else {
            if (strcmp(name, node->name) < 0) {
                if (node->left == NULL) {
                    return NULL;
                } else {
                    return searchItemAux(node->left, name);
                }
            } else {
                if (node->right == NULL) {
                    return NULL;
                } else {
                    return searchItemAux(node->right, name);
                }
            }
        }
    }
}

/*
 *  This function is the starting point of the before item count process.
 * It is the "interface" method that takes the tree where the item will
 * be searched.
 */
int getItemCountBefore(tree_name_node *tree, item_node *node) {
    if (tree == NULL || node == NULL) {
        return -1;
    } else {
        return getItemCountBeforeAux(tree->theTree, node);
    }
}

/*
 * This function is the recursvie part of the before item count process.
 * For every node that the function takes that is not null or is not the node
 * the counter will be incremented by 1. It repeats the same process dor every left sub-tree
 * of every node, and just in case that the node is not in the left sub-tree, the function is called
 * on the right sub-tree.
 */
int getItemCountBeforeAux(item_node *self, item_node *node) {
    int count = 0;
    if (self == node || self == NULL) {
        return count;
    } else {
        count++;
        count += getItemCountBeforeAux(self->left, node);
        if (searchItemAux(self->left, node) == NULL)
            count += getItemCountBeforeAux(self->right, node);

        return count;
    }
}

/*
 * This function is a recursive function that calculates the height of any item_node
 * that is received as argument. It calls itself with the right and left subtree as
 * new parameters. For every call, the function return the greates height +1 (itself)
 */
int getHeight(item_node *node) {
    if (node == NULL) {
        return -1;
    }

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

/*
 * The interface method to get the right height of a tree
 */
int getRightHeight(tree_name_node *tree) {
    return getHeight(tree->theTree->right);
}

/*
 * The interface method to get the left height of a tree
 */
int getLeftHeight(tree_name_node *tree) {
    return getHeight(tree->theTree->left);
}

/*
 * The starting point of the total item count process.
 * It is the method that recevies the tree where the count
 * will be calculated.
 */
int getCount(tree_name_node *tree) {
    return getCountAux(tree->theTree);
}
/*
 * The recursive part of the total item count process.
 *traverses the tree in a pre-order direction. For every call
 * that the function makes, it return the count of the current
 * node, so it will be added later.
 */
int getCountAux(item_node *node) {
    if (node == NULL) {
        return 0;
    } else {
        int count = node->count;
        count += getCountAux(node->left);
        count += getCountAux(node->right);

        return count;
    }
}

/*
 * The starting point of the item deletion process.
 * It is the method that recevies the tree where the item
 * will be deleted. Also is where the father of the node is calculated.
 */
void deleteItemNode(tree_name_node *tree, item_node *node) {
    deleteItemNodeAux(tree, node, get_item_before(tree->theTree, node));
}

/*
 * The recursive part of the item deletion process.
 */
void deleteItemNodeAux(tree_name_node *tree, item_node *node, item_node *father) {
    item_node *aux;
    if (node == NULL || tree == NULL) {
        return;
    }
    /*
     * if the item is a leaf, it means it can now be deleted without a problem.
     * if the father is null, it means this is the first item in the tree, so it
     * assigns NULL to the tree reference of the name node.
     */
    if (item_is_leaf(node)) {
        if (father == NULL) {
            tree->theTree = NULL;
        } else {
            /*
             * If the father is not null, it sets the pinter to the node that will be deleted
             * to NULL
             */
            if (father->right == node) {
                father->right = NULL;
            } else {
                father->left = NULL;
            }
        }
        /*
         * the node is deleted
         */
        free(node);
        node = NULL;
    } else {
        /*
         * If the node is not a leaf, it has to be replaced with the highest node of the left sub-tree
         * or the lowest node of the right sub tree. Once the replacement is done, the function calls itself
         * with the highest or lowest as the node to be deleted and with the father of the node that will
         * be deleted.
         */
        if (node->left != NULL) {
            aux = get_highest(node->left);
            node->count = aux->count;
            father = get_item_before(tree->theTree, aux);
            strcpy(node->name, aux->name);
            deleteItemNodeAux(tree, get_highest(node->left),father);
        } else {
            aux = get_lowest(node->right);
            node->count = aux->count;
            father = get_item_before(tree->theTree, aux);
            strcpy(node->name, aux->name);
            deleteItemNodeAux(tree, get_lowest(node->right), father);

        }
    }
}

/*
 * Checks if the item have any child. It return 1 if the item has no child
 */
int item_is_leaf(item_node *node) {
    return node->right == NULL && node->left == NULL ? 1 : 0;
}

/*
 * Checks if the item have any child. It return 1 if the item has no child
 */
int tree_node_is_leaf(tree_name_node *node) {
    return node->right == NULL && node->left == NULL ? 1 : 0;
}

/*
 * recursive function that calculates the highest node (in lexicographicall value)
 * of a sub-tree. The function checks all of the right sub trees of the node.
 */
item_node *get_highest(item_node *node) {
    if (node == NULL || node->right == NULL) {
        return node;
    }

    return get_highest(node->right);
}

/*
 * recursive function that calculates the highest name node (in lexicographicall value)
 * of a sub-tree. The function checks all of the right sub trees of the name node.
 */
tree_name_node *get_highest_tree(tree_name_node *tree) {
    if (tree == NULL || tree->right == NULL) {
        return tree;
    }

    return get_highest_tree(tree->right);
}

/*
 * recursive function that calculates the lowest node (in lexicographicall value)
 * of a sub-tree. The function checks all of the left sub trees of the node.
 */
item_node *get_lowest(item_node *node) {
    if (node == NULL || node->left == NULL) {
        return node;
    }

    return get_lowest(node->left);
}

/*
 * recursive function that calculates the lowest name node (in lexicographicall value)
 * of a sub-tree. The function checks all of the left sub trees of the name node.
 */
tree_name_node *get_lowest_tree(tree_name_node *tree) {
    if (tree == NULL || tree->left == NULL) {
        return tree;
    }

    return get_lowest_tree(tree->left);
}

/*
 * Recursive function that gets the "father" of a node. It searches and stops
 * until one of the child node is the node passed as argument.
 */
item_node *get_item_before(item_node *root, item_node *node) {
    if (node == NULL || strcmp(node->name, root->name) == 0) {
        return NULL;
    } else {
        if (root->left == node || root->right == node) {
            return root;
        } else {
            if (strcmp(node->name, root->name) < 0) {
                return get_item_before(root->left, node);
            } else {
                return get_item_before(root->right, node);
            }
        }
    }
}


/*
 * Recursive function that gets the "father" of a name node. It searches and stops
 * until one of the child name node is the name node passed as argument.
 */
tree_name_node *get_tree_before(tree_name_node *root, tree_name_node *node) {
    if (node == NULL || strcmp(node->treeName, root->treeName) == 0) {
        return NULL;
    } else {
        if (root->left == node || root->right == node) {
            return root;
        } else {
            if (strcmp(node->treeName, root->treeName) < 0) {
                return get_tree_before(root->left, node);
            } else {
                return get_tree_before(root->right, node);
            }
        }
    }
}

/*
 * The starting point of the name node deletion process.
 * It is the method that recevies the tree where the item
 * will be deleted. Also is where the father of the node is calculated.
 */
void deleteTreeNameNode(tree_name_node *tree, tree_name_node *node) {
    deleteTreeNameNodeAux(tree, node, get_tree_before(tree, node));
}

/*
 * The recursive part of the name node deletion process.
 * The logic is exactly the same as the deleteItemNodeAux function, but instead
 * of using item_node pointers, the function uses tree_name_node pointers
 */
void deleteTreeNameNodeAux(tree_name_node *tree, tree_name_node *node, tree_name_node *father) {

    tree_name_node *aux;
    if (node == NULL) {
        return;
    }
    if (tree_node_is_leaf(node)) {
        if (father != NULL) {
            if (father->right == node) {
                father->right = NULL;
            } else {
                father->left = NULL;
            }
        }

        while (node->theTree != NULL) {
            deleteItemNode(node, node->theTree);
        }

        free(node);
        node = NULL;
    } else {
        if (node->left != NULL) {
            aux = get_highest_tree(node->left);
            node->theTree = aux->theTree;
            father = get_tree_before(tree, aux);
            strcpy(node->treeName, aux->treeName);
            deleteTreeNameNodeAux(tree, get_highest_tree(node->left), father);
        } else {
            aux = get_lowest_tree(node->right);
            node->theTree = aux->theTree;
            father = get_tree_before(tree, aux);
            strcpy(node->treeName, aux->treeName);
            deleteTreeNameNodeAux(tree, get_lowest_tree(node->right), father);
        }
    }
}

/*
 * Function that delete all of the items held by the super tree.
 * It calls deleteTreeNameNode for every name node that exists.
 */
void deleteAll(tree_name_node *root_tree) {
    for (int i = 0; i < getTreeCount(root_tree); i++) {
        deleteTreeNameNode(root_tree, root_tree);
    }
}

/*
 * Recursive function that counts every item that a tree of name nodes
 * have.
 */
int getTreeCount(tree_name_node *tree) {
    if (tree == NULL) {
        return 0;
    } else {
        int count = 1;
        count += getCountAux(tree->left);
        count += getCountAux(tree->right);

        return count;
    }
}

/*
 * Recursive funtion that prints the super tree and the sub tree in order.
 * In receives an integer, that will act as a counter so only the first
 * print the name of the name nodes and the recursive calls dont.
 */
void traverse_in_order(tree_name_node *root, int counter) {
    if (root == NULL) {
        return;
    } else {
        traverse_in_order(root->left, 0);
        fputs(root->treeName, stdout);
        fputs(" ", stdout);
        traverse_in_order(root->right, 0);
    }
    if (counter == 1) {
        fputs("\n", stdout);
        print_name_tree_in_order(root);
    }
}

/*
 * recursive function that prints every name node in order
 */
void print_tree_in_order(item_node *node) {
    if (node == NULL) {
        return;
    } else {
        print_tree_in_order(node->left);
        fputs(node->name, stdout);
        fputs(" ", stdout);
        print_tree_in_order(node->right);
    }
}

/*
 * recursive function that prints a tree of item in order.
 */
void print_name_tree_in_order(tree_name_node *root) {
    if (root == NULL) {
        return;
    } else {
        print_name_tree_in_order(root->left);
        fputs("===", stdout);
        fputs(root->treeName, stdout);
        fputs("===\n", stdout);
        print_tree_in_order(root->theTree);
        fputs("\n", stdout);
        print_name_tree_in_order(root->right);
    }
}











