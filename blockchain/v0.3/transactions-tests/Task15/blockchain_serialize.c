#include <fcntl.h>
#include <unistd.h>

#include "blockchain.h"
#include "endianness.h"

static int _tx_in_serialize(tx_in_t const *in,
	unsigned int idx, int const *ptr_fd)
{
	int fd = *ptr_fd;

	write(fd, in->block_hash, sizeof(in->block_hash));
	write(fd, in->tx_id, sizeof(in->tx_id));
	write(fd, in->tx_out_hash, sizeof(in->tx_out_hash));
	write(fd, &in->sig, sizeof(in->sig));

	(void)idx;
	return (0);
}

static int _tx_out_serialize(tx_out_t const *out,
	unsigned int idx, int const *ptr_fd)
{
	int fd = *ptr_fd;

	write(fd, &out->amount, 4);
	write(fd, out->pub, EC_PUB_LEN);
	write(fd, out->hash, sizeof(out->hash));

	(void)idx;
	return (0);
}

static int _transaction_serialize(transaction_t const *transaction,
	unsigned int idx, int const *ptr_fd)
{
	int fd = *ptr_fd;
	uint32_t nb_inputs = llist_size(transaction->inputs);
	uint32_t nb_outputs = llist_size(transaction->outputs);

	write(fd, transaction->id, sizeof(transaction->id));
	write(fd, &nb_inputs, 4);
	write(fd, &nb_outputs, 4);

	llist_for_each(transaction->inputs,
		(node_func_t)_tx_in_serialize, (void *)ptr_fd);
	llist_for_each(transaction->outputs,
		(node_func_t)_tx_out_serialize, (void *)ptr_fd);

	(void)idx;
	return (0);
}

/**
 * _block_serialize - Writes a block into a stream given its file descriptor
 *
 * @block:  Pointer to the Block to be written
 * @ptr_fd: Pointer to the stream file descriptor
 */
static int _block_serialize(block_t const *block, unsigned int idx,
	int const *ptr_fd)
{
	int fd = *ptr_fd;
	uint32_t nb_transactions = llist_size(block->transactions);

	write(fd, &block->info, sizeof(block->info));
	write(fd, &block->data.len, 4);
	write(fd, block->data.buffer, block->data.len);
	write(fd, block->hash, sizeof(block->hash));
	write(fd, &nb_transactions, 4);

	llist_for_each(block->transactions,
		(node_func_t)_transaction_serialize, (void *)ptr_fd);

	(void)idx;
	return (0);
}

static int _unspent_serialize(unspent_tx_out_t const *unspent,
	unsigned int idx, int const *ptr_fd)
{
	int fd = *ptr_fd;

	write(fd, unspent->block_hash, SHA256_DIGEST_LENGTH);
	write(fd, unspent->tx_id, SHA256_DIGEST_LENGTH);
	write(fd, &unspent->out.amount, 4);
	write(fd, unspent->out.pub, EC_PUB_LEN);
	write(fd, unspent->out.hash, SHA256_DIGEST_LENGTH);

	(void)idx;
	return (0);
}

/**
 * _blockchain_to_file - Converts a Blockchain onto a serialized file
 *
 * @fd:         File descriptor to write into
 * @blockchain: Pointer to the Blockchain to be written
 *
 * Return: 0 upon success. Error code upon failure
 */
static int _blockchain_to_fd(int fd, blockchain_t const *blockchain)
{
	uint32_t nb_blocks = llist_size(blockchain->chain);
	uint32_t nb_unspent = llist_size(blockchain->unspent);
	hblk64_hdr_t header = {
		BLOCKCHAIN_MAGIC,
		BLOCKCHAIN_VERSION,
		1
	};

	/* HEADER */
	header.endianness = _get_endianness();
	write(fd, &header, sizeof(header));
	write(fd, &nb_blocks, 4);
	write(fd, &nb_unspent, 4);

	llist_for_each(blockchain->chain,
		(node_func_t)_block_serialize, &fd);

	llist_for_each(blockchain->unspent,
		(node_func_t)_unspent_serialize, &fd);

	return (0);
}

int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	int fd;
	int ret;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);

	ret = _blockchain_to_fd(fd, blockchain);

	close(fd);
	return (ret);
}
