#include "merkle_tree.h"

merkle_tree* merkle_tree_init(){
    merkle_tree* tree = malloc(sizeof (merkle_tree));
    if(tree == NULL){
        perror("[merkle_tree_init][malloc]");
        return NULL;
    }
    memset(tree->hash,0,HASH_SIZE);
    tree->left= NULL;
    tree->right=NULL;
    return tree;
}
void merkle_tree_print_node(merkle_tree* node){
    if(node == NULL){
        printf("merkle tree is NULL\n");
        return ;
    }
    print_hash(node->hash,HASH_SIZE,"hash");
}
void merkle_tree_print_tree(merkle_tree* tree){
    merkle_tree_print_node(tree);
    if(tree->left != NULL){
        merkle_tree_print_tree(tree->left);
    }
    if(tree->right != NULL){
        merkle_tree_print_tree(tree->right);
    }
}





