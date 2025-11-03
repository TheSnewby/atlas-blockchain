#include "blockchain.h"

/**
 * block_destroy - deletes an existing Block
 * @block: Block to be deleted
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;

	llist_destroy(block->transactions, 1, (node_dtor_t)transaction_destroy);

	free(block);
	block = NULL;
}
