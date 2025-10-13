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
	if (!block || !hash_buf)
		return (NULL);

	SHA256((unsigned char *)block,
	block->data.len + sizeof(block->info), hash_buf);
	return (hash_buf);
}
