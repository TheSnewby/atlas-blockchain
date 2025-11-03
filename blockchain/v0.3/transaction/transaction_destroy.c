#include "blockchain.h"

/**
 * transaction_destroy - deallocates a transaction structure
 * @transaction: transaction to delete
 */
void transaction_destroy(transaction_t *transaction)
{
	int i, inputs_size, outputs_size;
	tx_in_t *in = NULL;
	tx_out_t *out = NULL;

	if (!transaction)
		return;

	inputs_size = llist_size(transaction->inputs);
	outputs_size = llist_size(transaction->outputs);

	for (i = 0; i < inputs_size; i++)
	{
		in = llist_get_node_at(transaction->inputs, i);
		free(in);
		in = NULL;
	}
	for (i = 0; i < outputs_size; i++)
	{
		out = llist_get_node_at(transaction->outputs, i);
		free(out);
		out = NULL;
	}

	llist_destroy(transaction->inputs, 0, NULL);
	llist_destroy(transaction->outputs, 0, NULL);
	free(transaction);
}
