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











