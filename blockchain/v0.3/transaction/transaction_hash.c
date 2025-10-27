#include "blockchain.h"

/**
 * transaction_hash - computes the ID (hash) of a transaction
 * @transaction: transaction to compute the hash of
 * @hash_buf: buffer in which to stre the computed hash
 *
 * Return: pointer to hash_buf
 */
uint8_t *transaction_hash(
	transaction_t const *transaction,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int inputs_size, outputs_size, hash_me_size = 0, i, j;
	uint8_t *hash_me = NULL;
	tx_in_t *tx_in = NULL;
	tx_out_t *tx_out = NULL;

	if (!transaction || !hash_buf)
		return (0);

	inputs_size = llist_size(transaction->inputs);
	outputs_size = llist_size(transaction->outputs);
	hash_me_size = (32 * 3 * inputs_size) + (32 * outputs_size);

	hash_me = (uint8_t *)malloc(hash_me_size);
	if (!hash_me)
		return (NULL);

	/* might need to change to i = 1 if i = 0 is a genesis value */
	for (i = 0; i < inputs_size; i++) /* inputs tx_in_t */
	{
		tx_in = (tx_in_t *)llist_get_node_at(transaction->inputs, i);
		memcpy(hash_me + i * 32 * 3, tx_in->block_hash, SHA256_DIGEST_LENGTH);
		memcpy(hash_me + i * 32 * 3 + 32, tx_in->tx_id, SHA256_DIGEST_LENGTH);
		memcpy(hash_me + i * 32 * 3 + 64, tx_in->tx_out_hash,
			SHA256_DIGEST_LENGTH);
	}

	fprintf(stderr, "inputs_size: %d, i: %d\n", inputs_size, i);

	for (j = 0; j < outputs_size; j++)
	{
		tx_out = (tx_out_t *)llist_get_node_at(transaction->outputs, j);
		memcpy(hash_me + i * 32 * 3 + j * 32, tx_out->hash,
			SHA256_DIGEST_LENGTH);
	}

	SHA256(hash_me, hash_me_size, hash_buf);

	return (hash_buf);
}
