#include "blockchain.h"

void _print_hex_bufferz(uint8_t const *buf, size_t len)
{
	size_t i;

	for (i = 0; buf && i < len; i++)
		printf("%02x", buf[i]);

	fflush(NULL);
}

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
	(void)prev_block;
	if (llist_size(block->transactions) <= 0)
	{
		fprintf(stderr, "DEBUG: !llist_size\n");
		return (1);
	}

	if (!coinbase_is_valid(llist_get_head(block->transactions),
	block->info.index))
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
		block_hash(prev_block, prev_hash);
	block_hash(block, current_hash);
	if (prev_block &&
		memcmp(prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
	{
		fprintf(stderr, "1\nprev_hash: ");
		_print_hex_bufferz(prev_hash, SHA256_DIGEST_LENGTH);
		fprintf(stderr, "\nprv_blk_h: ");
		_print_hex_bufferz(prev_block->hash, SHA256_DIGEST_LENGTH);
		fprintf(stderr, "\n");
		return (1);
	}
	if (prev_block && (memcmp(prev_block->hash, block->info.prev_hash,
		SHA256_DIGEST_LENGTH) != 0))
	{
		fprintf(stderr, "2\n");
		return (1);
	}
	if (memcmp(current_hash, block->hash, SHA256_DIGEST_LENGTH) != 0)
	{
		fprintf(stderr, "3\n");
		return (1);
	}
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
	{
		fprintf(stderr, "4\n");
		return (1);
	}
	if (sizeof(block->data.buffer) > BLOCKCHAIN_DATA_MAX)
	{
		fprintf(stderr, "5\n");
		return (1);
	}
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
	{
		fprintf(stderr, "6\n");
		return (1);
	}
	if (coinbase_check(block, prev_block, all_unspent) != 0)
	{
		fprintf(stderr, "7\n");
		return (1);
	}

	return (0);
}
