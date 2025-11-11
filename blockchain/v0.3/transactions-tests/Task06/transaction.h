#ifndef _TRANSACTION_H_
# define _TRANSACTION_H_

# include <stdint.h>
# include <llist.h>

# include "hblk_crypto.h"

# define COINBASE_AMOUNT	50

/*********************************** TYPES ***********************************/

/**
 * tx_out_t - Transaction output
 *
 * @amount: Amount received
 * @pub:    Receiver's public address
 */
typedef struct
{
	uint32_t	amount;
	uint8_t		pub[EC_PUB_LEN];
	uint8_t		hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/**
 * tx_in_t - Transaction input
 *
 * Description: A transaction input always refers to a previous
 * transaction output. The only exception is for a Coinbase transaction, that
 * adds new coins to ciruclation.
 *
 * @tx_id:      Transaction ID of the referenced transaction output
 * @tx_out_idx: Index of the referenced output in the referenced transaction
 * @sig:        Signature. Prevents anyone from altering the content of the
 *              transaction. The transaction input is signed by the receiver
 *              of the referenced transaction output, using its private key
 */
typedef struct
{
	uint8_t		block_hash[SHA256_DIGEST_LENGTH];
	uint8_t		tx_id[SHA256_DIGEST_LENGTH];
	uint8_t		tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t		sig;
} tx_in_t;

/**
 * unspent_tx_out_t - Unspent transaction output
 *
 * Description: This structure helps identify transaction outputs that were not
 * used in any transaction input yet. That makes them "available".
 *
 * @out:        Copy of the referenced transaction output
 * @tx_id:      Transaction ID of the referenced transaction output
 * @tx_out_idx: Index of the referenced output in the referenced transaction
 */
typedef struct
{
	uint8_t		block_hash[SHA256_DIGEST_LENGTH];
	uint8_t		tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t	out;
} unspent_tx_out_t;

/**
 * transaction_t - Transaction structure
 *
 * @id:      Transaction ID. A hash of all the inputs and outputs.
 *           Prevents further alteration of the transaction.
 * @inputs:  List of `tx_in_t *`. Transaction inputs
 * @outputs: List of `tx_out_t *`. Transaction outputs
 */
typedef struct
{
	uint8_t		id[SHA256_DIGEST_LENGTH];
	llist_t		*inputs;
	llist_t		*outputs;
} transaction_t;

/********************************* FUNCTIONS *********************************/

/* tx_out_create.c */
tx_out_t		*tx_out_create(uint32_t amount,
			uint8_t const pub[EC_PUB_LEN]);

/* unspent_tx_out_create.c */
unspent_tx_out_t	*unspent_tx_out_create(
			uint8_t block_hash[SHA256_DIGEST_LENGTH],
			uint8_t tx_id[SHA256_DIGEST_LENGTH],
			tx_out_t const *out);

/* tx_in_create.c */
tx_in_t			*tx_in_create(unspent_tx_out_t const *unspent);

/* transaction_hash.c */
uint8_t			*transaction_hash(transaction_t const *transaction,
			uint8_t hash_buf[SHA256_DIGEST_LENGTH]);

/* tx_in_sign.c */
sig_t			*tx_in_sign(tx_in_t *in,
			uint8_t const tx_id[SHA256_DIGEST_LENGTH],
			EC_KEY const *sender, llist_t *all_unspent);

/* transaction_create.c */
transaction_t		*transaction_create(EC_KEY const *sender,
			EC_KEY const *receiver, uint32_t amount,
			llist_t *all_unspent);

/* transaction_is_valid.c */
int			transaction_is_valid(transaction_t const *transaction,
			llist_t *all_unspent);

/* coinbase_create.c */
transaction_t		*coinbase_create(EC_KEY const *receiver,
			uint32_t block_index);
/* coinbase_is_valid.c */
int			coinbase_is_valid(transaction_t const *transaction,
			uint32_t block_index);

/* transaction_destroy.c */
void			transaction_destroy(transaction_t *transaction);

/* update_unspent.c */
llist_t			*update_unspent(llist_t *transactions,
			uint8_t block_hash[SHA256_DIGEST_LENGTH],
			llist_t *all_unspent);

#endif /* ! _TRANSACTION_H_ */
