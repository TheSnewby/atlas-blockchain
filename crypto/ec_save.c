#include "hblk_crypto.h"

/**
 * is_dir - discovers whether a folder path exists and is writeable
 * @folder: string path to the folder
 *
 * Return: 1 if path exists and is writeable, 0 otherwise
 */
int is_dir(char const *folder)
{
	struct stat st;

	if (!stat(folder, &st) && S_ISDIR(st.st_mode)) /* if success */
		return (1);
	return (0);
}

/**
 * ec_save - saves an existing EC key pair on the disk
 * @key: points to the EC key pair to be saved on disk
 * @folder: the path to the folder in which to save the keys
 *
 * Return: 1 on success, 0 on failure
 */
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *file;
	int folder_len = strlen(folder);
	char *fp_prv;
	char *fp_pub;

	if (!key || !folder)
		return (0);

	fp_prv = calloc(folder_len + strlen("key.pem") + 2, 1);
	fp_pub = calloc(folder_len + strlen("key_pub.pem") + 2, 1);

	sprintf(fp_prv, "%s/%s", folder, "key.pem");
	sprintf(fp_pub, "%s/%s", folder, "key_pub.pem");

	if (!is_dir(folder))
		return (0);

	file = fopen(fp_prv, "w");
	if (!PEM_write_ECPrivateKey(file, key, NULL, NULL, 0, NULL, NULL))
		return (0);
	fclose(file);

	file = fopen(fp_pub, "w");
	if (!PEM_write_EC_PUBKEY(file, key))
		return (0);
	fclose(file);

	return (1);
}
