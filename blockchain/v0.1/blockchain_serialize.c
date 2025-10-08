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
 * writeBlk - writes a Block to memory
 * @blk: individual block to print
 * @fd: file descriptor to fwrite to
 * Return: 0 on success
 */
int writeBlk(block_t *blk, FILE *fd)
{
	fwrite(&blk->info.index, sizeof(int), 1, fd);
	fwrite(&blk->info.difficulty, sizeof(int), 1, fd);
	fwrite(&blk->info.timestamp, sizeof(uint64_t), 1, fd);
	fwrite(&blk->info.nonce, sizeof(uint64_t), 1, fd);
	fwrite(&blk->info.prev_hash, SHA256_DIGEST_LENGTH, 1, fd);
	fwrite(&blk->data.len, sizeof(uint32_t), 1, fd);
	fwrite(&blk->data.buffer, blk->data.len, 1, fd);
	fwrite(&blk->hash, SHA256_DIGEST_LENGTH, 1, fd);

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
	int isBigEnd = isBigEndian() + 1, hblk_blocks = 0;

	if (!blockchain || !path)
		return (-1);

	hblk_blocks = llist_size(blockchain->chain);
	fd = fopen(path, "wb");
	if (!fd)
		return (-1);

	/* HEADER */
	fwrite("HBLK", 1, 4, fd); /* hblk magic */
	fwrite("1.0", 1, 3, fd); /* hblk version */
	fwrite(&isBigEnd, 1, 1, fd); /* endian */
	fwrite(&hblk_blocks, sizeof(int), 1, fd); /* number of blocks */

	/* BLOCKS */
	llist_for_each(blockchain->chain, writeBlk, fd);
	fclose(fd);

	return (0);
}


