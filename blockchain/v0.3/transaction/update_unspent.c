#include "blockchain.h"

/**
 * update_unspent - updates the list of all unspent transaction outputs,
 *  given a list of processed transactions
 * @transaction: list of validated transactions
 * @block_hash: hash of the validated block that contains the
 *  transaction list transactions
 * @all_unspent: current list of unspent transaction outputs
 *
 * Return: new list of unspent transaction outputs
 */
llist_t *update_unspent(
	llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	llist_t *all_unspent)
{
	
}
