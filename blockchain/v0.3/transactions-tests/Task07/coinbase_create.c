#include <stdlib.h>
#include <string.h>

#include "transaction.h"

transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *coinbase;
	tx_in_t *in;
	tx_out_t *out;
	uint8_t pub[EC_PUB_LEN];

	coinbase = malloc(sizeof(*coinbase));
	memset(coinbase, 0, sizeof(*coinbase));
	if (!coinbase)
		return (NULL);
	coinbase->inputs = llist_create(MT_SUPPORT_FALSE);
	coinbase->outputs = llist_create(MT_SUPPORT_FALSE);

	in = malloc(sizeof(*in));
	memset(in, 0, sizeof(*in));
	memcpy(in->tx_out_hash, &block_index, sizeof(block_index));
	llist_add_node(coinbase->inputs, in, ADD_NODE_REAR);

	out = tx_out_create(COINBASE_AMOUNT, ec_to_pub(receiver, pub));
	llist_add_node(coinbase->outputs, out, ADD_NODE_REAR);

	transaction_hash(coinbase, coinbase->id);

	return (coinbase);
}
