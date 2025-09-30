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
	newB->info.prev_hash[0] = 0;
	strcpy(newB->data.buffer, "Holberton School");
	newB->data.len = strlen("Holberton School");
	strcpy(newB->hash, "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03");

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
	if (!llist_add_node(newLL, newB, ADD_NODE_FRONT))
		return (NULL);

	return (newBC);
}
