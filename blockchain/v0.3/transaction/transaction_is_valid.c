#include "blockchain.h"

/**
 * transaction_is_valid - checks whether a transaction is valid
 * @transaction: transaction to verify
 * @all_unspent: is the list of all unspent transaction outputs to date
 *
 * Return: 1 if the transaction is valid, 0 otherwise
 */
int transaction_is_valid(transaction_t const *transaction, llist_t *all_unspent)
{
	int i, j, inputs_size, outputs_size, all_unspent_size, found;
	uint8_t test_hash[SHA256_DIGEST_LENGTH] = {0};
	tx_in_t *input = NULL;
	unspent_tx_out_t *unspent = NULL;
	int input_size = SHA256_DIGEST_LENGTH * 3;
	uint8_t input_buffer[input_size];

	if (!transaction || !all_unspent)
	{
		fprintf(stderr, "!params\n");
		return (0);
	}

	transaction_hash(transaction, test_hash); /* transaction hash verification */
	if (memcmp(test_hash, transaction->id, SHA256_DIGEST_LENGTH) != 0)
	{
		fprintf(stderr, "!transaction hash\n");
		return (0);
	}

	inputs_size = llist_size(transaction->inputs);
	outputs_size = llist_size(transaction->outputs);
	all_unspent_size = llist_size(all_unspent);

	if (inputs_size != outputs_size)
	{
		fprintf(stderr, "!sizes\n");
		return (0);
	}

	/* O(n^2) doesn't matter with small sizes */
	for (i = 0; i < inputs_size; i++) /* input verification with unspent */
	{
		input = (tx_in_t *)llist_get_node_at(transaction->inputs, i);
		found = 0;
		for (j = 0; j < all_unspent_size; j++)
		{
			unspent = (unspent_tx_out_t *)llist_get_node_at(all_unspent, j);
			if (memcmp(input->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH) == 0)
			{
				found = 1;
				/* verify sig of input using matching unspent's public key */
				memcpy(input_buffer, input->block_hash, SHA256_DIGEST_LENGTH);
				memcpy(input_buffer + SHA256_DIGEST_LENGTH, input->tx_id, SHA256_DIGEST_LENGTH);
				memcpy(input_buffer + SHA256_DIGEST_LENGTH * 2, input->tx_out_hash, SHA256_DIGEST_LENGTH);
				if (!ec_verify(ec_from_pub(unspent->out.pub), input_buffer,
				input_size, &input->sig))
				{
					fprintf(stderr, "!ec_verify\n");
					return (0);
				}
				break;
			}
		}
		if (!found)
		{
			fprintf(stderr, "!found\n");
			return (0);
		}
	}

	return (1);
}
