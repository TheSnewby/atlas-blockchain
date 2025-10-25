#include "blockchain.h"

/**
 * transaction_create - creates a transaction
 * @sender: contains the private key of the transaction sender
 * @receiver: contains the public key of the transaction receiver
 * @amount: amount to send
 * @all_unspent: list of all the unspent outputs to date
 *
 * Return: created transaction, NULL on failure
 */
transaction_t *transaction_create(
	EC_KEY const *sender,
	EC_KEY const *receiver,
	uint32_t amount,
	llist_t *all_unspent)
{
	
}
