#include "hblk_crypto.h"

/**
 * ec_from_pub - creates an EC_KEY structure given a public key
 * @pub: contains the public key to be converted
 *
 * Return: pointer to the created EC_KEY structure, or NULL on failure
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *new_key = NULL;
	EC_POINT *new_pt = NULL;
	const EC_GROUP *new_gp = NULL;

	if (!pub)
		return (NULL);

	new_key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!new_key)
		return (NULL);

	new_gp = EC_KEY_get0_group(new_key);
	if (!new_gp)
	{
		fprintf(stderr, "EC_KEY_get0_group failure\n");
		return (NULL);
	}

	new_pt = EC_POINT_new(new_gp);
	if (!new_pt)
	{
		fprintf(stderr, "new_pt creation failure\n");
		return (NULL);
	}

	if (!EC_POINT_oct2point(new_gp, new_pt, pub, EC_PUB_LEN, NULL))
	{
		fprintf(stderr, "EC_POINT_oct2point failed\n");
		return (NULL);
	}

	if (!EC_KEY_set_public_key(new_key, new_pt))
	{
		fprintf(stderr, "EC_KEY_set_public_key failure\n");
		return (NULL);
	}

	return (new_key);
}
