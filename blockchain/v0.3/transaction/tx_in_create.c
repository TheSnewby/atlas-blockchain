#include "blockchain.h"

/**
 * tx_in_create - allocates and initializes a transaction input structure
 * @unspent: unspent transaction output to be converted to a transaction input
 *
 * Return: pointer to the created transaction intput upon success,
 *  NULL on failure
 */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *tx = NULL;

	if (!unspent)
		return (NULL);

	tx = (tx_in_t *)malloc(sizeof(tx_in_t));
	if (!tx)
		return (NULL);

	memcpy(tx->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(tx->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(tx->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);
	memset(tx->sig.sig, 0, SIG_MAX_LEN);
	tx->sig.len = 0;

	return (tx);
}
