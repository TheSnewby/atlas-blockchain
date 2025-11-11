#include <string.h>

#include "transaction.h"

int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t id[SHA256_DIGEST_LENGTH];
	tx_in_t const *in;
	tx_out_t const *out;

	if (!coinbase)
		return (0);

	/* Check Transaction ID */
	transaction_hash(coinbase, id);
	if (memcmp(coinbase->id, id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	/* Coinbase transaction must have a single input */
	if (llist_size(coinbase->inputs) != 1)
		return (0);

	in = (tx_in_t *)llist_get_head(coinbase->inputs);
	if (memcmp(in->tx_out_hash, &block_index, sizeof(block_index)) != 0)
		return (0);

	/* Coinbase transaction must have a single output */
	if (llist_size(coinbase->outputs) != 1)
		return (0);

	/* Coinbase amount is always the same */
	out = (tx_out_t *)llist_get_head(coinbase->outputs);
	if (out->amount != COINBASE_AMOUNT)
		return (0);

	return (1);
}
