#include "blockchain.h"

/**
 * input_matches_unspent - checks whether an input matches any of the unspent
 * @in: individual input
 * @un: individual unspent transaction
 * @block_hash: hash of the validated block that contains the
 *  transaction list transactions
 *
 * Return: 1 if matches, 0 if not
 */
int input_matches_unspent(tx_in_t *in, unspent_tx_out_t *un,
	uint8_t block_hash[SHA256_DIGEST_LENGTH])
{
	if (memcmp(in->block_hash, block_hash, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	if (memcmp(in->tx_id, un->tx_id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	if (memcmp(in->tx_out_hash, un->out.hash, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	return (1);
}

/**
 * update_unspent - updates the list of all unspent transaction outputs,
 *  given a list of processed transactions
 * @transactions: list of validated transactions
 * @block_hash: hash of the validated block that contains the
 *  transaction list transactions
 * @all_unspent: current list of unspent transaction outputs
 *
 * Return: new list of unspent transaction outputs
 */
llist_t *update_unspent(
	llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	llist_t *all_unspent)
{
	int i, j, k, txs_size, in_size, out_size, unspent_size, un_found;
	llist_t *new_unspent_txns = NULL;
	transaction_t *tx = NULL;
	tx_in_t *in = NULL;
	tx_out_t *out = NULL;
	unspent_tx_out_t *un = NULL;

	if (!transactions || !block_hash || !all_unspent)
		return (NULL);
	txs_size = llist_size(transactions);
	unspent_size = llist_size(all_unspent);
	new_unspent_txns = llist_create(MT_SUPPORT_FALSE);
	for (i = 0; i < txs_size; i++) /* all_unspent compared vs each input */
	{
		un = llist_get_node_at(all_unspent, i);
		un_found = 0;
		for (j = 0; j < txs_size && un_found == 0; j++) /* all transactions */
		{
			tx = llist_get_node_at(transactions, j);
			in_size = llist_size(tx->inputs);
			for (k = 0; k < in_size && un_found == 0; k++) /* inputs of tx */
			{
				in = llist_get_node_at(tx->inputs, k);
				if (input_matches_unspent(in, block_hash, all_unspent))
					un_found = 1;
			}
		}
		if (!un_found)
			llist_add_node(new_unspent_txns, un, ADD_NODE_REAR);
	}
	for (i = 0; i < txs_size; i++) /* add txns outputs to new_unspent_txns */
	{
		tx = llist_get_node_at(transactions, i);
		out_size = llist_size(tx->outputs);
		for (j = 0; j < out_size; j++)
		{
			out = llist_get_node_at(tx->outputs, j);
			un = unspent_tx_out_create(block_hash, tx->id, out);
			llist_add_node(new_unspent_txns, un, ADD_NODE_REAR);
		}
	}
	return (new_unspent_txns);
}
