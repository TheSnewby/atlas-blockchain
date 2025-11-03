#include "blockchain.h"

/**
 * inputs_and_outputs_generation - generates the llist of inputs & outputs
 * @tx: transaction
 * @receiver: contains the public key of the transaction receiver
 * @block_index: index of the Block the coinbase trasnaction will belong to
 * @amount: amount to send
 *
 * Return: 1 on success, 0 on failure
 */
static int inputs_and_outputs_generation(transaction_t *tx, EC_KEY const *receiver,
	uint32_t block_index, uint32_t amount)
{
	uint8_t receiver_pub[EC_PUB_LEN];
	tx_in_t *tx_in = NULL;
	tx_out_t *tx_out = NULL;

	ec_to_pub(receiver, receiver_pub);
	tx_in = (tx_in_t *)malloc(sizeof(tx_in_t));
	tx_out = tx_out_create(amount, receiver_pub);
	if (!tx_in || !tx_out)
		return (0);

	memset(tx_in->block_hash, 0, SHA256_DIGEST_LENGTH);
	memset(tx_in->tx_id, 0, SHA256_DIGEST_LENGTH);
	memset(tx_in->tx_out_hash, 0, SHA256_DIGEST_LENGTH);
	tx_in->tx_out_hash[0] = block_index;
	memset(tx_in->sig.sig, 0, SIG_MAX_LEN);
	tx_in->sig.len = 0;

	llist_add_node(tx->inputs, tx_in, ADD_NODE_REAR);
	llist_add_node(tx->outputs, tx_out, ADD_NODE_REAR);

	return (1);
}

/**
 * coinbase_create - creates a coinbase transaction
 * @receiver: public key of the miner, who will receive the coinbase coins
 * @block_index: index of the Block the coinbase transaction will belong to
 *
 * Return: created transaction, NULL on failure
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *tx = NULL;
	llist_t *inputs = NULL;
	llist_t *outputs = NULL;

	if (!receiver)
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

	if (!inputs_and_outputs_generation(tx, receiver, block_index,
		COINBASE_AMOUNT))
		return (NULL);

	transaction_hash(tx, tx->id);

	return (tx);
}
