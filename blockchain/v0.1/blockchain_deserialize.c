#include "blockchain.h"

/**
 * deserialize_blocks - deserializes the specific blocks
 * @rd: file contents
 * @numb: number of Blocks
 * @bchn: blockchain
 * Return: 1 on success
 */
int deserialize_blocks(void *rd, unsigned int numb, void *bchn)
{
	int offset = 12, i, numBlocks = (int)numb;
	block_t *block = NULL;
	blockchain_t *blockchain = (blockchain_t *)bchn;
	char *read = (char *)rd;

	/* BLOCKS */
	for (i = 0; i < numBlocks; i++)
	{
		block = (block_t *)malloc(sizeof(block_t));
		if (!block)
		{
			free(blockchain);
			return (0);
		}
		memcpy(&block->info.index, read + offset, sizeof(uint32_t));
		memcpy(&block->info.difficulty, read + offset + 4, sizeof(uint32_t));
		memcpy(&block->info.timestamp, read + offset + 8, sizeof(uint64_t));
		memcpy(&block->info.nonce, read + offset + 16, sizeof(uint64_t));
		memcpy(&block->info.prev_hash, read + offset + 24, SHA256_DIGEST_LENGTH);
		memcpy(&block->data.len, read + offset + 56, sizeof(uint32_t));
		memcpy(&block->data.buffer, read + offset + 60, block->data.len);
		memcpy(&block->hash, read + offset + block->data.len + 60,
			SHA256_DIGEST_LENGTH);

		offset += 60 + block->data.len + SHA256_DIGEST_LENGTH;
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	return (1);
}

/**
 * blockchain_deserialize - deserialized a Blockchain from a file
 * @path: file path to deserialize from
 * Return: pointer to deserialized Blockchain, NULL on failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	int numBlocks;
	FILE *fd;
	blockchain_t *blockchain = NULL;
	llist_t *chain = NULL;
	char read[FILEREAD] = {0};

	if (!path)
		return (NULL);
	fd = fopen(path, "rb");
	if (!fd)
		return (NULL);
	blockchain = (blockchain_t *)malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);
	chain = llist_create(MT_SUPPORT_TRUE);
	if (!chain)
		return (NULL);
	blockchain->chain = chain;
	fread(read, 1, FILEREAD, fd);
	fclose(fd);

	/* HEADER */
	if (strncmp(read, "HBLK", 4) != 0) /* magic number */
	{
		free(blockchain);
		llist_destroy(chain, 0, 0);
		return (NULL);
	}
	memcpy(&numBlocks, read + 8, sizeof(int));

	/* BLOCKS */
	if (!deserialize_blocks(read, (unsigned int)numBlocks, blockchain))
	{
		free(blockchain);
		llist_destroy(chain, 0, 0);
		return (NULL);
	}

	return (blockchain);
}
