#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "blockchain.h"
#include "endianness.h"

static tx_out_t *_tx_out_deserialize(int fd, int change_endianness)
{
	tx_out_t *out;

	out = malloc(sizeof(*out));
	memset(out, 0, sizeof(*out));

	read(fd, &out->amount, 4);
	if (change_endianness)
		SWAPENDIAN(out->amount);
	read(fd, out->pub, 65);
	read(fd, out->hash, 32);

	return (out);
}

static tx_in_t *_tx_in_deserialize(int fd, int change_endianness)
{
	tx_in_t *in;

	in = malloc(sizeof(*in));
	memset(in, 0, sizeof(*in));

	read(fd, in->block_hash, 32);
	read(fd, in->tx_id, 32);
	read(fd, in->tx_out_hash, 32);
	read(fd, in->sig.sig, 72);
	read(fd, &in->sig.len, 1);

	(void)change_endianness;
	return (in);
}

static transaction_t *_transaction_deserialize(int fd, int change_endianness)
{
	transaction_t *transaction;
	int32_t nb_inputs;
	int32_t nb_outputs;

	transaction = malloc(sizeof(*transaction));
	memset(transaction, 0, sizeof(*transaction));

	read(fd, transaction->id, sizeof(transaction->id));

	read(fd, &nb_inputs, 4);
	if (change_endianness)
		SWAPENDIAN(nb_inputs);
	transaction->inputs = NULL;
	if (nb_inputs > -1)
		transaction->inputs = llist_create(MT_SUPPORT_FALSE);

	read(fd, &nb_outputs, 4);
	if (change_endianness)
		SWAPENDIAN(nb_outputs);
	transaction->outputs = NULL;
	if (nb_outputs > -1)
		transaction->outputs = llist_create(MT_SUPPORT_FALSE);

	while (nb_inputs-- > 0)
	{
		tx_in_t *in;

		in = _tx_in_deserialize(fd, change_endianness);
		if (in)
			llist_add_node(transaction->inputs, in, ADD_NODE_REAR);
	}
	while (nb_outputs-- > 0)
	{
		tx_out_t *out;

		out = _tx_out_deserialize(fd, change_endianness);
		if (out)
			llist_add_node(transaction->outputs, out, ADD_NODE_REAR);
	}

	return (transaction);
}

/**
 * block_deserialize - Read a Block from a stream given its file descriptor
 *
 * @fd:                File descriptor of the stream to read the Block from
 * @change_endianness: Flag set to 1 if endianness needs to be changed for
 *                     multi-byte fields
 *
 * Return: 0 upon succes. Error code upon failure
 */
static block_t *_block_deserialize(int fd, int change_endianness)
{
	block_t *block;
	int32_t nb_transactions;

	block = malloc(sizeof(*block));
	memset(block, 0, sizeof(*block));

	read(fd, &block->info, sizeof(block->info));
	if (change_endianness)
	{
		SWAPENDIAN(block->info.index);
		SWAPENDIAN(block->info.difficulty);
		SWAPENDIAN(block->info.timestamp);
		SWAPENDIAN(block->info.nonce);
	}

	read(fd, &block->data.len, 4);
	if (change_endianness)
		SWAPENDIAN(block->data.len);
	read(fd, block->data.buffer, block->data.len);

	read(fd, block->hash, sizeof(block->hash));

	read(fd, &nb_transactions, 4);
	if (change_endianness)
		SWAPENDIAN(nb_transactions);
	block->transactions = NULL;
	if (nb_transactions > -1)
		block->transactions = llist_create(MT_SUPPORT_FALSE);

	while (nb_transactions-- > 0)
	{
		transaction_t *transaction;

		transaction = _transaction_deserialize(fd, change_endianness);
		if (transaction)
			llist_add_node(block->transactions, transaction, ADD_NODE_REAR);
	}

	return (block);
}

static unspent_tx_out_t *_unspent_deserialize(int fd, int change_endianness)
{
	unspent_tx_out_t *unspent;

	unspent = malloc(sizeof(*unspent));
	memset(unspent, 0, sizeof(*unspent));

	read(fd, unspent->block_hash, SHA256_DIGEST_LENGTH);
	read(fd, unspent->tx_id, SHA256_DIGEST_LENGTH);

	read(fd, &unspent->out.amount, 4);
	if (change_endianness)
		SWAPENDIAN(unspent->out.amount);

	read(fd, unspent->out.pub, EC_PUB_LEN);
	read(fd, unspent->out.hash, SHA256_DIGEST_LENGTH);

	return (unspent);
}

/**
 * _blockchain_from_file - Converts a serialized file to a Blockchain
 *
 * @fd:         File descriptor to read from
 *
 * Return: 0 upon success. Error code upon failure
 */
static blockchain_t *_blockchain_from_fd(int fd)
{
	blockchain_t *blockchain;
	hblk64_hdr_t header;
	int32_t nb_blocks;
	int32_t nb_unspent;
	int change_endianness = 0;

	read(fd, &header, sizeof(header));
	if (strncmp((char *)header.magic, BLOCKCHAIN_MAGIC, 4) != 0)
		return (NULL);
	if (strncmp((char *)header.version, BLOCKCHAIN_VERSION, 3) != 0)
		return (NULL);
	if (header.endianness != _get_endianness())
		change_endianness = 1;

	read(fd, &nb_blocks, sizeof(nb_blocks));
	if (change_endianness)
		SWAPENDIAN(nb_blocks);
	read(fd, &nb_unspent, sizeof(nb_unspent));
	if (change_endianness)
		SWAPENDIAN(nb_unspent);

	blockchain = malloc(sizeof(*blockchain));
	blockchain->chain = NULL;
	if (nb_blocks > -1)
		blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (nb_unspent > -1)
		blockchain->unspent = llist_create(MT_SUPPORT_FALSE);

	while (nb_blocks-- > 0)
	{
		block_t *block;

		block = _block_deserialize(fd, change_endianness);
		if (block)
			llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	while (nb_unspent-- > 0)
	{
		unspent_tx_out_t *unspent;

		unspent = _unspent_deserialize(fd, change_endianness);
		if (unspent)
			llist_add_node(blockchain->unspent, unspent, ADD_NODE_REAR);
	}

	return (blockchain);
}

blockchain_t *blockchain_deserialize(char const *path)
{
	blockchain_t *blockchain;
	int fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "Can't open file: %s\n", path);
		return (NULL);
	}

	blockchain = _blockchain_from_fd(fd);

	close(fd);
	return (blockchain);
}
