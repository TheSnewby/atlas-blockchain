#include "blockchain.h"

/**
 * isBigEndian - finds whether the format is big or small endian
 *
 * Return: 0 on little and 1 on big
 */
int isBigEndian(void)
{
	int i = 1;

	return (!*((char *)&i));
}

/**
 * swap_endian32 - swaps value to little endian
 * @val: value to be swapped
 *
 * Return: little endian value
 */
uint32_t swap_endian32(uint32_t val)
{
	return (((val >> 24) & 0xFF) |
	((val >> 8) & 0xFF00) |
	((val << 8) & 0xFF0000) |
	((val << 24) & 0xFF000000));
}

/**
 * writeUnspentBlk- writes a Block to memory
 * @blk: individual block to print
 * @a: unused variable required by llist_for_each()
 * @fd: file descriptor to fwrite to
 * Return: 0 on success
 */
int writeUnspentBlk(void *blk, unsigned int a, void *fd)
{
	unspent_tx_out_t *un = (unspent_tx_out_t *)blk;
	FILE *f = (FILE *)fd;
	(void)a;

	/* rewrite for unspent */
	fwrite(&un->block_hash, SHA256_DIGEST_LENGTH, 1, f);
	fwrite(&un->tx_id, SHA256_DIGEST_LENGTH, 1, f);
	fwrite(&un->out.amount, 1, 1, f);
	fwrite(&un->out.pub, EC_PUB_LEN, 1, f);
	fwrite(&un->out.hash, SHA256_DIGEST_LENGTH, 1, f);


	return (0);
}

/**
 * writeBlk - writes a Block to memory
 * @blk: individual block to print
 * @a: unused variable required by llist_for_each()
 * @fd: file descriptor to fwrite to
 * Return: 0 on success
 */
int writeBlk(void *blk, unsigned int a, void *fd)
{
	block_t *block = (block_t *)blk;
	FILE *f = (FILE *)fd;
	int txs_size, i, j, isBigEnd = isBigEndian() + 1, in_size, out_size, temp;
	transaction_t *tx = NULL;
	(void)a;
	tx_in_t *in = NULL;
	tx_out_t *out = NULL;

	txs_size = llist_size(block->transactions);

	fwrite(&block->info.index, sizeof(int), 1, f);
	fwrite(&block->info.difficulty, sizeof(int), 1, f);
	fwrite(&block->info.timestamp, sizeof(uint64_t), 1, f);
	fwrite(&block->info.nonce, sizeof(uint64_t), 1, f);
	fwrite(&block->info.prev_hash, SHA256_DIGEST_LENGTH, 1, f);
	fwrite(&block->data.len, sizeof(uint32_t), 1, f);
	fwrite(&block->data.buffer, block->data.len, 1, f);
	fwrite(&block->hash, SHA256_DIGEST_LENGTH, 1, f);
	fwrite(&txs_size, sizeof(int), 1, f);

	for (i = 0; i < txs_size; i++) /* transactions */
	{
		tx = llist_get_node_at(block->transactions, i);
		fwrite(&tx->id, SHA256_DIGEST_LENGTH, 1, f);
		in_size = llist_size(tx->inputs);
		out_size = llist_size(tx->outputs);
		if (isBigEnd)
		{
			fwrite(&in_size, sizeof(int), 1, f);
			fwrite(&out_size, sizeof(int), 1, f);
		}
		else
		{
			temp = swap_endian32(in_size);
			fwrite(&temp, sizeof(int), 1, f);
			temp = swap_endian32(out_size);
			fwrite(&temp, sizeof(int), 1, f);
		}

		for (j = 0; j < in_size; j++) /* inputs */
		{
			in = llist_get_node_at(tx->inputs, j);
			fwrite(&in->block_hash, SHA256_DIGEST_LENGTH, 1, f);
			fwrite(&in->tx_id, SHA256_DIGEST_LENGTH, 1, f);
			fwrite(&in->tx_out_hash, SHA256_DIGEST_LENGTH, 1, f);
			fwrite(&in->sig.sig, SIG_MAX_LEN, 1, f);
			fwrite(&in->sig.len, 1, 1, f);
		}

		for (j = 0; j < out_size; j++)
		{
			out = llist_get_node_at(tx->outputs, j);
			fwrite(&out->amount, sizeof(int), 1, f);
			fwrite(&out->pub, EC_PUB_LEN, 1, f);
			fwrite(&out->hash, SHA256_DIGEST_LENGTH, 1, f);
		}
	}

	return (0);
}

/**
 * blockchain_serialize - serializes a Blockchain into a file
 * @blockchain: blockchain to be serialized
 * @path: filepath
 *
 * Return: 0 upon success, -1 upon failure
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *fd = NULL;
	int isBigEnd = isBigEndian() + 1, hblk_blocks = 0, un_hblk_blocks = 0;


	if (!blockchain || !path)
		return (-1);

	hblk_blocks = llist_size(blockchain->chain);
	un_hblk_blocks = llist_size(blockchain->unspent);
	fd = fopen(path, "wb");
	if (!fd)
		return (-1);

	/* HEADER */
	fwrite("HBLK", 1, 4, fd); /* hblk magic */
	fwrite("0.3", 1, 3, fd); /* hblk version */
	fwrite(&isBigEnd, 1, 1, fd); /* endian */
	fwrite(&hblk_blocks, sizeof(int), 1, fd); /* number of blocks */
	fwrite(&un_hblk_blocks, sizeof(int), 1, fd); /* number of unspent blocks */

	/* BLOCKS */
	llist_for_each(blockchain->chain, writeBlk, fd);
	llist_for_each(blockchain->unspent, writeUnspentBlk, fd);
	fclose(fd);

	return (0);
}


