#include "blockchain.h"

/**
 * calculate_output_amount - verifies unspent amounts match the transaction
 * @transaction: transaction to verify
 *
 * Return: sum of output amounts
 */
uint32_t calculate_output_amount(transaction_t const *transaction)
{
	int out_size;
	uint32_t output_amount = 0;
	tx_out_t *out = NULL;

	out_size = llist_size(transaction->outputs);
	if (out_size >= 1)
	{
		out = (tx_out_t *)llist_get_node_at(transaction->outputs, 0);
		output_amount += out->amount;
		fprintf(stderr, "1st output: %d", out->amount);
	}
	if (out_size == 2)
	{
		out = (tx_out_t *)llist_get_node_at(transaction->outputs, 1);
		output_amount += out->amount;

		fprintf(stderr, ", 2nd output: %d", out->amount);
	}
	if (out_size > 2)
		fprintf(stderr, "More than two outputs. out_size: %d\n", out_size);
	fprintf(stderr, "\n");

	return (output_amount);
}

/**
 * check_unspent_vs_input - checks whether an unspent tx is part of the input
 * @unspent: individual unspent transaction
 * @input: individual input transaction
 *
 * Return: 1 if matching, 0 otherwise
 */
int check_unspent_vs_input(unspent_tx_out_t *unspent, tx_in_t *input)
{
	if ((memcmp(input->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH)
		== 0) &&
		(memcmp(input->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH) == 0) &&
		(memcmp(input->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH)
		== 0))
		return (1);
	return (0);
}

/**
 * verify_inputs - verifies inputs used in transaction vs unspent
 * @transaction: transaction to verify
 * @all_unspent: is the list of all unspent transaction outputs to date
 * @input_amount: sum of matching amounts in inputs
 *
 * Return: 1 on sucess, 0 otherwise
 */
int verify_inputs(transaction_t const *transaction, llist_t *all_unspent,
	uint32_t *input_amount)
{
	int i, j, inputs_size, all_unspent_size, found;
	tx_in_t *input = NULL;
	unspent_tx_out_t *unspent = NULL;
	EC_KEY *unspent_key = NULL;

	inputs_size = llist_size(transaction->inputs);
	all_unspent_size = llist_size(all_unspent);

	for (i = 0; i < inputs_size; i++) /* input verification with unspent */
	{
		input = (tx_in_t *)llist_get_node_at(transaction->inputs, i);
		found = 0;
		for (j = 0; j < all_unspent_size; j++)
		{
			unspent = (unspent_tx_out_t *)llist_get_node_at(all_unspent, j);
			unspent_key = ec_from_pub(unspent->out.pub);
			if (check_unspent_vs_input(unspent, input))
			{
				found = 1;
				/* verify sig of input using matching unspent's public key */
				if (!ec_verify(unspent_key, transaction->id,
					SHA256_DIGEST_LENGTH, &input->sig))
				{
					fprintf(stderr, "!ec_verify\n");
					EC_KEY_free(unspent_key);
					return (0);
				}
				*input_amount += unspent->out.amount;
				break;
			}
		}
		if (!found)
		{
			fprintf(stderr, "!found\n");
			EC_KEY_free(unspent_key);
			return (0);
		}
	}
	EC_KEY_free(unspent_key);
	return (1);
}

/**
 * transaction_is_valid - checks whether a transaction is valid
 * @transaction: transaction to verify
 * @all_unspent: is the list of all unspent transaction outputs to date
 *
 * Return: 1 if the transaction is valid, 0 otherwise
 */
int transaction_is_valid(transaction_t const *transaction,
	llist_t *all_unspent)
{
	uint8_t test_hash[SHA256_DIGEST_LENGTH] = {0};
	uint32_t output_amount = 0, input_amount = 0;


	if (!transaction || !all_unspent)
	{
		fprintf(stderr, "!params in transaction_is_valid\n");
		return (0);
	}

	transaction_hash(transaction, test_hash); /* transaction hash verification */
	if (memcmp(test_hash, transaction->id, SHA256_DIGEST_LENGTH) != 0)
	{
		fprintf(stderr, "!transaction hash\n");
		return (0);
	}

	/* O(n^2) doesn't matter with small sizes */
	if (!verify_inputs(transaction, all_unspent, &input_amount))
		return (0);

	output_amount = calculate_output_amount(transaction);
	if (output_amount != input_amount)
	{
		fprintf(stderr, "outputs != inputs outputs: %d, inputs: %d\n",
			output_amount, input_amount);
		fprintf(stderr, "output_size: %d, input_size: %d\n",
			llist_size(transaction->outputs), llist_size(transaction->inputs));
		return (0);
	}

	return (1);
}
