#include <stdlib.h>
#include <string.h>

#include "transaction.h"

static int _find_inputs(unspent_tx_out_t const *unspent, unsigned int idx,
	void *args[])
{
	EC_KEY const *sender = (EC_KEY const *)args[0];
	uint32_t target_amount = *((uint32_t *)args[1]);
	uint32_t *total_amount = (uint32_t *)args[2];
	llist_t *inputs = (llist_t *)args[3];
	uint8_t pub[EC_PUB_LEN];

	ec_to_pub(sender, pub);
	if (memcmp(unspent->out.pub, pub, EC_PUB_LEN) == 0)
	{
		tx_in_t *in;

		in = tx_in_create(unspent);
		llist_add_node(inputs, in, ADD_NODE_REAR);

		*total_amount += unspent->out.amount;
		if (*total_amount >= target_amount)
			return (1);
	}

	(void)idx;
	return (0);
}

static llist_t *_init_outputs(EC_KEY const *sender, EC_KEY const *receiver,
	uint32_t amount, uint32_t total_amount)
{
	llist_t *outputs;
	uint8_t pub[EC_PUB_LEN];
	tx_out_t *out;

	outputs = llist_create(MT_SUPPORT_FALSE);

	out = tx_out_create(amount, ec_to_pub(receiver, pub));
	llist_add_node(outputs, out, ADD_NODE_REAR);

	if (total_amount > amount)
	{
		out = tx_out_create(total_amount - amount,
			ec_to_pub(sender, pub));
		llist_add_node(outputs, out, ADD_NODE_REAR);
	}

	return (outputs);
}

static int _sign_inputs(tx_in_t *in, unsigned int idx, void *args[])
{
	transaction_t const *transaction = (transaction_t const *)args[0];
	EC_KEY const *sender = (EC_KEY const *)args[1];
	llist_t *all_unspent = (llist_t *)args[2];

	tx_in_sign(in, transaction->id, sender, all_unspent);

	(void)idx;
	return (0);
}

transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
	uint32_t amount, llist_t *all_unspent)
{
	transaction_t *transaction;
	uint32_t total_amount = 0;
	void *args[4] = {NULL, NULL, NULL, NULL};

	/* Init transaction structure */
	transaction = malloc(sizeof(*transaction));
	if (!transaction)
		return (NULL);
	memset(transaction, 0, sizeof(*transaction));
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);

	/* Init transaction inputs */
	args[0] = (void *)sender;
	args[1] = (void *)&amount;
	args[2] = (void *)&total_amount;
	args[3] = (void *)transaction->inputs;
	llist_for_each(all_unspent, (node_func_t)&_find_inputs, args);

	if (total_amount < amount)
	{
		llist_destroy(transaction->inputs, 1, free);
		free(transaction);
		return (NULL);
	}

	/* Init transaction outputs */
	transaction->outputs = _init_outputs(sender, receiver,
		amount, total_amount);

	/* Compute transaction ID */
	transaction_hash(transaction, transaction->id);

	/* Sign transaction inputs */
	args[0] = (void *)transaction;
	args[1] = (void *)sender;
	args[2] = (void *)all_unspent;
	llist_for_each(transaction->inputs, (node_func_t)&_sign_inputs, args);

	return (transaction);
}
