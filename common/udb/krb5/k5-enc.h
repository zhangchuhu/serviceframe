#ifndef K5_ENC_H
#define K5_ENC_H

struct derived_key {
    krb5_data constant;
    krb5_key dkey;
    struct derived_key *next;
};

/* Internal structure of an opaque key identifier */
struct krb5_key_st {
    krb5_keyblock keyblock;
    int refcount;
    struct derived_key *derived;
    /*
    * Cache of data private to the cipher implementation, which we
    * don't want to have to recompute for every operation.  This may
    * include key schedules, iteration counts, etc.
    *
    * The cipher implementation is responsible for setting this up
    * whenever needed, and the enc_provider key_cleanup method must
    * then be provided to dispose of it.
    */
    void *cache;
};

/* new encryption provider api */

struct krb5_enc_provider {
    /* keybytes is the input size to make_key;
    keylength is the output size */
    size_t block_size, keybytes, keylength;

    krb5_error_code (*encrypt)(krb5_key key, const krb5_data *cipher_state,
        krb5_crypto_iov *data, size_t num_data);

    krb5_error_code (*decrypt)(krb5_key key, const krb5_data *cipher_state,
        krb5_crypto_iov *data, size_t num_data);

    /* May be NULL if the cipher is not used for a cbc-mac checksum. */
    krb5_error_code (*cbc_mac)(krb5_key key, const krb5_crypto_iov *data,
        size_t num_data, const krb5_data *ivec,
        krb5_data *output);

    krb5_error_code (*make_key)(const krb5_data *randombits,
        krb5_keyblock *key);

    krb5_error_code (*init_state)(const krb5_keyblock *key,
        krb5_keyusage keyusage,
        krb5_data *out_state);
    krb5_error_code (*free_state)(krb5_data *state);

    /* May be NULL if there is no key-derived data cached.  */
    void (*key_cleanup)(krb5_key key);
};

struct krb5_hash_provider {
    char hash_name[8];
    size_t hashsize, blocksize;

    krb5_error_code (*hash)(const krb5_crypto_iov *data, size_t num_data,
        krb5_data *output);
};

/*
* in here to deal with stuff from lib/crypto
*/

void krb5int_nfold(unsigned int inbits, const unsigned char *in,
                   unsigned int outbits, unsigned char *out);

krb5_error_code krb5int_hmac(const struct krb5_hash_provider *hash,
                             krb5_key key, const krb5_crypto_iov *data,
                             size_t num_data, krb5_data *output);

krb5_error_code
krb5int_hmac_keyblock(const struct krb5_hash_provider *hash,
                      const krb5_keyblock *keyblock,
                      const krb5_crypto_iov *data, size_t num_data,
                      krb5_data *output);

krb5_error_code krb5int_pbkdf2_hmac_sha1(const krb5_data *, unsigned long,
                                         const krb5_data *, const krb5_data *);

/*
* normally to free a cipher_state you can just memset the length to zero and
* free it.
*/
krb5_error_code krb5int_default_free_state(krb5_data *state);

/* Not sure it's ready for exposure just yet.  */
extern krb5_error_code
krb5int_c_mandatory_cksumtype(krb5_context, krb5_enctype, krb5_cksumtype *);


krb5_error_code
krb5int_aes_encrypt(krb5_key key, const krb5_data *ivec, krb5_crypto_iov *data,
                    size_t num_data);

krb5_error_code
krb5int_aes_decrypt(krb5_key key, const krb5_data *ivec, krb5_crypto_iov *data,
                    size_t num_data);

krb5_error_code
krb5_encrypt_helper(krb5_context context, const krb5_keyblock *key,
                    krb5_keyusage keyusage, const krb5_data *plain,
                    krb5_enc_data *cipher);

#endif /* K5_ENC_H */
