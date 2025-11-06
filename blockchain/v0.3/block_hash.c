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
	unsigned char *buffer = NULL;
	transaction_t *tx = NULL;

	if (!block || !hash_buf)
	{
		fprintf(stderr, "!param\n");
		return (NULL);
	}

	buffer = (unsigned char *)malloc(block->data.len + sizeof(block->info) +
	SHA256_DIGEST_LENGTH);
	if (!buffer)
	{
		fprintf(stderr, "!buffer\n");
		return (NULL);
	}
	// fprintf(stderr, "-1\n");

	tx = (transaction_t *)llist_get_node_at(block->transactions, 0);
	// fprintf(stderr, "0\n");

	memcpy(buffer, (unsigned char *)block,
	sizeof(block->info) + block->data.len);
	// fprintf(stderr, "1\n");

	memcpy(buffer + sizeof(block->info) + block->data.len,
	tx->id, SHA256_DIGEST_LENGTH);
	// fprintf(stderr, "2\n");

	SHA256(buffer, block->data.len + sizeof(block->info) +
	SHA256_DIGEST_LENGTH, hash_buf);
	// fprintf(stderr, "3\n");

	free(buffer);
	// fprintf(stderr, "4\n");
	return (hash_buf);
}
