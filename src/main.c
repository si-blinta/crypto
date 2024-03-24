#include "tth.h"
#include "utils.h"
#include "merkle_tree.h"
int main(){
	size_t number_blocks;
	uint8_t** data_blocks = exctract_blocks_from_file("s.png",&number_blocks);
	if(data_blocks == NULL)
		return 0;
	print_blocks(data_blocks,DATA_BLOCK_SIZE,50,number_blocks,"blocks");
	floyd_collision(data_blocks[3],number_blocks);
	/*uint8_t m[]={32,32,0,0,0};
	tth_t_calc_hash(m,m,HASH_SIZE);
	print_hash(m,HASH_SIZE,"hash");*/
	return 0;
}
// 32 32 0 0 0 LOOP