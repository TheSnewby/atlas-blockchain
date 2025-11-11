/* Description: Create a coinbase transaction */

#include <stdlib.h>

#include "transaction.h"

EC_KEY *_ec_load(char const *folder);
void _transaction_print(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	transaction_t *coinbase;
	EC_KEY *owner;

	owner = _ec_load("_keys/alex");

	coinbase = coinbase_create(owner, 1);
	_transaction_print(coinbase);

	/* Cleanup */
	EC_KEY_free(owner);

	return (EXIT_SUCCESS);
}
