/* Description: Creates a transaction input that refers to a created unspent transaction output, that itself refers to a transaction output with an amount of 972 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

static void _tx_in_print(tx_in_t const *in)
{
	if (!in)
		return;

	printf("Transaction input: {\n");

	printf("\tblock_hash: ");
	_print_hex_buffer(in->block_hash, sizeof(in->block_hash));
	printf(",\n");
	printf("\ttx_id: ");
	_print_hex_buffer(in->tx_id, sizeof(in->tx_id));
	printf(",\n");
	printf("\ttx_out_hash: ");
	_print_hex_buffer(in->tx_out_hash, sizeof(in->tx_out_hash));
	printf("\n");
	printf("\tsig: ");
	if (in->sig.len)
		_print_hex_buffer(in->sig.sig, in->sig.len);
	else
		printf("null");
	printf("\n");

	printf("}\n");
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t transaction_id[SHA256_DIGEST_LENGTH];
	tx_out_t *out;
	uint8_t pub[EC_PUB_LEN];
	unspent_tx_out_t *unspent;
	tx_in_t *in;

	sha256((int8_t *)"Block test", strlen("Block test"), block_hash);
	sha256((int8_t *)"Transaction test", strlen("Transaction test"), transaction_id);
	memset(pub, 98, EC_PUB_LEN);
	out = tx_out_create(972, pub);
	unspent = unspent_tx_out_create(block_hash, transaction_id, out);

	in = tx_in_create(unspent);
	_tx_in_print(in);

	free(out);
	free(unspent);
	free(in);

	return (EXIT_SUCCESS);
}
