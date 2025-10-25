#include "blockchain.h"

/**
 * blockchain_destroy - deletes an existing Blockchain including
 * all contained Blocks
 * @blockchain: blockchain to be deleted
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	llist_destroy(blockchain->chain, 1, NULL);
	free(blockchain);
	blockchain = NULL;
}
