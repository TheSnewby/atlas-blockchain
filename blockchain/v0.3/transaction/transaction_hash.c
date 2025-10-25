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

}
