#include <stdlib.h>

#include "blockchain.h"

/**
 * blockchain_destroy - Deallocates a Blockchain structure
 *
 * @blockchain: Pointer to the Blockchain structure to be freed
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	llist_destroy(blockchain->chain, 1, (node_dtor_t)block_destroy);
	llist_destroy(blockchain->unspent, 1, free);

	free(blockchain);
}
