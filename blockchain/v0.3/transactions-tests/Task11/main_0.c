/* Description: Create a Blockchain and add a few Blocks to it, then destroy it */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

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

	blockchain = blockchain_create();
	block = llist_get_head(blockchain->chain);

	block = block_create(block, (int8_t *)"Holberton", 9);
	block->info.timestamp = 1234567890;
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	block = block_create(block, (int8_t *)"School", 6);
	block->info.timestamp = 1234567890;
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	block = block_create(block, (int8_t *)"of", 2);
	block->info.timestamp = 1234567890;
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	block = block_create(block, (int8_t *)"Software", 8);
	block->info.timestamp = 1234567890;
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	block = block_create(block, (int8_t *)"Engineering", 11);
	block->info.timestamp = 1234567890;
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

	_blockchain_print_brief(blockchain);
	printf("Unspent list size: %d\n", llist_size(blockchain->unspent));
	blockchain_destroy(blockchain);

	return (EXIT_SUCCESS);
}
