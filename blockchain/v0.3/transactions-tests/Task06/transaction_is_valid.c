#include <string.h>

#include "transaction.h"

static int _is_unspent(unspent_tx_out_t const *unspent, tx_in_t const *in)
{
	if (memcmp(unspent->block_hash, in->block_hash,
		sizeof(in->block_hash)) == 0 &&
	    memcmp(unspent->tx_id, in->tx_id, sizeof(in->tx_id)) == 0 &&
	    memcmp(unspent->out.hash, in->tx_out_hash,
		sizeof(in->tx_out_hash)) == 0)
	{
		return (1);
	}

	return (0);
}

static int _check_inputs(tx_in_t const *in, unsigned int idx, void *args[])
{
	llist_t *all_unspent = (llist_t *)args[0];
	transaction_t const *transaction = (transaction_t const *)args[1];
	uint32_t *amount = (uint32_t *)args[2];
	unspent_tx_out_t *ref;
	EC_KEY *key;

	ref = llist_find_node(all_unspent, (node_ident_t)&_is_unspent,
		(void *)in);
	if (!ref)
		return (1);

	key = ec_from_pub(ref->out.pub);
	if (!ec_verify(key, transaction->id, SHA256_DIGEST_LENGTH, &in->sig))
	{
		EC_KEY_free(key);
		return (1);
	}

	*amount += ref->out.amount;

	EC_KEY_free(key);

	(void)idx;
	return (0);
}

static int _outputs_amount(tx_out_t const *out, unsigned int idx, void *args)
{
	uint32_t *amount = (uint32_t *)args;

	*amount += out->amount;

	(void)idx;
	return (0);
}

int transaction_is_valid(transaction_t const *transaction,
	llist_t *all_unspent)
{
	uint8_t id[SHA256_DIGEST_LENGTH];
	uint32_t inputs_amount = 0;
	uint32_t outputs_amount = 0;
	void *args[3] = {NULL, NULL, NULL};

	if (!transaction)
		return (0);

	transaction_hash(transaction, id);
	if (memcmp(transaction->id, id, sizeof(id)) != 0)
		return (0);

	args[0] = (void *)all_unspent;
	args[1] = (void *)transaction;
	args[2] = (void *)&inputs_amount;
	if (llist_for_each(transaction->inputs,
		(node_func_t)&_check_inputs, args) != 0)
	{
		return (0);
	}

	llist_for_each(transaction->outputs, (node_func_t)&_outputs_amount,
		(void *)&outputs_amount);
	if (inputs_amount != outputs_amount)
		return (0);

	return (1);
}
