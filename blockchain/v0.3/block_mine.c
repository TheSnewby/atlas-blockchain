#include "blockchain.h"

/**
 * block_mine - mines a block in order to insert it in the blockchain.
 *  finds a hash for the Block to be mined that matches its difficulty
 * @block: points to Block to be mined
 */
void block_mine(block_t *block)
{
	for (block->info.nonce = 0;
		block->info.nonce < UINT64_MAX; block->info.nonce++)
	{
		/* GENERATE A HASH */
		SHA256((unsigned char *)block,
		block->data.len + sizeof(block->info),
		block->hash);

		/* CHECK */
		if (hash_matches_difficulty(block->hash, block->info.difficulty))
			break;
	}
}
