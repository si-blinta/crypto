#include "tth.h"

uint8_t*  tth_t_padding(uint8_t* message, size_t message_length, size_t* padded_length){
	size_t values_missing = BLOCK_SIZE - ( message_length % BLOCK_SIZE ) ;
	uint8_t* padded_message = malloc ( message_length + values_missing  );
	if(padded_message == NULL ){
		perror("[tth_t_padding][malloc]");
		return NULL;
	}
	int offset = 0;
	memcpy(padded_message,message,message_length);
	offset += message_length;
	padded_message[offset++] = 32;
	memset(padded_message+offset,0, offset - message_length - sizeof(uint8_t) ) ;
	*padded_length = message_length + values_missing;
	return padded_message;
}
void tth_t_print_message(uint8_t* message,size_t message_length,char* info){
	printf("---------------------message----------------------------\n");
	printf("%s\n",info);
	printf("[");
	for(size_t i = 0; i < message_length; i++)
#if HEXA
	printf("%02x ",message[i]);
#else
	printf("%u ",message[i]);
#endif//HEXA
	printf("]\n");

}
uint8_t** tth_t_generate_blocks(uint8_t* padded_message,size_t padded_length){
	size_t number_of_blocks = padded_length / BLOCK_SIZE;
	uint8_t** blocks = malloc(number_of_blocks*sizeof(uint8_t*));
	if(blocks == NULL){
		perror("[tth_t_generate_blocks][first malloc]");
		return NULL;
	}
	for(size_t i = 0 ; i < number_of_blocks; i++){
		blocks[i] = malloc( sizeof(uint8_t) * BLOCK_SIZE);
		if(blocks[i] == NULL){
			perror("[tth_t_generate_blocks][second malloc]");
			return NULL;
		}
		memcpy(blocks[i],padded_message+i*BLOCK_SIZE,BLOCK_SIZE);
	}
	return blocks;
}
void tth_t_print_blocks(uint8_t** blocks, size_t blocks_size){	
	printf("---------------------BLOCKS----------------------------\n");
	for(size_t i = 0; i < blocks_size/BLOCK_SIZE; i++){
		printf("---block#%ld----\n[",i);
		for(size_t j =0;j<BLOCK_SIZE;j++){
			if(j%HASH_SIZE == 0)printf("\n");
#if HEXA
			printf("%02x ",blocks[i][j]);
#else
			printf("%u ",blocks[i][j]);
#endif //HEXA			
		}
		printf("\n]\n");
		printf("--------------\n");
	}	
}
void tth_t_print_data_blocks(uint8_t** blocks, size_t blocks_size){	
	printf("------------------DATA BLOCKS-----------------------\n");
	for(size_t i = 0; i < blocks_size; i++){
		printf("---block#%ld----\n[",i);
		for(size_t j =0;j<BLOCK_SIZE;j++){
			if(j%HASH_SIZE == 0)printf("\n");
#if HEXA
			printf("%02x ",blocks[i][j]);
#else
			printf("%u ",blocks[i][j]);
#endif //HEXA			
		}
		printf("\n]\n");
		printf("--------------\n");
	}	
}
uint8_t* tth_t_calc_empreinte_block(uint8_t * block){
	uint8_t* empreinte = malloc(HASH_SIZE);
	if(empreinte == NULL){
		perror("[tth_t_calc_empreinte_block][malloc]");
		return NULL;
	}
	memset(empreinte,0,HASH_SIZE);	// fill it with zeros
	for(size_t i = 0 ; i < HASH_SIZE ; i++){	
		for(size_t j  = 0 ; j < HASH_SIZE ; j++){
			empreinte[i]+=block[i+j*HASH_SIZE];
		}
		empreinte[i]%=64;
	}
	return empreinte;
}
static uint8_t* right_shift_circular(uint8_t* array,size_t size, uint8_t value ){
	uint8_t* new_array = malloc(size);
	if(new_array == NULL){
		perror("[right_rotate_circular][malloc]");
		return NULL;
	}
	for(size_t i = 0; i < size; i++){
		new_array[(i + value) % size] = array[i];
	}
	return new_array;
}

static void right_shift_block(uint8_t* block){
	for(size_t i = 0 ; i < HASH_SIZE ; i ++){
		uint8_t* temp = right_shift_circular(block + HASH_SIZE*i,HASH_SIZE,i);
		memcpy(block + i*HASH_SIZE,temp,HASH_SIZE);
		free(temp);
	}
}
static int tth_t_compare(uint8_t* h1,uint8_t* h2){
	for(size_t i = 0; i < HASH_SIZE ; i++){
		if(h1[i] > h2[i] )
			return 1;
		else if(h1[i] < h2[i])
			return -1;
	}
	return 0;
}

void tth_t_right_shift_blocks(uint8_t** blocks, size_t size){
	for(size_t i  = 0 ; i < size; i++){
		right_shift_block(blocks[i]);
	}
}

