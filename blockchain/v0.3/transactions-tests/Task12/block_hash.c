#include <stdlib.h>
#include <string.h>

#include "blockchain.h"

static int _transaction_id_buf(transaction_t const *transaction,
	unsigned int idx, int8_t *buffer)
{
	memcpy(buffer + idx * SHA256_DIGEST_LENGTH,
		transaction->id, SHA256_DIGEST_LENGTH);
	return (0);
}

/**
 * block_hash_nonce - Hash a Block, excepts its 'nonce', which is given as
 * a parameter
 *
 * @block:    Pointer to the Block to hash
 * @hash_buf: Pointer to a buffer in which the hash will be stored
 * @nonce:    Nonce to be put in the hash, in lieu of the Block's nonce
 *
 * Return: A 32-bytes hash of the Block
 */
uint8_t *block_hash_nonce(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH], uint64_t nonce)
{
	block_t b;
	int8_t *buffer;
	size_t buf_len;

	memcpy(&b, block, sizeof(b));
	b.info.nonce = nonce;

	buf_len = sizeof(block->info) +
		block->data.len +
		llist_size(block->transactions) * SHA256_DIGEST_LENGTH;
	buffer = malloc(buf_len);

	memcpy(buffer, block, sizeof(block->info) + block->data.len);
	llist_for_each(block->transactions,
		(node_func_t)_transaction_id_buf,
		buffer + sizeof(block->info) + block->data.len);

	sha256(buffer, buf_len, hash_buf);

	return ((uint8_t *)hash_buf);
}

/**
 * block_hash - Hash a block
 * This function calls the function 'block_hash_nonce', with the Block's nonce
 * as the second parameter. Refer to this function for details.
 *
 *
 * @block:    Pointer to the Block to hash
 * @hash_buf: Pointer to a buffer in which the hash will be stored
 *
 * Return: A 32-bytes hash of the Block
 */
uint8_t *block_hash(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int8_t *buffer;
	size_t buf_len;
	int list_size;

	list_size = llist_size(block->transactions);
	list_size = (list_size < 0 ? 0 : list_size);
	buf_len = sizeof(block->info) +
		block->data.len +
		list_size * SHA256_DIGEST_LENGTH;
	buffer = malloc(buf_len);

	memcpy(buffer, block, sizeof(block->info) + block->data.len);
	if (list_size > 0)
		llist_for_each(block->transactions,
			(node_func_t)_transaction_id_buf,
			buffer + sizeof(block->info) + block->data.len);

	sha256(buffer, buf_len, hash_buf);

	return ((uint8_t *)hash_buf);
}
