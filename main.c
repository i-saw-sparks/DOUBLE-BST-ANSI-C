#include <stdio.h>
#include <stdlib.h>
#include "BST.h"

void executeCommand(tree_name_node*, char*, char*, char*);
void printTreeNotFound(char*);
void printItemNotFound(char*, char*);

int main() {
    FILE *fptr;
    tree_name_node  *tree = NULL;

    if((fptr = fopen("input1.txt", "r")) == NULL){
        printf("Error ... main(): File not found");
        exit(1);
    }

    char line[63];
    int lineCounter = 0;
    int treeN=0, itemsN=0, commandsN=0;

    while(fgets(line, sizeof(line), fptr) != NULL){
        lineCounter ++;
        if(lineCounter == 1){
            sscanf(line, "%d%d%d", &treeN, &itemsN, &commandsN);
        }else if(lineCounter-1 <= treeN){
            if(tree == NULL){
                tree = treeInit(line);
            }else{
                insertTreeNameNode(tree, line);
            }
            //line == tree
        }else if(lineCounter-1-treeN <= itemsN){
            char treeName[32], itemName[32];
            int counter;
            sscanf(line, "%s%s%d", treeName, itemName, &counter);
            insertItemNode(search_for_name_node(tree, treeName), itemName, counter);
        }else if(lineCounter-1-treeN-itemsN <= itemsN){
            char cmd[15], treeName[32], itemName[32];
            sscanf(line, "%s%s%s", cmd, treeName, itemName);
            executeCommand(tree, treeName, itemName, cmd);
        }
    }
    fclose(fptr);
}

void executeCommand(tree_name_node *root, char* treeName, char* itemName, char*cmd){
    if(strncmp(cmd, "search", 6) == 0){
        tree_name_node* tree = search_for_name_node(root, treeName);
        if(tree == NULL){
            printTreeNotFound(treeName);
        }else{
            item_node* node = search_in_name_node(tree, itemName);
            if(node == NULL){
                printItemNotFound(treeName, itemName);
            }else{
                printf("%i ", node->count);
                fputs(itemName, stdout);
                fputs(" found in ", stdout);
                fputs(treeName, stdout);
                fputs("\n", stdout);
            }
        }
    }else if(strncmp(cmd, "item_before", 11) == 0){
        tree_name_node* tree = search_for_name_node(root, treeName);
        if(tree == NULL){
            printTreeNotFound(treeName);
        }else{
            item_node* node = search_in_name_node(tree, itemName);
            if(node == NULL){
                printItemNotFound(treeName, itemName);
            }else{
                fputs("items before ", stdout);
                fputs(itemName, stdout);
                printf(": %d\n", getItemCountBefore(tree, node));
            }
        }
    }else if(strncmp(cmd, "height_balance", 14) == 0){
        tree_name_node* tree = search_for_name_node(root, treeName);if(tree == NULL){
            printTreeNotFound(treeName);
        }else {
            int rightHeight = getRightHeight(tree);
            int leftHeight = getLeftHeight(tree);
            int difference = rightHeight - leftHeight;
            fputs(treeName, stdout);
            printf(": left height %d, right height %d, difference %d, ", leftHeight, rightHeight, difference);
            if(difference== 0){
                fputs("balanced\n", stdout);
            }else{
                fputs("not balanced\n", stdout);
            }
        }
    }else if(strncmp(cmd, "height_balance", 14) == 0){
        tree_name_node* tree = search_for_name_node(root, treeName);if(tree == NULL){
            printTreeNotFound(treeName);
        }else {
            int rightHeight = getRightHeight(tree);
            int leftHeight = getLeftHeight(tree);
            int difference = rightHeight - leftHeight;
            fputs(treeName, stdout);
            printf(": left height %d, right height %d, difference %d, ", leftHeight, rightHeight, difference);
            if(difference== 0){
                fputs("balanced\n", stdout);
            }else{
                fputs("not balanced\n", stdout);
            }
        }
    }else if(strncmp(cmd, "count", 5) == 0){
        tree_name_node* tree = search_for_name_node(root, treeName);
        if(tree == NULL){
            printTreeNotFound(treeName);
        }else {
            fputs(treeName, stdout);
            printf(" count %d\n", getCount(tree));
        }
    }else if(strncmp(cmd, "delete", 6) == 0){
        tree_name_node* tree = search_for_name_node(root, treeName);
        if(tree == NULL){
            printTreeNotFound(treeName);
        }else {
            item_node* node = search_in_name_node(tree, itemName);
            if(node == NULL){
                printItemNotFound(treeName, itemName);
            }else{
                deleteItemNode(tree, node);
                fputs(itemName, stdout);
                fputs(" deleted from ", stdout);
                fputs(treeName, stdout);
                fputs("\n", stdout);
            }
        }
    }
}

void printTreeNotFound(char* treeName){
    fputs(treeName, stdout);
    fputs(" does not exist\n", stdout);
}

void printItemNotFound(char* treeName, char* itemName){
    fputs(itemName, stdout);
    fputs(" not found in ", stdout);
    fputs(treeName, stdout);
    fputs("\n", stdout);
}