#include "blockchain.h"

/**
 * inputs_and_outputs_generation - generates the llist of inputs & outputs
 * @tx: transaction
 * @inputs: inputs llist for tx
 * @outputs: outputs llist for tx
 * @all_unspent: all unspent transactions
 * @sender: contains the private key of the transaction sender
 * @receiver: contains the public key of the transaction receiver
 * @amount: amount to send
 *
 * Return: 1 on success, 0 on failure
 */
int inputs_and_outputs_generation(
	transaction_t *tx,
	llist_t *inputs,
	llist_t *outputs,
	llist_t *all_unspent,
	EC_KEY const *sender,
	EC_KEY const *receiver,
	uint32_t amount)
{
	uint32_t sender_bank = 0;
	uint8_t sender_pub[EC_PUB_LEN], receiver_pub[EC_PUB_LEN];
	tx_out_t *tx_out_pay = NULL, *tx_out_rtn = NULL;
	tx_in_t *tx_in = NULL;
	unspent_tx_out_t *unspent_tx = NULL;
	int i, all_unspent_size;

	all_unspent_size = llist_size(all_unspent);
	fprintf(stderr, "llist_size: %d\n", all_unspent_size);
	ec_to_pub(sender, sender_pub);
	ec_to_pub(receiver, receiver_pub);

	for (i = 0; i < all_unspent_size; i++)
	{
		unspent_tx = (unspent_tx_out_t *)llist_get_node_at(all_unspent, i);
		if (memcmp(unspent_tx->out.pub, sender_pub, EC_PUB_LEN) == 0)
		{
			tx_in = tx_in_create(unspent_tx);
			llist_add_node(inputs, tx_in, ADD_NODE_REAR);
			sender_bank += unspent_tx->out.amount;
		}
	}

	if (amount > sender_bank)
	{
		fprintf(stderr, "Not enough money.\n");
		return (0);
	}

	tx_out_pay = tx_out_create(amount, receiver_pub);
	llist_add_node(outputs, tx_out_pay, ADD_NODE_REAR);
	if (sender_bank > amount)
	{
		tx_out_rtn = tx_out_create(sender_bank - amount, sender_pub);
		llist_add_node(outputs, tx_out_rtn, ADD_NODE_REAR);
	}

	transaction_hash(tx, tx->id);

	for (i = 0; i < all_unspent_size; i++) /* sign inputs */
		tx_in_sign((tx_in_t *)llist_get_node_at(inputs, i), tx->id,
		sender, all_unspent);

	return (1);
}

/**
 * transaction_create - creates a transaction
 * @sender: contains the private key of the transaction sender
 * @receiver: contains the public key of the transaction receiver
 * @amount: amount to send
 * @all_unspent: list of all the unspent outputs to date
 *
 * Return: created transaction, NULL on failure
 */
transaction_t *transaction_create(
	EC_KEY const *sender,
	EC_KEY const *receiver,
	uint32_t amount,
	llist_t *all_unspent)
{
	transaction_t *tx = NULL;
	llist_t *inputs = NULL;
	llist_t *outputs = NULL;

	if (!sender || !receiver || !amount || !all_unspent)
	{
		fprintf(stderr, "!param\n");
		return (NULL);
	}

	tx = (transaction_t *)malloc(sizeof(transaction_t));
	if (!tx)
		return (NULL);

	inputs = llist_create(MT_SUPPORT_TRUE);
	outputs = llist_create(MT_SUPPORT_TRUE);
	if (!inputs || !outputs)
		return (NULL);

	tx->inputs = inputs;
	tx->outputs = outputs;

	if (!inputs_and_outputs_generation(tx, inputs, outputs,
		all_unspent, sender, receiver, amount))
		return (NULL);

	return (tx);
}
