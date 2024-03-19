#include "tth.h"
#include "utils.h"
#include "merkle_tree.h"
int main(){
	size_t number_blocks;
	uint8_t** data_blocks = exctract_blocks_from_file("x.png",&number_blocks);
	print_blocks(data_blocks,DATA_BLOCK_SIZE,10,number_blocks,"data block");
	merkle_tree* tree = merkle_tree_init();
	merkle_tree_put(0,data_blocks[2],DATA_BLOCK_SIZE,tree);
	tree->left = merkle_tree_init();
	merkle_tree_put(1,data_blocks[0],DATA_BLOCK_SIZE,tree->left);
	tree->right = merkle_tree_init();
	merkle_tree_put(2,data_blocks[1],DATA_BLOCK_SIZE,tree->right);
	merkle_tree_print_tree(tree);
	return 0;
}
