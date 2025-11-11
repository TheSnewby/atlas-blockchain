/* Description: Create a transaction with one input and one output and sign the input */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hblk_crypto.h"
#include "transaction.h"
#include "blockchain.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t transaction_id[SHA256_DIGEST_LENGTH];
	tx_out_t *out, *out2;
	EC_KEY *owner;
	uint8_t pub[EC_PUB_LEN];
	llist_t *all_unspent;
	unspent_tx_out_t *unspent;
	tx_in_t *in;
	transaction_t transaction;

	sha256((int8_t *)"Block", strlen("Block"), block_hash);
	sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);
	owner = ec_create();

	all_unspent = llist_create(MT_SUPPORT_FALSE);
	transaction.inputs = llist_create(MT_SUPPORT_FALSE);
	transaction.outputs = llist_create(MT_SUPPORT_FALSE);

	out = tx_out_create(972, ec_to_pub(owner, pub));
	unspent = unspent_tx_out_create(block_hash, transaction_id, out);
	llist_add_node(all_unspent, unspent, ADD_NODE_REAR);

	in = tx_in_create(unspent);
	llist_add_node(transaction.inputs, in, ADD_NODE_REAR);
	memset(pub, 23, EC_PUB_LEN); /* Pretend it's a different address :) */
	out2 = tx_out_create(972, pub);
	llist_add_node(transaction.outputs, out2, ADD_NODE_REAR);

	transaction_hash(&transaction, transaction.id);
	/* Sign transaction input with transaction hash */
	tx_in_sign(in, transaction.id, owner, all_unspent);
	if (ec_verify(owner, transaction.id, sizeof(transaction.id), &in->sig))
		printf("Signature verified\n");
	else
		printf("Signature invalid\n");

	/* Cleanup */
	EC_KEY_free(owner);
	free(out);
	llist_destroy(all_unspent, 1, free);
	llist_destroy(transaction.inputs, 1, free);
	llist_destroy(transaction.outputs, 1, free);
	return (EXIT_SUCCESS);
}
