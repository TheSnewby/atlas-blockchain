#include "blockchain.h"

/**
 * blockchain_create - creates and initializes a Blockchain structure
 *
 * Return: new blockchain or NULL on failure
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *newBC = NULL;
	llist_t *newLL = NULL;
	block_t *newB = NULL;

	/* Create Initial Block */
	newB = (block_t *)malloc(sizeof(block_t));
	if (!newB)
		return (NULL);

	newB->info.index = 0;
	newB->info.difficulty = 0;
	newB->info.timestamp = 1537578000;
	newB->info.nonce = 0;
	memset(newB->info.prev_hash, 0, 32);
	memcpy(newB->data.buffer, "Holberton School", strlen("Holberton School"));
	newB->data.len = 16;
	memcpy(newB->hash, HOLBERTON_HASH, SHA256_DIGEST_LENGTH);

	/* Create Blockchain */
	newBC = (blockchain_t *)malloc(sizeof(blockchain_t));
	if (!newBC)
		return (NULL);

	/* Create new Linked List for Blockchain */
	newLL = llist_create(MT_SUPPORT_TRUE);
	if (!newLL)
		return (NULL);

	/* Assign new Linked List to Blockchain and add Block to new LL */
	newBC->chain = newLL;
	if (llist_add_node(newLL, newB, ADD_NODE_FRONT))
		return (NULL);

	return (newBC);
}