void tth_t_calc_hash(uint8_t* hash,uint8_t* message, size_t message_length){
	size_t padded_length;
	uint8_t * padded_message = tth_t_padding(message,message_length,&padded_length);
	uint8_t ** blocks = tth_t_generate_blocks(padded_message,padded_length);
	uint8_t* tmp_empreinte = NULL;
	for(size_t i = 0; i < padded_length / BLOCK_SIZE; i++){
		tmp_empreinte = tth_t_calc_empreinte_block(blocks[i]);
		for(int i = 0;i<HASH_SIZE;i++){
			hash[i]+=tmp_empreinte[i];
			hash[i]%=64;
		}
		free(tmp_empreinte);
#if DEBUG
		tth_t_print_message(hash,HASH_SIZE,"empreinte");
#endif //DEBUG
		right_shift_block(blocks[i]);
		tmp_empreinte = tth_t_calc_empreinte_block(blocks[i]);
		for(int i = 0;i<HASH_SIZE;i++){
			hash[i]+=tmp_empreinte[i];
			hash[i]%=64;
		}
		free(tmp_empreinte);
#if DEBUG
		tth_t_print_message(hash,HASH_SIZE,"empreinte");
#endif //DEBUG
	}
#if DEBUG
		tth_t_print_message(hash,HASH_SIZE,"empreinte_final");
#endif //DEBUG
	free(padded_message);
	for(size_t i = 0; i <padded_length / BLOCK_SIZE; i ++)free(blocks[i]);
	free(blocks);
}
int floyd_collision(){ //TODO
	uint8_t* message = malloc(MESSAGE_SIZE);
	uint64_t number_iterations = 0;
	if(message == NULL){
		perror("[floyd_collision][malloc]");
		return -1;
	}
	for(size_t i = 0; i < MESSAGE_SIZE ;i++)message[i] = rand() % 64;
	tth_t_print_message(message,MESSAGE_SIZE,"message");
	int found = 0;
	uint8_t h1[HASH_SIZE],h2[HASH_SIZE];
	tth_t_calc_hash(h1,message,MESSAGE_SIZE);
	tth_t_calc_hash(h2,message,MESSAGE_SIZE);
	uint8_t* tmp1 = malloc(5);
	uint8_t* tmp2 = malloc(5);
	while(!found ){
		usleep(10);
		number_iterations++;
		memcpy(tmp1,h1,HASH_SIZE);
		tth_t_calc_hash(h1,h1,HASH_SIZE);
		tth_t_calc_hash(h2,h2,HASH_SIZE);
		memcpy(tmp2,h2,HASH_SIZE);
		tth_t_calc_hash(h2,h2,HASH_SIZE);
		if(tth_t_compare(h1,h2) == 0){
			tth_t_print_message(tmp1,HASH_SIZE,"m1");
			tth_t_print_message(tmp2,HASH_SIZE,"m2");
			tth_t_print_message(h2,HASH_SIZE,"h2");
			tth_t_print_message(h1,HASH_SIZE,"h1");
			found = 1;
		}
	}
	tth_t_calc_hash(h1,message,MESSAGE_SIZE);
	while(tth_t_compare(h1,h2) != 0){
		usleep(10);
		memcpy(tmp1,h1,HASH_SIZE);
		tth_t_calc_hash(h1,h1,HASH_SIZE);
		memcpy(tmp2,h2,HASH_SIZE);
		tth_t_calc_hash(h2,h2,HASH_SIZE);
	}
	tth_t_print_message(tmp1,HASH_SIZE,"m1");
	tth_t_print_message(tmp2,HASH_SIZE,"m2");
	tth_t_print_message(h2,HASH_SIZE,"h2");
	tth_t_print_message(h1,HASH_SIZE,"h1");
	return number_iterations;
}
uint8_t** generate_blocks_from_file(char* filename,size_t* number_blocks){
	FILE* file = fopen(filename,"rb");
	if(file == NULL){
		perror("[generate_blocks_from_file][fopen]");
		return NULL;
	}
	struct stat st;
	stat(filename, &st);
	size_t size = st.st_size;
	size_t remaining = size % DATA_BLOCK_SIZE;
	if(remaining != 0){
		size+=DATA_BLOCK_SIZE-remaining;//Padding
	}
	*number_blocks = size / DATA_BLOCK_SIZE;
	uint8_t** data_blocks = malloc(*number_blocks * sizeof(uint8_t*));
	if(data_blocks == NULL){
		perror("[generate_blocks_from_file][first malloc]");
		return NULL;
	} 
	for(size_t i = 0; i < *number_blocks; i++){
		data_blocks[i] = malloc(DATA_BLOCK_SIZE);
		if(data_blocks[i] == NULL){
			perror("[generate_blocks_from_file][second malloc]");
			return NULL;
		}
		memset(data_blocks[i],0,DATA_BLOCK_SIZE);
	}
	for(size_t i = 0; i < *number_blocks; i++){
		size_t bytes_read = fread(data_blocks[i],1,DATA_BLOCK_SIZE,file);
		if(bytes_read < DATA_BLOCK_SIZE){
			data_blocks[i][bytes_read] = 32; 
		}
		for(size_t j = 0 ; j < DATA_BLOCK_SIZE ; j++)data_blocks[i][j]%=64;

	}
	fclose(file);
	return data_blocks;

}










//------------STATIC--------------------------------------------------------------
/**
 * @brief This function right shifts in circular fashion an array.
 * 		  It is used in the tth_t_right_shift_block.
 * @param array The array to rotate.
 * @param size  The size of the array.
 * @param value The amount of the shift.
 * @return A dynamically allocated rotated array.
*/
static uint8_t* right_shift_circular(uint8_t* array,size_t size, uint8_t value );
/**
*@brief This function appy circular right shift to a block.
		This function used in tth_right_shift_blocks.
*@param block
*/
static void right_shift_block(uint8_t* block);
/**
*@brief This function compares hashes, h1 and h2.
*@param h1 hash
*@param h2 hash
*@return 0 if equal , 1 if h1 greater and -1 if h2 greater
*/
static int tth_t_compare(uint8_t* h1,uint8_t* h2);












