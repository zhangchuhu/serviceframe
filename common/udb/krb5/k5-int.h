/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 * 
 */

#ifndef _KRB5_INT_H
#define _KRB5_INT_H

#ifdef __cplusplus 
extern "C" { 
#endif

#include "autoconf.h"
#include "k5-platform.h"
#include "krb5.h"
#include "k5-enc.h"
#include "kdc-config.h"

#include "time.h"

/*
 * Attempt to zero memory in a way that compilers won't optimize out.
 *
 * This mechanism should work even for heap storage about to be freed,
 * or automatic storage right before we return from a function.
 *
 * Then, even if we leak uninitialized memory someplace, or UNIX
 * "core" files get created with world-read access, some of the most
 * sensitive data in the process memory will already be safely wiped.
 *
 * We're not going so far -- yet -- as to try to protect key data that
 * may have been written into swap space....
 */
#ifdef _WIN32
# define zap(ptr, len) SecureZeroMemory(ptr, len)
#else
static inline void zap(void *ptr, size_t len)
{
    memset(ptr, 0, len);
    /*
     * Some versions of gcc have gotten clever enough to eliminate a
     * memset call right before the block in question is released.
     * This (empty) asm requires it to assume that we're doing
     * something interesting with the stored (zero) value, so the
     * memset can't be eliminated.
     *
     * An optimizer that looks at assembly or object code may not be
     * fooled, and may still cause the memset to go away.  Address
     * that problem if and when we encounter it.
     *
     * This also may not be enough if free() does something
     * interesting like purge memory locations from a write-back cache
     * that hasn't written back the zero bytes yet.  A memory barrier
     * instruction would help in that case.
     */
    asm volatile ("" : : "g" (ptr), "g" (len));
}
#endif

/* Convenience function: zap and free ptr if it is non-NULL. */
static inline void
zapfree(void *ptr, size_t len)
{
    if (ptr != NULL) {
        zap(ptr, len);
        free(ptr);
    }
}

/*
 * Combine two keys (normally used by the hardware preauth mechanism)
 */

void krb5int_c_free_keyblock(krb5_context, krb5_keyblock *key);
void krb5int_c_free_keyblock_contents(krb5_context, krb5_keyblock *);
krb5_error_code krb5int_c_init_keyblock(krb5_context, krb5_enctype enctype,
                                        size_t length, krb5_keyblock **out);
krb5_error_code krb5int_c_copy_keyblock(krb5_context context,
                                        const krb5_keyblock *from,
                                        krb5_keyblock **to);
krb5_error_code krb5int_c_copy_keyblock_contents(krb5_context context,
                                                 const krb5_keyblock *from,
                                                 krb5_keyblock *to);

krb5_error_code
krb5int_copy_data_contents(krb5_context, const krb5_data *, krb5_data *);

krb5_error_code
krb5int_copy_data_contents_add0(krb5_context, const krb5_data *, krb5_data *);

krb5_error_code
krb5int_copy_creds_contents(krb5_context, const krb5_creds *, krb5_creds *);

void krb5int_free_data_list(krb5_context context, krb5_data *data);

struct _krb5_context {
    krb5_magic magic;
};

/*
 * Begin "asn1.h"
 */
#ifndef KRB5_ASN1__
#define KRB5_ASN1__

/* ASN.1 encoding knowledge; KEEP IN SYNC WITH ASN.1 defs! */
/* here we use some knowledge of ASN.1 encodings */
/*
  Ticket is APPLICATION 1.
  Authenticator is APPLICATION 2.
  AS_REQ is APPLICATION 10.
  AS_REP is APPLICATION 11.
  TGS_REQ is APPLICATION 12.
  TGS_REP is APPLICATION 13.
  AP_REQ is APPLICATION 14.
  AP_REP is APPLICATION 15.
  KRB_SAFE is APPLICATION 20.
  KRB_PRIV is APPLICATION 21.
  KRB_CRED is APPLICATION 22.
  EncASRepPart is APPLICATION 25.
  EncTGSRepPart is APPLICATION 26.
  EncAPRepPart is APPLICATION 27.
  EncKrbPrivPart is APPLICATION 28.
  EncKrbCredPart is APPLICATION 29.
  KRB_ERROR is APPLICATION 30.
*/
/* allow either constructed or primitive encoding, so check for bit 6
   set or reset */
#define krb5int_is_app_tag(dat,tag)                     \
    ((dat != NULL) && (dat)->length &&                  \
     ((((dat)->data[0] & ~0x20) == ((tag) | 0x40))))
#define krb5_is_krb_ticket(dat)               krb5int_is_app_tag(dat, 1)
#define krb5_is_krb_authenticator(dat)        krb5int_is_app_tag(dat, 2)
#define krb5_is_as_req(dat)                   krb5int_is_app_tag(dat, 10)
#define krb5_is_as_rep(dat)                   krb5int_is_app_tag(dat, 11)
#define krb5_is_tgs_req(dat)                  krb5int_is_app_tag(dat, 12)
#define krb5_is_tgs_rep(dat)                  krb5int_is_app_tag(dat, 13)
#define krb5_is_ap_req(dat)                   krb5int_is_app_tag(dat, 14)
#define krb5_is_ap_rep(dat)                   krb5int_is_app_tag(dat, 15)
#define krb5_is_krb_safe(dat)                 krb5int_is_app_tag(dat, 20)
#define krb5_is_krb_priv(dat)                 krb5int_is_app_tag(dat, 21)
#define krb5_is_krb_cred(dat)                 krb5int_is_app_tag(dat, 22)
#define krb5_is_krb_enc_as_rep_part(dat)      krb5int_is_app_tag(dat, 25)
#define krb5_is_krb_enc_tgs_rep_part(dat)     krb5int_is_app_tag(dat, 26)
#define krb5_is_krb_enc_ap_rep_part(dat)      krb5int_is_app_tag(dat, 27)
#define krb5_is_krb_enc_krb_priv_part(dat)    krb5int_is_app_tag(dat, 28)
#define krb5_is_krb_enc_krb_cred_part(dat)    krb5int_is_app_tag(dat, 29)
#define krb5_is_krb_error(dat)                krb5int_is_app_tag(dat, 30)

/*************************************************************************
 * Prototypes for krb5_encode.c
 *************************************************************************/

/*
  krb5_error_code encode_krb5_structure(const krb5_structure *rep,
  krb5_data **code);
  modifies  *code
  effects   Returns the ASN.1 encoding of *rep in **code.
  Returns ASN1_MISSING_FIELD if a required field is emtpy in *rep.
  Returns ENOMEM if memory runs out.
*/

krb5_error_code
encode_krb5_authenticator(const krb5_authenticator *rep, krb5_data **code);

krb5_error_code
encode_krb5_ticket(const krb5_ticket *rep, krb5_data **code);

krb5_error_code
encode_krb5_enc_tkt_part(const krb5_enc_tkt_part *rep, krb5_data **code);

krb5_error_code
encode_krb5_enc_kdc_rep_part(const krb5_enc_kdc_rep_part *rep,
                             krb5_data **code);

/* yes, the translation is identical to that used for KDC__REP */
krb5_error_code
encode_krb5_as_rep(const krb5_kdc_rep *rep, krb5_data **code);

/* yes, the translation is identical to that used for KDC__REP */
krb5_error_code
encode_krb5_tgs_rep(const krb5_kdc_rep *rep, krb5_data **code);

krb5_error_code
encode_krb5_ap_req(const krb5_ap_req *rep, krb5_data **code);

krb5_error_code
encode_krb5_ap_rep(const krb5_ap_rep *rep, krb5_data **code);

krb5_error_code
encode_krb5_ap_rep_enc_part(const krb5_ap_rep_enc_part *rep, krb5_data **code);

krb5_error_code
encode_krb5_as_req(const krb5_kdc_req *rep, krb5_data **code);

krb5_error_code
encode_krb5_tgs_req(const krb5_kdc_req *rep, krb5_data **code);

krb5_error_code
encode_krb5_kdc_req_body(const krb5_kdc_req *rep, krb5_data **code);

krb5_error_code
encode_krb5_cred(const krb5_cred *rep, krb5_data **code);

krb5_error_code
encode_krb5_enc_cred_part(const krb5_cred_enc_part *rep, krb5_data **code);

krb5_error_code
encode_krb5_error(const krb5_error *rep, krb5_data **code);

krb5_error_code
encode_krb5_authdata(krb5_authdata *const *rep, krb5_data **code);

krb5_error_code
encode_krb5_authdata_elt(const krb5_authdata *rep, krb5_data **code);

krb5_error_code
encode_krb5_padata_sequence(krb5_pa_data *const *rep, krb5_data **code);

/*************************************************************************
 * Prototypes for krb5_decode.c
 *************************************************************************/
/*
  krb5_error_code decode_krb5_structure(const krb5_data *code,
  krb5_structure **rep);

  requires  Expects **rep to not have been allocated;
  a new *rep is allocated regardless of the old value.
  effects   Decodes *code into **rep.
  Returns ENOMEM if memory is exhausted.
  Returns asn1 and krb5 errors.
*/

krb5_error_code
decode_krb5_authenticator(const krb5_data *code, krb5_authenticator **rep);

krb5_error_code
decode_krb5_ticket(const krb5_data *code, krb5_ticket **rep);

krb5_error_code
decode_krb5_encryption_key(const krb5_data *output, krb5_keyblock **rep);

krb5_error_code
decode_krb5_enc_tkt_part(const krb5_data *output, krb5_enc_tkt_part **rep);

krb5_error_code
decode_krb5_enc_kdc_rep_part(const krb5_data *output,
                             krb5_enc_kdc_rep_part **rep);

krb5_error_code
decode_krb5_as_rep(const krb5_data *output, krb5_kdc_rep **rep);

krb5_error_code
decode_krb5_tgs_rep(const krb5_data *output, krb5_kdc_rep **rep);

krb5_error_code
decode_krb5_ap_req(const krb5_data *output, krb5_ap_req **rep);

krb5_error_code
decode_krb5_ap_rep(const krb5_data *output, krb5_ap_rep **rep);

krb5_error_code
decode_krb5_ap_rep_enc_part(const krb5_data *output,
                            krb5_ap_rep_enc_part **rep);

krb5_error_code
decode_krb5_as_req(const krb5_data *output, krb5_kdc_req **rep);

krb5_error_code
decode_krb5_tgs_req(const krb5_data *output, krb5_kdc_req **rep);

krb5_error_code
decode_krb5_kdc_req_body(const krb5_data *output, krb5_kdc_req **rep);

krb5_error_code
decode_krb5_cred(const krb5_data *output, krb5_cred **rep);

krb5_error_code
decode_krb5_enc_cred_part(const krb5_data *output, krb5_cred_enc_part **rep);

krb5_error_code
decode_krb5_error(const krb5_data *output, krb5_error **rep);

krb5_error_code
decode_krb5_authdata(const krb5_data *output, krb5_authdata ***rep);

krb5_error_code
decode_krb5_padata_sequence(const krb5_data *output, krb5_pa_data ***rep);

krb5_error_code
decode_krb5_enc_data(const krb5_data *output, krb5_enc_data **rep);

krb5_error_code
decode_krb5_setpw_req(const krb5_data *, krb5_data **, krb5_principal *);

krb5_error_code
decode_krb5_pa_fx_fast_reply(const krb5_data *, krb5_enc_data **);

/*************************************************************************
 * End of prototypes for krb5_decode.c
 *************************************************************************/

#endif /* KRB5_ASN1__ */
/*
 * End "asn1.h"
 */

/* free_rtree.c */

void krb5_free_authenticator_contents(krb5_context, krb5_authenticator *);
void krb5_free_address(krb5_context, krb5_address *);
void krb5_free_enc_tkt_part(krb5_context, krb5_enc_tkt_part *);
void krb5_free_tickets(krb5_context, krb5_ticket **);
void  krb5_free_kdc_req(krb5_context, krb5_kdc_req *);
void  krb5_free_kdc_req_contents(krb5_context context, krb5_kdc_req *val);
void  krb5_free_kdc_rep(krb5_context, krb5_kdc_rep *);
void  krb5_free_last_req(krb5_context, krb5_last_req_entry **);
void  krb5_free_enc_kdc_rep_part(krb5_context, krb5_enc_kdc_rep_part *);
void  krb5_free_ap_req(krb5_context, krb5_ap_req *);
void  krb5_free_ap_req_contents(krb5_context, krb5_ap_req *);
void  krb5_free_ap_rep(krb5_context, krb5_ap_rep *);
void  krb5_free_cred(krb5_context, krb5_cred *);
void  krb5_free_cred_contents(krb5_context, krb5_creds *);
void  krb5_free_cred_enc_part(krb5_context, krb5_cred_enc_part *);
void  krb5_free_pa_data(krb5_context, krb5_pa_data **);
void  krb5_free_tkt_authent(krb5_context, krb5_tkt_authent *);
void  krb5_free_enc_data(krb5_context, krb5_enc_data *);
void  krb5_free_ticket(krb5_context, krb5_ticket * );
void  krb5_free_keyblock(krb5_context context, register krb5_keyblock *val);
void  krb5_free_checksum_contents(krb5_context context, register krb5_checksum *val);
void  krb5_free_checksum(krb5_context context, register krb5_checksum *val);
void  krb5_free_principal(krb5_context context, krb5_principal val);
void  krb5_free_authdata(krb5_context context, krb5_authdata **val);
void  krb5_free_keyblock_contents(krb5_context context, register krb5_keyblock *key);
void  krb5_free_addresses(krb5_context context, krb5_address **val);
void  krb5_free_data(krb5_context context, krb5_data *val);
void  krb5_free_creds(krb5_context context, krb5_creds *val);
void  krb5_free_data_contents(krb5_context context, krb5_data *val);
void  krb5_free_authenticator(krb5_context context, krb5_authenticator *val);
void  krb5_free_ap_rep_enc_part(krb5_context context, krb5_ap_rep_enc_part *val);
void  krb5_free_error(krb5_context context, register krb5_error *val);

/* Some data comparison and conversion functions.  */
static inline int
data_eq(krb5_data d1, krb5_data d2)
{
    return (d1.length == d2.length
            && !memcmp(d1.data, d2.data, d1.length));
}

static inline krb5_data
make_data(void *data, unsigned int len)
{
    krb5_data d;

    d.magic = KV5M_DATA;
    d.data = (char *) data;
    d.length = len;
    return d;
}

static inline krb5_data
empty_data()
{
    return make_data(NULL, 0);
}

static inline krb5_data
string2data(char *str)
{
    return make_data(str, strlen(str));
}

static inline krb5_error_code
alloc_data(krb5_data *data, unsigned int len)
{
    /* Allocate at least one byte since zero-byte allocs may return NULL. */
    char *ptr = (char *) calloc((len > 0) ? len : 1, 1);

    if (ptr == NULL)
        return ENOMEM;
    data->magic = KV5M_DATA;
    data->data = ptr;
    data->length = len;
    return 0;
}

static inline int
data_eq_string (krb5_data d, char *s)
{
    return data_eq(d, string2data(s));
}

static inline int
authdata_eq(krb5_authdata a1, krb5_authdata a2)
{
    return (a1.ad_type == a2.ad_type
            && a1.length == a2.length
            && !memcmp(a1.contents, a2.contents, a1.length));
}

/* Allocate zeroed memory; set *code to 0 on success or ENOMEM on failure. */
static inline void *
k5alloc(size_t len, krb5_error_code *code)
{
    void *ptr;

    /* Allocate at least one byte since zero-byte allocs may return NULL. */
    ptr = calloc((len > 0) ? len : 1, 1);
    *code = (ptr == NULL) ? ENOMEM : 0;
    return ptr;
}

time_t krb5int_gmt_mktime (struct tm *);
krb5_error_code uauth_set_server_time(krb5_int32 srv_seconds);
krb5_error_code krb5_us_timeofday(krb5_int32 *seconds, krb5_int32 *microseconds);
krb5_error_code krb5_c_random_make_octets(krb5_context context, krb5_data *data);

#ifdef __cplusplus 
}
#endif

#endif /* _KRB5_INT_H */
