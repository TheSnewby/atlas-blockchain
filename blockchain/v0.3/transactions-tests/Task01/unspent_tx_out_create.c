#include <stdlib.h>
#include <string.h>

#include "blockchain.h"

unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out)
{
	unspent_tx_out_t *unspent;

	unspent = malloc(sizeof(*unspent));
	if (!unspent)
		return (NULL);

	memcpy(unspent->block_hash, block_hash, sizeof(unspent->block_hash));
	memcpy(unspent->tx_id, tx_id, sizeof(unspent->tx_id));
	memcpy(&unspent->out, out, sizeof(unspent->out));

	return (unspent);
}
