#include "blockchain.h"

/**
 * block_destroy - deletes an existing Block
 * @block: Block to be deleted
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;

	free(block);
	block = 0;
}
