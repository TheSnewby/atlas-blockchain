#include "blockchain.h"

/**
 * blockchain_difficulty - computes the difficulty to assign to a potential
 *  next Block in the Blockchain
 * @blockchain: blockchain
 * Return: difficulty to potential next block
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	uint32_t next_diff;
	block_t *last_block, *last_changed_block;
	uint64_t expected_time_elapsed, actual_time_elapsed;

	if (!blockchain)
		return (0);

	last_block = (block_t *)llist_get_tail(blockchain->chain);
	next_diff = last_block->info.difficulty;

	if (last_block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL == 0 &&
		last_block->info.index != 0)
	{
		last_changed_block = (block_t *)llist_get_node_at(blockchain->chain,
			last_block->info.index - DIFFICULTY_ADJUSTMENT_INTERVAL);
		expected_time_elapsed = BLOCK_GENERATION_INTERVAL *
		DIFFICULTY_ADJUSTMENT_INTERVAL;
		actual_time_elapsed = last_block->info.timestamp -
		last_changed_block->info.timestamp;

		/* Too Fast/Easy */
		if (actual_time_elapsed < (expected_time_elapsed / 2))
			return (++next_diff);
		/* Too Slow/Difficult */
		else if ((actual_time_elapsed > (expected_time_elapsed * 2)) &&
		(next_diff != 0))
			return (--next_diff);
	}

	return (last_block->info.difficulty);
}
