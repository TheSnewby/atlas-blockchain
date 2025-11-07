#include "blockchain.h"

/**
 * coinbase_check - checks coinbase for validity
 * @block: the Block to check
 * @prev_block: previous Block in the Blockchain, or NULL if first Block
 * @all_unspent: llist of unspent transaction outputs
 * Return: 0 on success, 1 on failure
 */
int coinbase_check(block_t const *block, block_t const *prev_block,
	llist_t *all_unspent)
{
	(void)all_unspent;
	if ((llist_size(block->transactions) <= 0) || (llist_size(prev_block->transactions) <= 0))
		return (0);

	if ((!coinbase_is_valid(llist_get_head(block->transactions), block->info.index)) ||
	(!coinbase_is_valid(llist_get_head(prev_block->transactions), prev_block->info.index)))
	{
		fprintf(stderr, "DEBUG: !coinbase_is_valid\n");
		return (1);
	}
	return (0);
}

/**
 * block_is_valid - verifies that a Block is valid
 * @block: the Block to check
 * @prev_block: previous Block in the Blockchain, or NULL if first Block
 * @all_unspent: llist of unspent transaction outputs
 * Return: 0 on success, 1 on failure
 */
int block_is_valid(block_t const *block, block_t const *prev_block,
	llist_t *all_unspent)
{
	uint8_t prev_hash[SHA256_DIGEST_LENGTH] = {0};
	uint8_t current_hash[SHA256_DIGEST_LENGTH] = {0};

	(void)all_unspent;
	if (!block || (block->info.index != 0 && !prev_block))
		return (1);
	if (prev_block && (block->info.index != prev_block->info.index + 1))
		return (1);
	if (block->info.index == 0) /* Genesis Block Check */
	{
		if (prev_block || block->info.difficulty || block->info.nonce)
			return (1);
		if (memcmp(block->hash, HOLBERTON_HASH, SHA256_DIGEST_LENGTH) != 0)
			return (1);
	}
	if (prev_block)
		SHA256((unsigned char *)prev_block,
		prev_block->data.len + sizeof(prev_block->info), prev_hash);
	SHA256((unsigned char *)block,
	block->data.len + sizeof(block->info), current_hash);
	if (prev_block &&
		memcmp(prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);
	if (prev_block && (memcmp(prev_block->hash, block->info.prev_hash,
		SHA256_DIGEST_LENGTH) != 0))
		return (1);
	if (memcmp(current_hash, block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);
	if (sizeof(block->data.buffer) > BLOCKCHAIN_DATA_MAX)
		return (1);
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);
	if (!coinbase_check(block, prev_block, all_unspent))
		return (1);

	return (0);
}
