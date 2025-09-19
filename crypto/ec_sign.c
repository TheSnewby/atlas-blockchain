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
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t
 msglen, sig_t *sig)
 {
	EVP_PKEY *evp_pkey = NULL;
	EVP_MD_CTX *emc = NULL;
	size_t sig_len = 0;

	if (!key || !msg)
		return (NULL);

	evp_pkey = EVP_PKEY_new();
	if (!EVP_PKEY_set1_EC_KEY(evp_pkey, (EC_KEY *)key))
	{
		fprintf(stderr, "Failure at EVP_PKEY_set1_EC_KEY\n");
		return (NULL);
	}

	emc = EVP_MD_CTX_new();
	if (!emc)
	{
		fprintf(stderr, "Failure at EVP_MD_CTX_new\n");
		return (NULL);
	}

	if (!EVP_DigestSignInit(emc, NULL, EVP_sha256(), NULL, evp_pkey))
	{
		fprintf(stderr, "Failure at EVP_DigestSignInit\n");
		return (NULL);
	}

	if (!EVP_DigestSignUpdate(emc, msg, msglen))
	{
		fprintf(stderr, "Failure at EVP_DigestSignUpdate\n");
		return (NULL);
	}

	if (!EVP_DigestSignFinal(emc, NULL, &sig_len))
	{
		fprintf(stderr, "Failure at EVP_DigestSignFinal\n");
		return (NULL);
	}

	if (sig_len > SIG_MAX_LEN)
	{
		fprintf(stderr, "Signature length exceeds SIG_MAX_LEN\n");
		return (NULL);
	}
	sig->len = (uint8_t)sig_len;

		if (!EVP_DigestSignFinal(emc, sig->sig, &sig_len))
	{
		fprintf(stderr, "Failure at EVP_DigestSignFinal\n");
		return (NULL);
	}

	return (sig->sig);
 }
