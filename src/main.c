#include "tth.h"
void tth_t_print_data_blocks(uint8_t** blocks, size_t blocks_size);
int main(){
	size_t number_blocks;
	uint8_t** data_blocks = generate_blocks_from_file("x.png",&number_blocks);
	tth_t_print_data_blocks(data_blocks,number_blocks);
	return 0;

}
