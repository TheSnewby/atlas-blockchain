#include <stdlib.h>
#include <string.h>

#include "transaction.h"

tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *in;

	in = malloc(sizeof(*in));
	if (!in)
		return (NULL);

	memcpy(in->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(in->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(in->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);
	memset(&in->sig, 0, sizeof(in->sig));

	return (in);
}
