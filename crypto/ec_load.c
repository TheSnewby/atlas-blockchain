#include "hblk_crypto.h"

/**
 * is_dir - discovers whether a folder path exists and is writeable
 * @folder: string path to the folder
 *
 * Return: 1 if path exists and is writeable, 0 otherwise
 */
static int is_dir(char const *folder)
{
	struct stat st;

	if (!stat(folder, &st) && S_ISDIR(st.st_mode)) /* if success */
		return (1);
	return (0);
}

/**
 * ec_load - loads an EC key pair from the disk
 * @folder: path to the folder from which to load the keys
 *
 * Return: pointer to created EC key pair or NULL on failure
 */
EC_KEY *ec_load(char const *folder)
{
	int folder_len = strlen(folder);
	char *fp_prv;
	char *fp_pub;
	EC_KEY *key = NULL;
	EVP_PKEY *pkey_prv = NULL; /* *pkey_pub = NULL */
	FILE *file;

	if (!folder || !is_dir(folder))
		return (0);

	fp_prv = malloc(folder_len + strlen("key.pem") + 2);
	fp_pub = malloc(folder_len + strlen("key_pub.pem") + 2);

	sprintf(fp_prv, "%s/%s", folder, "key.pem");
	sprintf(fp_pub, "%s/%s", folder, "key_pub.pem");

	file = fopen(fp_prv, "r");
	pkey_prv = PEM_read_PrivateKey(file, NULL, NULL, NULL);
	fclose(file);

	/* file = fopen(fp_pub, "r"); */
	/* pkey_pub = PEM_read_PUBKEY(file, NULL, NULL, NULL); */
	/* fclose(file); */

	key = (EC_KEY *)EVP_PKEY_get0_EC_KEY(pkey_prv);
	return (key);
}
