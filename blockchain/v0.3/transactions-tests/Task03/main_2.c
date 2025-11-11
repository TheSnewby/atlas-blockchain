/* Description: Creates a transaction containing two inputs and one output, and compute its ID */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _transaction_print(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t block_hash2[SHA256_DIGEST_LENGTH];
	uint8_t transaction_id[SHA256_DIGEST_LENGTH];
	uint8_t transaction_id2[SHA256_DIGEST_LENGTH];
	tx_out_t *out, *out2;
	uint8_t pub[EC_PUB_LEN];
	llist_t *all_unspent;
	unspent_tx_out_t *unspent;
	tx_in_t *in;
	transaction_t transaction;

	sha256((int8_t *)"Block test", strlen("Block test"), block_hash);
	sha256((int8_t *)"Block test 2", strlen("Block test 2"), block_hash2);
	sha256((int8_t *)"Transaction test", strlen("Transaction test"), transaction_id);
	sha256((int8_t *)"Transaction test 2", strlen("Transaction test 2"), transaction_id2);

	memset(pub, 98, EC_PUB_LEN);
	transaction.inputs = llist_create(MT_SUPPORT_FALSE);
	transaction.outputs = llist_create(MT_SUPPORT_FALSE);
	all_unspent = llist_create(MT_SUPPORT_FALSE);

	out = tx_out_create(72, pub);
	unspent = unspent_tx_out_create(block_hash, transaction_id, out);
	llist_add_node(all_unspent, unspent, ADD_NODE_REAR);
	in = tx_in_create(unspent);
	llist_add_node(transaction.inputs, in, ADD_NODE_REAR);

	free(out);
	out = tx_out_create(900, pub);
	unspent = unspent_tx_out_create(block_hash2, transaction_id2, out);
	llist_add_node(all_unspent, unspent, ADD_NODE_REAR);
	in = tx_in_create(unspent);
	llist_add_node(transaction.inputs, in, ADD_NODE_REAR);

	memset(pub, 35, EC_PUB_LEN); /* Pretend it's a different address :) */
	out2 = tx_out_create(972, pub);
	llist_add_node(transaction.outputs, out2, ADD_NODE_REAR);

	/* Compute transaction id (hash) */
	transaction_hash(&transaction, transaction.id);
	_transaction_print(&transaction);

	/* Cleanup */
	free(out);
	llist_destroy(all_unspent, 1, free);
	llist_destroy(transaction.inputs, 1, free);
	llist_destroy(transaction.outputs, 1, free);
	return (EXIT_SUCCESS);
}
