#include "hblk_crypto.h"

/**
 * ec_to_pub - extracts the public key from an EC_KEY opaque structure
 * @key: pointer to the EC_KEY structure
 * @pub: address at which to store the extracted public key (not compressed)
 *
 * Return: pointer to pub
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	size_t ecp2o_ret;

	if (!key || !pub)
		return (NULL);

	ecp2o_ret = EC_POINT_point2oct(
		EC_KEY_get0_group(key),
		EC_KEY_get0_public_key(key),
		POINT_CONVERSION_UNCOMPRESSED,
		pub,
		EC_PUB_LEN,
		NULL);

	if (ecp2o_ret == 0)
		return (NULL);

	return (pub);
}
