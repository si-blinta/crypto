#include "tth.h"
#include "utils.h"
#include "merkle_tree.h"
int main(){
	/*size_t number_blocks;
	uint8_t** data_blocks = exctract_blocks_from_file("s.png",&number_blocks);
	if(data_blocks == NULL)
		return 0;
	print_blocks(data_blocks,DATA_BLOCK_SIZE,50,number_blocks,"blocks");
	uint8_t message[]={0, 6, 8, 35, 17, 28, 24, 56, 62, 7, 12, 16, 20, 5, 33, 43, 35, 27, 12, 60,
					   			25, 23, 18, 1, 45, 56, 12, 34, 21, 20, 2, 10, 22, 20, 17, 34, 1};
	//floyd_collision(message,37);*/
	uint8_t m[]={32,32,0,0,0};
	tth_t_calc_hash(m,m,HASH_SIZE);
	print_hash(m,HASH_SIZE,"hash");
	return 0;
}
