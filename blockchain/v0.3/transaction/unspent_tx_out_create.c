#include "blockchain.h"
#include "transaction.h"

/**
 * unspent_tx_out_create - allocates and initializes an unspent transaction
 *  output structure
 * @block_hash: contains the hash of the Block where the referenced transaction
 *  output is located
 * @tx_id: contains the hash of a transaction  in the Block block_hash,
 *  where the refernced transaction output is located
 * @out: points to the referenced transaction output
 *
 * Return: pointer to the created unspent transaction output, NULL on failure
 */
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH],
	tx_out_t const *out)
{
	unspent_tx_out_t *tx = NULL;

	if (!block_hash || !out)
		return (NULL);

	tx = (unspent_tx_out_t *)malloc(sizeof(unspent_tx_out_t));
	if (!tx)
		return (NULL);

	memcpy(tx->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(tx->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	tx->out.amount = out->amount;
	memcpy(tx->out.pub, out->pub, EC_PUB_LEN);
	memcpy(tx->out.hash, out->hash, SHA256_DIGEST_LENGTH);

	return (tx);
}
