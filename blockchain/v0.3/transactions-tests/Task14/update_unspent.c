#include <stdlib.h>
#include <string.h>

#include "transaction.h"

static int _tx_in_dup(tx_in_t const *in, unsigned int idx, llist_t *all_inputs)
{
	tx_in_t *dup;

	dup = malloc(sizeof(*dup));
	memcpy(dup, in, sizeof(*dup));

	llist_add_node(all_inputs, dup, ADD_NODE_REAR);

	(void)idx;
	return (0);
}

static int _append_inputs(transaction_t const *transaction, unsigned int idx,
	llist_t *all_inputs)
{
	llist_for_each(transaction->inputs, (node_func_t)_tx_in_dup, all_inputs);

	(void)idx;
	return (0);
}

/**
 * _out_to_unspent - Creates an unspent transaction output refering to a
 * given transaction output
 *
 * Description: This function is called by `llist_for_each_arg`
 *
 * @out:  Pointer to the transaction output
 * @args: Array of arguments
 *        args[0] -> Pointer to the transaction the output belongs to
 *        args[1] -> New list of unspent transaction outputs
 *
 * Return: FOREACH_CONTINUE
 */
static int _out_to_unspent(tx_out_t const *out, unsigned int idx,
	void *args[3])
{
	unspent_tx_out_t *unspent = malloc(sizeof(*unspent));
	transaction_t const *transaction = (transaction_t const *)args[0];
	llist_t *new_unspent = (llist_t *)args[1];
	uint8_t *block_hash = (uint8_t *)args[2];

	memcpy(unspent->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(unspent->tx_id, transaction->id, SHA256_DIGEST_LENGTH);
	memcpy(&unspent->out, out, sizeof(*out));

	llist_add_node(new_unspent, unspent, ADD_NODE_REAR);

	(void)idx;
	return (0);
}

/**
 * _check_consumed_in - Checks whether an unspent transaction output is
 * referenced by a transaction input
 *
 * Description: This function is called by `llist_for_each_arg`
 *
 * @in:  Pointer to the transaction input
 * @out: Pointer to the unspent transaction output
 *
 * Return: FOREACH_CONTINUE if the transaction input does not refer to the
 *         unspent transaction output.
 *         !FOREACH_CONTINUE is returned otherwise, stoping the loop
 */
static int _check_consumed_input(tx_in_t const *in, unsigned int idx,
	unspent_tx_out_t const *out)
{
	if (memcmp(in->block_hash, out->block_hash, SHA256_DIGEST_LENGTH) == 0 &&
		memcmp(in->tx_id, out->tx_id, SHA256_DIGEST_LENGTH) == 0 &&
		memcmp(in->tx_out_hash, out->out.hash, SHA256_DIGEST_LENGTH) == 0)
	{
		/*
		 * Transaction input matches an unspent output.
		 * The output is consumed, stop the loop
		 */
		return (1);
	}

	(void)idx;
	return (0);
}

/**
 * _ignore_consumed_unspent - Adds an unspent transaction output to the new
 * list only if it is not referenced by a transaction input
 *
 * Description: This function is called by `llist_for_each_arg`
 *
 * @out:
 * @args: Array of arguments
 *        args[0] -> List of all the transaction inputs
 *        args[1] -> New list of unspent transaction outputs
 *
 * Return: FOREACH_CONTINUE
 */
static int _ignore_consumed_unspent(unspent_tx_out_t const *out,
	unsigned int idx, void *args[2])
{
	llist_t *all_inputs = (llist_t *)args[0];
	llist_t *new_unspent = (llist_t *)args[1];

	if (llist_for_each(all_inputs, (node_func_t)_check_consumed_input,
		(void *)out) == 0)
	{
		unspent_tx_out_t *u = malloc(sizeof(*u));

		/*
		 * Unspent transaction output is not consumed, add it
		 * to the new list
		 */
		memcpy(u, out, sizeof(*u));
		llist_add_node(new_unspent, u, ADD_NODE_REAR);
	}

	(void)idx;
	return (0);
}
void _print_hex_buffer(uint8_t const *buf, size_t len);
/**
 * _append_outputs - Appends all the outputs of a transaction to the new list
 * of unspent transaction outputs
 *
 * Description: This function is called by `llist_for_each_arg`
 *
 * @tx:          Pointer to the transaction to get all the outputs from
 * @new_unspent: New list of unspent transaction outputs
 *
 * Return: FOREACH_CONTINUE
 */
static int _append_outputs(transaction_t const *transaction, unsigned int idx,
	void *args[2])
{
	llist_t *new_unspent = (llist_t *)args[0];
	uint8_t *block_hash = (uint8_t *)args[1];
	void *_args[3];

	/*
	 * Include each output of the transaction to the new unspent
	 * output list
	 */
	_args[0] = (void *)transaction;
	_args[1] = (void *)new_unspent;
	_args[2] = (void *)block_hash;
	llist_for_each(transaction->outputs, (node_func_t)_out_to_unspent,
		_args);

	(void)idx;
	return (0);
}

/**
 * update_unspent - Updates the list of unspent transaction outputs, given
 * a list of processed transactions
 *
 * @transactions: List of processed transactions
 * @all_unspent:  Current list of all unspent transaction outputs
 *
 * Return: A new list of unspent transaction outputs
 */
llist_t *update_unspent(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	llist_t *new_unspent = llist_create(MT_SUPPORT_FALSE);
	llist_t *all_inputs;
	void *args[2];

	all_inputs = llist_create(MT_SUPPORT_FALSE);
	llist_for_each(transactions, (node_func_t)_append_inputs,
		(void *)all_inputs);

	/*
	 * For each unspent transaction output, check if it is contained in
	 * the transaction inputs of all the transactions.
	 * If it is, don't include it in the new unspent output list
	 */
	args[0] = (void *)all_inputs;
	args[1] = (void *)new_unspent;
	llist_for_each(all_unspent,
		(node_func_t)_ignore_consumed_unspent, args);
	llist_destroy(all_inputs, 0, NULL);

	/* Include all new outputs as well */
	args[0] = (void *)new_unspent;
	args[1] = (void *)block_hash;
	llist_for_each(transactions, (node_func_t)_append_outputs, args);

	/* Remove old list */
	llist_destroy(all_unspent, 1, free);

	return (new_unspent);
}
