#include "blockchain.h"

/**
 * coinbase_is_valid - checks whether a coinbase transaction is valid
 * @coinbase: coinbase transaction to verify
 * @block_index: index of the Block the coinbase transaction will belong to
 *
 * Return: 1 if the coinbase transaction is valid, 0 otherwise
 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t test_id[SHA256_DIGEST_LENGTH];
	uint8_t zeroed_buffer[SHA256_DIGEST_LENGTH] = {0};
	tx_in_t *tx_in = NULL;
	tx_out_t *tx_out = NULL;

	if (!coinbase)
		return (0);

	if (llist_size(coinbase->inputs) != 1 || llist_size(coinbase->outputs) != 1)
		return (0);

	transaction_hash(coinbase, test_id);
	if (memcmp(coinbase->id, test_id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	tx_out = (tx_out_t *)llist_get_node_at(coinbase->outputs, 0);
	if (tx_out->amount != COINBASE_AMOUNT)
		return (0);

	tx_in = (tx_in_t *)llist_get_node_at(coinbase->inputs, 0);
	if (memcmp(tx_in->tx_out_hash, &block_index, sizeof(uint32_t)) != 0)
		return (0);
	if (memcmp(tx_in->block_hash, zeroed_buffer, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	if (memcmp(tx_in->tx_id, zeroed_buffer, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	if (memcmp(tx_in->sig.sig, zeroed_buffer, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	return (1);
}
