#include "tth.h"
#include "utils.h"
#include "merkle_tree.h"
int main(){
	size_t number_blocks;
	uint8_t** data_blocks = exctract_blocks_from_file("utils.c",&number_blocks);
	//print_blocks(data_blocks,DATA_BLOCK_SIZE,10,number_blocks,"data block");
	//print_blocks(data_blocks,DATA_BLOCK_SIZE,10,number_blocks,"data block");
	merkle_tree* tree = merkle_tree_build(data_blocks,number_blocks);
	printf("%d\n",number_blocks);
	merkle_tree_print_tree(tree);
	return 0;
}
