#include <string.h>

#include "blockchain.h"

static int _genesis_is_valid(block_t const *block)
{
	if (memcmp(block, &_genesis, sizeof(*block)) != 0)
		return (-1);
	return (0);
}

static int _transaction_is_valid(transaction_t const *transaction,
	unsigned int idx, llist_t *all_unspent)
{

	if (idx == 0)
		return (0);

	if (!transaction_is_valid(transaction, all_unspent))
		return (-1);

	return (0);
}

/**
 * block_is_valid - Checks for a Block's integrity
 *
 * @block:      Pointer to the Block to be validated
 * @prev_block: Pointer to the Block that precedes @block in the Blockchain
 *
 * Return: Return status code
 */
int block_is_valid(block_t const *block, block_t const *prev_block,
	llist_t *all_unspent)
{
	unsigned char hash[SHA256_DIGEST_LENGTH] = {0};

	if (!block || (block->info.index > 0 && !prev_block))
	{
		return (-1);
	}

	if (block->info.index == 0)
		return (_genesis_is_valid(block));

	if (block->info.index != prev_block->info.index + 1)
	{
		return (-1);
	}

	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
	{
		return (-1);
	}

	block_hash(prev_block, hash);
	if (memcmp(block->info.prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH) != 0 ||
	    memcmp(block->info.prev_hash, hash, SHA256_DIGEST_LENGTH) != 0)
	{
		return (-1);
	}

	block_hash(block, hash);
	if (memcmp(block->hash, hash, SHA256_DIGEST_LENGTH) != 0)
	{
		return (-1);
	}

	if (block->data.len > BLOCKCHAIN_DATA_MAX)
	{
		return (-1);
	}

	if (llist_size(block->transactions) < 1)
	{
		return (-1);
	}
	if (!coinbase_is_valid(llist_get_head(block->transactions),
		block->info.index))
	{
		return (-1);
	}
	if (llist_for_each(block->transactions,
		(node_func_t)_transaction_is_valid, (void *)all_unspent) != 0)
	{
		return (-1);
	}

	return (0);
}
