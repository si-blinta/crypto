#include "tth.h"

uint8_t *tth_t_padding(uint8_t *message, size_t message_length, size_t *padded_length)
{
	uint8_t *padded_message;
	if (message_length % BLOCK_SIZE == 0)
	{
		// No padding needed
		*padded_length = message_length;
		padded_message = malloc(message_length);
		memcpy(padded_message, message, message_length);
		return padded_message;
	}

	size_t values_missing = BLOCK_SIZE - (message_length % BLOCK_SIZE);
	*padded_length = message_length + values_missing;
	padded_message = malloc(message_length + values_missing);
	if (padded_message == NULL)
	{
		perror("[tth_t_padding][malloc]");
		return NULL;
	}
	int offset = 0;
	memcpy(padded_message, message, message_length);
	offset += message_length;
	padded_message[offset++] = 32;
	memset(padded_message + offset, 0, *padded_length - offset);
	return padded_message;
}
uint8_t **tth_t_generate_blocks(uint8_t *padded_message, size_t padded_length)
{
	size_t number_of_blocks = padded_length / BLOCK_SIZE;
	uint8_t **blocks = malloc(number_of_blocks * sizeof(uint8_t *));
	if (blocks == NULL)
	{
		perror("[tth_t_generate_blocks][first malloc]");
		return NULL;
	}
	for (size_t i = 0; i < number_of_blocks; i++)
	{
		blocks[i] = malloc(sizeof(uint8_t) * BLOCK_SIZE);
		if (blocks[i] == NULL)
		{
			perror("[tth_t_generate_blocks][second malloc]");
			return NULL;
		}
		memcpy(blocks[i], padded_message + i * BLOCK_SIZE, BLOCK_SIZE);
	}
	return blocks;
}
uint8_t *tth_t_calc_empreinte_block(uint8_t *block)
{
	uint8_t *empreinte = malloc(HASH_SIZE);
	if (empreinte == NULL)
	{
		perror("[tth_t_calc_empreinte_block][malloc]");
		return NULL;
	}
	memset(empreinte, 0, HASH_SIZE); // fill it with zeros
	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		for (size_t j = 0; j < HASH_SIZE; j++)
		{
			empreinte[i] += block[i + j * HASH_SIZE];
		}
		empreinte[i] %= 64;
	}
	return empreinte;
}
static uint8_t *right_shift_circular(uint8_t *array, size_t size, uint8_t value)
{
	uint8_t *new_array = malloc(size);
	if (new_array == NULL)
	{
		perror("[right_rotate_circular][malloc]");
		return NULL;
	}
	for (size_t i = 0; i < size; i++)
	{
		new_array[(i + value) % size] = array[i];
	}
	return new_array;
}

static void right_shift_block(uint8_t *block)
{
	for (size_t i = 0; i < HASH_SIZE; i++)
	{
#if MODIFIED
		uint8_t *temp = right_shift_circular(block + HASH_SIZE * i, HASH_SIZE, i + 1);
#else
		uint8_t *temp = right_shift_circular(block + HASH_SIZE * i, HASH_SIZE, i);
#endif
		memcpy(block + i * HASH_SIZE, temp, HASH_SIZE);
		free(temp);
	}
}
int tth_t_compare(uint8_t *h1, uint8_t *h2)
{
	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		if (h1[i] > h2[i])
		{
			return 1;
		}
		if (h1[i] < h2[i])
		{
			return -1;
		}
	}
	return 0;
}

void tth_t_right_shift_blocks(uint8_t **blocks, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		right_shift_block(blocks[i]);
	}
}

void tth_t_calc_hash(uint8_t *hash, uint8_t *message, size_t message_length)
{
	size_t padded_length;
	uint8_t *temp_hash = malloc(HASH_SIZE); // work on a copy to allow this kind of calls calc_hash(hash,hash,HASH_SIZE)
	memset(temp_hash, 0, HASH_SIZE);		// set to zero because we will increment the contents.
	if (temp_hash == NULL)
	{
		perror("[tth_t_calc_hash][malloc]");
		exit(1);
	}
	uint8_t *padded_message = tth_t_padding(message, message_length, &padded_length);
#if DEBUG
	print_hash(padded_message, padded_length, "padding");

#endif // DEBUG
	uint8_t **blocks = tth_t_generate_blocks(padded_message, padded_length);
#if DEBUG
	print_blocks(blocks, BLOCK_SIZE, 5, padded_length / BLOCK_SIZE, "BLOCKS");

#endif // DEBUG
	uint8_t *tmp_empreinte = NULL;
	for (size_t i = 0; i < padded_length / BLOCK_SIZE; i++)
	{
		tmp_empreinte = tth_t_calc_empreinte_block(blocks[i]);
		for (int i = 0; i < HASH_SIZE; i++)
		{
			temp_hash[i] += tmp_empreinte[i];
			temp_hash[i] %= 64;
		}
		free(tmp_empreinte);
#if DEBUG
		char log_message[100];
		sprintf(log_message, "first empreinte %ld", i);
		print_hash(temp_hash, HASH_SIZE, log_message);
#endif // DEBUG
		right_shift_block(blocks[i]);
		tmp_empreinte = tth_t_calc_empreinte_block(blocks[i]);
		for (int i = 0; i < HASH_SIZE; i++)
		{
			temp_hash[i] += tmp_empreinte[i];
			temp_hash[i] %= 64;
		}
		free(tmp_empreinte);
#if DEBUG
		sprintf(log_message, "second empreinte (after shift) %ld", i);
		print_hash(temp_hash, HASH_SIZE, log_message);
#endif // DEBUG
	}
#if DEBUG
	print_hash(temp_hash, HASH_SIZE, "empreinte_final");
#endif // DEBUG
	free(padded_message);
	memcpy(hash, temp_hash, HASH_SIZE);
	free(temp_hash);
	for (size_t i = 0; i < padded_length / BLOCK_SIZE; i++)
		free(blocks[i]);
	free(blocks);
}

int floyd_collision(uint8_t *start_message, size_t message_length)
{
	uint8_t h1[HASH_SIZE], h2[HASH_SIZE];
	int iterations = 0;
	tth_t_calc_hash(h2, start_message, message_length);
	tth_t_calc_hash(h2, h2, HASH_SIZE);
	tth_t_calc_hash(h1, start_message, message_length);
	while (1)
	{
		iterations++;
		tth_t_calc_hash(h1, h1, HASH_SIZE);
		tth_t_calc_hash(h2, h2, HASH_SIZE);
		print_hash(h1, HASH_SIZE, "h1");
		print_hash(h2, HASH_SIZE, "h2");

		if (tth_t_compare(h1, h2) == 0)
		{
			printf("iterations %d\n", iterations);
			break;
		}
	}
	return 0;
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
static uint8_t *right_shift_circular(uint8_t *array, size_t size, uint8_t value);
/**
*@brief This function appy circular right shift to a block.
		This function used in tth_right_shift_blocks.
*@param block
*/
static void right_shift_block(uint8_t *block);
/**
 *@brief This function compares hashes, h1 and h2.
 *@param h1 hash
 *@param h2 hash
 *@return 0 if equal , 1 if h1 greater and -1 if h2 greater
 */
