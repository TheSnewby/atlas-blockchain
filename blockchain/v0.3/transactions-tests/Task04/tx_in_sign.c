#include <stdlib.h>
#include <string.h>

#include "transaction.h"

/**
 * _is_ref_unspent - Checks that a given unspent transaction
 * output matches a given transaction input reference
 *
 * @out: Pointer to the unspent transaction output
 * @in:  Pointer to the transaction input
 *
 * Return: 1 if they match, 0 otherwise
 */
static int _is_ref_unspent(unspent_tx_out_t const *unspent, tx_in_t const *in)
{
	if (memcmp(unspent->block_hash, in->block_hash,
		sizeof(in->block_hash)) == 0 &&
	    memcmp(unspent->tx_id, in->tx_id,
		sizeof(in->tx_id)) == 0 &&
	    memcmp(unspent->out.hash, in->tx_out_hash,
		sizeof(in->tx_out_hash)) == 0)
	{
		return (1);
	}

	return (0);
}

sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender, llist_t *all_unspent)
{
	unspent_tx_out_t *unspent;
	uint8_t pub[EC_PUB_LEN];
	node_ident_t ident = (node_ident_t)&_is_ref_unspent;

	ec_to_pub(sender, pub);
	unspent = llist_find_node(all_unspent, ident, in);
	if (memcmp(pub, unspent->out.pub, EC_PUB_LEN) != 0)
		return (NULL);

	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);

	return (&in->sig);
}
