#include "hblk_crypto.h"

/**
 * sha256 - computes the hash of a sequence of bytes
 * @s: sequence of bytes to be hashed
 * @len: number of bytes to hash
 * @digest: buffer to store the hash
 *
 * Return: pointer to digest, or NULL on failure
 */
uint8_t *sha256(int8_t const *s, size_t len, uint8_t
 digest[SHA256_DIGEST_LENGTH])
{
	SHA256_CTX *c = NULL;

	if (!s || len == 0 || !digest)
		return (NULL);

	if (!SHA256_Init(c))
		return (NULL);

	if (!SHA256_Update(c, s, len))
		return (NULL);

	if (!SHA256_Final(digest, c))
		return (NULL);

	return (digest);
}
