#include "blockchain.h"

/**
 * coinbase_create - creates a coinbase transaction
 * @receiver: public key of the miner, who will receive the coinbase coins
 * @block_index: index of the Block the coinbase transaction will belong to
 *
 * Return: created transaction, NULL on failure
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	
}
