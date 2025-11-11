#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

/**
 * blockchain_create - Creates a new Blockchain
 *
 * Return: A pointer to the created Blockchain structure
 *
 * Memory: The user is responsible for freeing the memory area pointed to by
 * the pointer returned by this function. This can be done by calling the
 * function 'blockchain_destroy()'
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain;
	block_t *genesis;

	blockchain = malloc(sizeof(*blockchain));
	if (!blockchain)
		return (NULL);

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);

	genesis = malloc(sizeof(*genesis));
	memcpy(genesis, &_genesis, sizeof(_genesis));
	llist_add_node(blockchain->chain, genesis, ADD_NODE_REAR);

	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);

	return (blockchain);
}
