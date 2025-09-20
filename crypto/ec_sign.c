#include "hblk_crypto.h"

/**
 * ec_sign - signs a given set of bytes, using a given EC_KEY private key
 * @key: EC_KEY structure containing the private key
 * @msg: points to characters to be signed
 * @msglen: length of msg
 * @sig: holds the address at which to store the signature
 *
 * Return: pointer to the signature buffer, NULL on failure
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg,
	size_t msglen, sig_t *sig)
{
	if (!key || !msg)
		return (NULL);

	if (!ECDSA_sign(0, msg, (int)msglen, sig->sig,
	(unsigned int *)&sig->len, (EC_KEY *)key))
		return (NULL);

	return (sig->sig);
}
