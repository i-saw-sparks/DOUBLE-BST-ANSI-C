#include "BST.h"

tree_name_node *treeInit(const char name[]) {
    tree_name_node *name_node = newTreeNameNode(name);
    return name_node;
}

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

void insertItemNode(tree_name_node *self, const char *name, int count) {
    if (self == NULL) {
        return;
    }
    self->theTree = insertItemNodeAux(self->theTree, name, count);
}

item_node *insertItemNodeAux(item_node *self, const char *name, int count) {
    if (self == NULL) {
        return newItemNode(name, count);
    } else {
        if (strcmp(name, self->name) < 0) {
            self->left = insertItemNodeAux(self->left, name, count);
        } else {
            self->right=insertItemNodeAux(self->right, name, count);
        }
    }
    return self;
}

tree_name_node *search_for_name_node(tree_name_node* node, const char* name) {
    if(node == NULL){
        return NULL;
    }else{
        int n = strlen(name);
        if(strncmp(node->treeName, name, n) == 0){
            return node;
        }else{
            if(strcmp(name, node->treeName) < 0){
                if(node->left == NULL){
                    return NULL;
                }else{
                    return search_for_name_node(node->left, name);
                }
            }else{
                if(node->right == NULL){
                    return NULL;
                }else{
                    return search_for_name_node(node->right, name);
                }
            }
        }
    }
}

item_node *search_in_name_node(tree_name_node *subTree, const char* name) {
    if (subTree == NULL) {
        return NULL;
    }
    return searchItemAux(subTree->theTree, name);
}

item_node *searchItemAux(item_node *node, const char *name) {
    if(node == NULL){
        return NULL;
    }else{
        int n = strlen(name);
        if(strncmp(node->name, name, n) == 0){
            return node;
        }else{
            if(strcmp(name, node->name) < 0){
                if(node->left == NULL){
                    return NULL;
                }else{
                    return searchItemAux(node->left, name);
                }
            }else{
                if(node->right == NULL){
                    return NULL;
                }else{
                    return searchItemAux(node->right, name);
                }
            }
        }
    }
}

int getItemCountBefore(tree_name_node *tree, item_node *node) {
    if(tree == NULL || node == NULL){
        return -1;
    }else{
        return getItemCountBeforeAux(tree->theTree, node);
    }
}

int getItemCountBeforeAux(item_node *self, item_node *node) {
    int count = 0;
    if(self == node || self == NULL){
        return count;
    }else{
        count ++;
        count += getItemCountBeforeAux(self->left, node);
        if(searchItemAux(self->left, node) == NULL)
            count += getItemCountBeforeAux(self->right, node);

        return count;
    }
}

int getHeight(item_node *node) {
    if(node == NULL){
        return -1;
    }

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    return (leftHeight > rightHeight ? leftHeight:rightHeight)+1;
}

int getRightHeight(tree_name_node *tree) {
    return getHeight(tree->theTree->right);
}

int getLeftHeight(tree_name_node *tree) {
    return getHeight(tree->theTree->left);
}

int getCount(tree_name_node *tree) {
    return getCountAux(tree->theTree);
}

int getCountAux(item_node *node){
    if(node == NULL){
        return 0;
    }else{
        int count = node->count;
        count += getCountAux(node->left);
        count += getCountAux(node->right);

        return count;
    }
}

void deleteItemNode(tree_name_node* tree, item_node *node) {
    item_node *aux;
    if(node == NULL){
        return;
    }
    if(is_leaf(node)){
        item_node* bef = get_node_before(tree->theTree, node);
        if(bef ->right == node){
            bef->right = NULL;
        }else{
            bef->left = NULL;
        }
        free(node);
    }else{
        if(node -> left != NULL){
            aux = get_highest(node->left);
            node->count = aux->count;
            strcpy(node->name, aux->name);
            deleteItemNode(tree, get_highest(node->left));
        }else{
            aux = get_lowest(node->right);
            node->count = aux->count;
            strcpy(node->name, aux->name);
            deleteItemNode(tree, get_lowest(node->right));

        }
    }
}

int is_leaf(item_node *node) {
    return node->right == NULL && node ->left == NULL ? 1:0;
}

item_node *get_highest(item_node *node) {
    if(node == NULL || node ->right == NULL){
        return node;
    }

    return get_highest(node->right);
}

item_node *get_lowest(item_node *node) {
    if(node == NULL || node ->left == NULL){
        return node;
    }

    return get_lowest(node->left);
}

item_node *get_node_before(item_node *root, item_node *node) {
    if(node == NULL || node == root){
        return NULL;
    }else{
        if(root ->left == node || root->right == node){
            return root;
        }else{
           if(strcmp(node->name, root->name) < 0){
               return get_node_before(root->left, node);
           }else{
               return get_node_before(root->right, node);
           }
        }
    }

}



