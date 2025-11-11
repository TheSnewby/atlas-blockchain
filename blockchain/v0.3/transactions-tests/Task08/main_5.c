/* Description: Input's block_hash is not zeroed */

#include <stdlib.h>
#include <limits.h>

#include "transaction.h"

EC_KEY *_ec_load(char const *folder);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	transaction_t *coinbase;
	EC_KEY *owner;
	tx_in_t *in;

	owner = _ec_load("_keys/alex");

	coinbase = coinbase_create(owner, UINT_MAX);
	in = llist_get_head(coinbase->inputs);
	in->block_hash[0] = 98;
	if (coinbase_is_valid(coinbase, UINT_MAX - 1))
		printf("Coinbase transaction valid\n");
	else
		printf("Coinbase transaction invalid\n");

	/* Cleanup */
	EC_KEY_free(owner);

	return (EXIT_SUCCESS);
}
