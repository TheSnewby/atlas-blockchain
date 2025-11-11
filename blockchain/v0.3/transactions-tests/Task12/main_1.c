/* Description: Create 2 Blocks and insert a coinbase transaction in each Block */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

EC_KEY *_ec_load(char const *folder);
void _blockchain_print_brief(blockchain_t const *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	blockchain_t *blockchain;
	block_t *block;
	EC_KEY *owner;

	owner = _ec_load("_keys/alex");
	blockchain = blockchain_create();
	block = llist_get_head(blockchain->chain);

	block = block_create(block, (int8_t *)"Holberton School", 16);
	block->info.timestamp = 1234567890;
	llist_add_node(block->transactions,
		coinbase_create(owner, block->info.index),
		ADD_NODE_REAR);
	block_hash(block, block->hash);
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	block = block_create(block, (int8_t *)"Software", 8);
	block->info.timestamp = 1234567890;
	llist_add_node(block->transactions,
		coinbase_create(owner, block->info.index),
		ADD_NODE_REAR);
	block_hash(block, block->hash);
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

	_blockchain_print_brief(blockchain);

	blockchain_destroy(blockchain);
	EC_KEY_free(owner);
	return (EXIT_SUCCESS);
}
