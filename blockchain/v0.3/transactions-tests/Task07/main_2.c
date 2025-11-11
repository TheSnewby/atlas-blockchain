/* Description: Create 10 coinbase transactions */

#include <stdlib.h>

#include "transaction.h"

EC_KEY *_ec_load(char const *folder);
void _transaction_print_brief(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	transaction_t *coinbase;
	EC_KEY *owner;
	uint32_t i;

	owner = _ec_load("_keys/alex");

	for (i = 0; i < 10; i++)
	{
		coinbase = coinbase_create(owner, i);
		_transaction_print_brief(coinbase);
	}

	/* Cleanup */
	EC_KEY_free(owner);

	return (EXIT_SUCCESS);
}
