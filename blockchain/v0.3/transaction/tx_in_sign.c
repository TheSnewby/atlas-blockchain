#include "blockchain.h"

/**
 * tx_in_sign - signs a transaction input, given the transaction id it is from
 * @in: points to the transaction input structure to sign
 * @tx_id: contains the ID (hash) of the transaction the transaction input
 *  to sign is stored in
 * @sender: contains the private key of the receiver of the coins
 * @all_unspent: the list of all unspent transaction outputs to date
 */
sig_t *tx_in_sign(
	tx_in_t *in,
	uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender,
	llist_t *all_unspent)
{
	
}
