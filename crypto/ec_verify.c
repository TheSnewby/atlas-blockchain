#include "hblk_crypto.h"

/**
 * ec_verify - verifies the signature of a given set of bytes
 *  using a given EC_KEY
 * @key: EC_KEY structure
 * @msg: points to characters to verify signature
 * @msglen: length of msg
 * @sig: points to the signature to be checked
 *
 * Return: 1 if the signature is valid, 0 otherwise
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg,
	size_t msglen, sig_t const *sig)
{
	if (!key || !msg || !sig || sig->len <= 0)
		return (0);

	if (!ECDSA_verify(0, msg, msglen, sig->sig, sig->len, (EC_KEY *)key))
		return (0);

	return (1);
}
