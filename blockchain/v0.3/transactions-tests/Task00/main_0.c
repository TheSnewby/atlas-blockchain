/* Description: Create a transaction output with an amount of 972 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "hblk_crypto.h"
#include "transaction.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	EC_KEY *receiver;
	uint8_t receiver_pub[EC_PUB_LEN];
	uint8_t hash[SHA256_DIGEST_LENGTH];
	tx_out_t *out;

	receiver = ec_create();

	out = tx_out_create(972, ec_to_pub(receiver, receiver_pub));
	sha256((int8_t *)out, sizeof(out->amount) + sizeof(out->pub), hash);
	if (memcmp(out->hash, hash, SHA256_DIGEST_LENGTH) != 0)
		printf("Invalid hash\n");
	else
		printf("Transaction output created\n");

	free(out);
	EC_KEY_free(receiver);

	return (EXIT_SUCCESS);
}
