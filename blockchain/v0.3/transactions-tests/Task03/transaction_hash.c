#include <string.h>

#include "transaction.h"

/**
 * _inputs_hash - Concatenates transaction input content into a buffer
 *
 * Description: This function is called by `llist_for_each_arg`
 *
 * @out:     Pointer to the input structure
 * @idx:     Element index in the linked list
 * @content: Buffer to concatenate the information into
 *
 * Return: FOREACH_CONTINUE
 */
static int _inputs_hash(tx_in_t const *in, unsigned int idx, int8_t *content)
{
	content += idx * SHA256_DIGEST_LENGTH * 3;
	memcpy(content, in->block_hash, SHA256_DIGEST_LENGTH);
	content += SHA256_DIGEST_LENGTH;
	memcpy(content, in->tx_id, SHA256_DIGEST_LENGTH);
	content += SHA256_DIGEST_LENGTH;
	memcpy(content, in->tx_out_hash, SHA256_DIGEST_LENGTH);

	return (0);
}

/**
 * _outputs_hash - Concatenates transaction output content into a buffer
 *
 * Description: This function is called by `llist_for_each_arg`
 *
 * @out:     Pointer to the output structure
 * @idx:     Element index in the linked list
 * @content: Buffer to concatenate the information into
 *
 * Return: FOREACH_CONTINUE
 */
static int _outputs_hash(tx_out_t const *out, unsigned int idx,
	int8_t *content)
{
	content += idx * SHA256_DIGEST_LENGTH;
	memcpy(content, out->hash, SHA256_DIGEST_LENGTH);

	return (0);
}

/**
 * transaction_hash - Computes the ID of a given transaction.
 *
 * @tx: Pointer to the transaction to compute the ID of
 * @id: Buffer in which to store the computed hash. If equals to NULL,
 *      NULL is returned
 *
 * Return: A pointer to @id
 */
uint8_t *transaction_hash(transaction_t const *transaction,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int8_t *content;
	size_t inputs_len, outputs_len;


	inputs_len = llist_size(transaction->inputs) * SHA256_DIGEST_LENGTH * 3;
	outputs_len = llist_size(transaction->outputs) * SHA256_DIGEST_LENGTH;
	content = malloc((inputs_len + outputs_len) * sizeof(*content));

	llist_for_each(transaction->inputs,
		(node_func_t)_inputs_hash, content);
	llist_for_each(transaction->outputs,
		(node_func_t)_outputs_hash, content + inputs_len);

	sha256(content, inputs_len + outputs_len, hash_buf);
	free(content);

	return ((uint8_t *)hash_buf);
}
