#include "blockchain.h"
#include "transaction.h"

/**
 * tx_out_create - allocates and initializes a transaction output structure
 * @amount: amount of the transaction
 * @pub: public key of the trsnaction receiver
 * Return: pointer to the created transaction output, NULL on failure
 */
tx_out_t *tx_out_create(
	uint32_t amount,
	uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *tx = NULL;
	uint8_t buffer_for_hashing[sizeof(uint32_t) + EC_PUB_LEN];

	if (!pub)
		return (NULL);

	tx = (tx_out_t *)malloc(sizeof(tx_out_t));
	if (!tx)
		return (NULL);

	tx->amount = amount;
	memcpy(tx->pub, pub, sizeof(uint8_t) * EC_PUB_LEN);
	memcpy(buffer_for_hashing, &amount, sizeof(uint32_t));
	memcpy(buffer_for_hashing + sizeof(uint32_t), pub, EC_PUB_LEN);
	SHA256(buffer_for_hashing, sizeof(buffer_for_hashing), tx->hash);

	return (tx);
}
