#include <stdlib.h>
#include <string.h>

#include "transaction.h"

tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *out;

	out = malloc(sizeof(*out));
	if (!out)
		return (NULL);

	out->amount = amount;
	memcpy(out->pub, pub, EC_PUB_LEN);
	sha256((int8_t *)out, sizeof(out->amount) + sizeof(out->pub), out->hash);

	return (out);
}
