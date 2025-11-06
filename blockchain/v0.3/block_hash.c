#include "blockchain.h"

/**
 * block_hash - computes the hash of a Block
 * @block: block to be hashed
 * @hash_buf: location to store hash
 *
 * Return: pointer to hash_buf, NULL on failure
 */
uint8_t *block_hash(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int transactions_size = sizeof(tx_in_t) + sizeof(tx_out_t);
	unsigned char *buffer = NULL;
	tx_in_t *in = NULL;
	tx_out_t *out = NULL;

	if (!block || !hash_buf)
	{
		fprintf(stderr, "!param\n");
		return (NULL);
	}

	buffer = (unsigned char *)malloc(block->data.len + sizeof(block->info) +
	transactions_size);
	if (!buffer)
	{
		fprintf(stderr, "!buffer\n");
		return (NULL);
	}

	in = (tx_in_t *)llist_get_node_at(((transaction_t *)block->transactions)->inputs, 0);
	out = (tx_out_t *)llist_get_node_at(((transaction_t *)block->transactions)->outputs, 0);

	memcpy(buffer, (unsigned char *)block,
	sizeof(block->info) + block->data.len);
	fprintf(stderr, "1\n");

	memcpy(buffer + sizeof(block->info) + block->data.len, (unsigned char*)in, sizeof(tx_in_t));
	fprintf(stderr, "2\n");

	memcpy(buffer + sizeof(block->info) + block->data.len + sizeof(tx_in_t),
	(unsigned char *)out, sizeof(tx_out_t));
	fprintf(stderr, "3\n");

	SHA256(buffer, block->data.len + sizeof(block->info) +
	transactions_size, hash_buf);
	fprintf(stderr, "4\n");

	free(buffer);
	fprintf(stderr, "5\n");
	return (hash_buf);
}
