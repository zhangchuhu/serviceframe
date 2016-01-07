/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* include/krb5.h
 *
 */

#ifndef KRB5_GENERAL__
#define KRB5_GENERAL__

/*
 * Word-size related definition.
 */

typedef unsigned char   krb5_octet;

#if INT_MAX == 0x7fff
typedef int     krb5_int16;
typedef unsigned int    krb5_ui_2;
#elif SHRT_MAX == 0x7fff
typedef short   krb5_int16;
typedef unsigned short  krb5_ui_2;
#else
#error undefined 16 bit type
#endif

#if INT_MAX == 0x7fffffffL
typedef int     krb5_int32;
typedef unsigned int    krb5_ui_4;
#elif LONG_MAX == 0x7fffffffL
typedef long    krb5_int32;
typedef unsigned long   krb5_ui_4;
#elif SHRT_MAX == 0x7fffffffL
typedef short   krb5_int32;
typedef unsigned short  krb5_ui_4;
#else
#error: undefined 32 bit type
#endif

/*
 * end wordsize.h
 */

/*
 * begin "base-defs.h"
 */

/*
 * Basic definitions for Kerberos V5 library
 */

#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif

typedef unsigned int krb5_boolean;
typedef unsigned int krb5_msgtype;
typedef unsigned int krb5_kvno;

typedef krb5_int32 krb5_addrtype;
typedef krb5_int32 krb5_enctype;
typedef krb5_int32 krb5_cksumtype;
typedef krb5_int32 krb5_authdatatype;
typedef krb5_int32 krb5_keyusage;
typedef krb5_int32 krb5_cryptotype;

typedef krb5_int32      krb5_preauthtype; /* This may change, later on */
typedef krb5_int32      krb5_flags;
typedef krb5_int32      krb5_timestamp;
typedef krb5_int32      krb5_error_code;

typedef krb5_error_code krb5_magic;

typedef struct _krb5_data {
    krb5_magic magic;
    unsigned int length;
    char *data;
} krb5_data;

/* structure for address */
typedef struct _krb5_address {
    krb5_magic magic;
    krb5_addrtype addrtype;
    unsigned int length;
    krb5_octet *contents;
} krb5_address;

typedef struct _krb5_octet_data {
    krb5_magic magic;
    unsigned int length;
    krb5_octet *data;
} krb5_octet_data;

typedef void * krb5_pointer;
typedef void const * krb5_const_pointer;

typedef struct krb5_principal_data {
    krb5_magic magic;
    krb5_data realm;
    krb5_data *data;            /* An array of strings */
    krb5_int32 length;
    krb5_int32 type;
} krb5_principal_data;

typedef krb5_principal_data * krb5_principal;
typedef const krb5_principal_data *krb5_const_principal;

#define krb5_princ_realm(context, princ) (&(princ)->realm)
#define krb5_princ_set_realm(context, princ,value) ((princ)->realm = *(value))
#define krb5_princ_set_realm_length(context, princ,value) (princ)->realm.length = (value)
#define krb5_princ_set_realm_data(context, princ,value) (princ)->realm.data = (value)
#define krb5_princ_size(context, princ) (princ)->length
#define krb5_princ_type(context, princ) (princ)->type
#define krb5_princ_name(context, princ) (princ)->data
#define krb5_princ_component(context, princ,i)  \
    (((i) < krb5_princ_size(context, princ))    \
     ? (princ)->data + (i)                      \
     : NULL)
/*
 * end "base-defs.h"
 */

struct _krb5_context;
typedef struct _krb5_context * krb5_context;

/*
 * begin "encryption.h"
 */

/* Exposed contents of a key. */
typedef struct _krb5_keyblock {
    krb5_magic magic;
    krb5_enctype enctype;
    unsigned int length;
    krb5_octet *contents;
} krb5_keyblock;

/*
 * Opaque identifier for a key.  Use with the krb5_k APIs for better
 * performance for repeated operations with the same key usage.  Key
 * identifiers must not be used simultaneously within multiple
 * threads, as they may contain mutable internal state and are not
 * mutex-protected.
 */
struct krb5_key_st;
typedef struct krb5_key_st *krb5_key;

typedef struct _krb5_checksum {
    krb5_magic magic;
    krb5_cksumtype checksum_type;       /* checksum type */
    unsigned int length;
    krb5_octet *contents;
} krb5_checksum;

typedef struct _krb5_enc_data {
    krb5_magic magic;
    krb5_enctype enctype;
    krb5_kvno kvno;
    krb5_data ciphertext;
} krb5_enc_data;

typedef struct _krb5_crypto_iov {
    krb5_cryptotype flags;
    krb5_data data;
} krb5_crypto_iov;

/* per Kerberos v5 protocol spec */

#define ENCTYPE_AES128_CTS_HMAC_SHA1_96 0x0011
#define ENCTYPE_AES256_CTS_HMAC_SHA1_96 0x0012

#define ENCTYPE_PRIVATE_LEOPARD      0x00113
#define ENCTYPE_PRIVATE_LEOPARD_S2_T 0x00114
#define ENCTYPE_PRIVATE_LEOPARD_S3_T 0x00115
#define ENCTYPE_PRIVATE_LEOPARD_S5_T 0x00116
#define ENCTYPE_PRIVATE_LEOPARD_T_S2 0x00117
#define ENCTYPE_PRIVATE_LEOPARD_T_S3 0x00118
#define ENCTYPE_PRIVATE_LEOPARD_T_S5 0x00119
#define ENCTYPE_PRIVATE_LEOPARD_T_T  0x0011a

#define CKSUMTYPE_RSA_MD5       0x0007
#define CKSUMTYPE_HMAC_SHA1_96_AES128   0x000f

krb5_error_code krb5_c_encrypt(krb5_context context, const krb5_keyblock *key,
               krb5_keyusage usage, const krb5_data *cipher_state,
               const krb5_data *input, krb5_enc_data *output);

krb5_error_code krb5_c_encrypt_length(krb5_context context, krb5_enctype enctype,
                      size_t inputlen, size_t *length);

krb5_error_code krb5_c_make_checksum(krb5_context context, krb5_cksumtype cksumtype,
                     const krb5_keyblock *key, krb5_keyusage usage,
                     const krb5_data *input, krb5_checksum *cksum);

krb5_error_code krb5_k_create_key(krb5_context context, const krb5_keyblock *key_data,
                  krb5_key *out);

/* Keys are logically immutable and can be "copied" by reference count. */
void  krb5_k_reference_key(krb5_context context, krb5_key key);

/* Decrement the reference count on a key and free it if it hits zero. */
void  krb5_k_free_key(krb5_context context, krb5_key key);

krb5_error_code 
krb5_k_encrypt(krb5_context context, krb5_key key, krb5_keyusage usage,
               const krb5_data *cipher_state, const krb5_data *input,
               krb5_enc_data *output);

krb5_error_code 
krb5_k_make_checksum(krb5_context context, krb5_cksumtype cksumtype,
                     krb5_key key, krb5_keyusage usage, const krb5_data *input,
                     krb5_checksum *cksum);

#define KRB5_KEYUSAGE_AS_REQ_PA_ENC_TS          1
#define KRB5_KEYUSAGE_KDC_REP_TICKET            2
#define KRB5_KEYUSAGE_AS_REP_ENCPART            3
#define KRB5_KEYUSAGE_TGS_REQ_AD_SESSKEY        4
#define KRB5_KEYUSAGE_TGS_REQ_AD_SUBKEY         5
#define KRB5_KEYUSAGE_TGS_REQ_AUTH_CKSUM        6
#define KRB5_KEYUSAGE_TGS_REQ_AUTH              7
#define KRB5_KEYUSAGE_TGS_REP_ENCPART_SESSKEY   8
#define KRB5_KEYUSAGE_TGS_REP_ENCPART_SUBKEY    9
#define KRB5_KEYUSAGE_AP_REQ_AUTH_CKSUM         10
#define KRB5_KEYUSAGE_AP_REQ_AUTH               11

/* AEAD APIs */
#define KRB5_CRYPTO_TYPE_EMPTY      0   /* [in] ignored */
#define KRB5_CRYPTO_TYPE_HEADER     1   /* [out] header */
#define KRB5_CRYPTO_TYPE_DATA       2   /* [in, out] plaintext */
#define KRB5_CRYPTO_TYPE_SIGN_ONLY  3   /* [in] associated data */
#define KRB5_CRYPTO_TYPE_PADDING    4   /* [out] padding */
#define KRB5_CRYPTO_TYPE_TRAILER    5   /* [out] checksum for encrypt */
#define KRB5_CRYPTO_TYPE_CHECKSUM   6   /* [out] checksum for MIC */
#define KRB5_CRYPTO_TYPE_STREAM     7   /* [in] entire message */

/*
 * end "encryption.h"
 */

/*
 * begin "proto.h"
 */

/* Protocol version number */
#define KRB5_PVNO       5

/* Message types */

#define KRB5_AS_REQ     ((krb5_msgtype)10) /* Req for initial authentication */
#define KRB5_AS_REP     ((krb5_msgtype)11) /* Response to KRB_AS_REQ request */
#define KRB5_TGS_REQ    ((krb5_msgtype)12) /* TGS request to server */
#define KRB5_TGS_REP    ((krb5_msgtype)13) /* Response to KRB_TGS_REQ req */
#define KRB5_AP_REQ     ((krb5_msgtype)14) /* application request to server */
#define KRB5_AP_REP     ((krb5_msgtype)15) /* Response to KRB_AP_REQ_MUTUAL */
#define KRB5_SAFE       ((krb5_msgtype)20) /* Safe application message */
#define KRB5_PRIV       ((krb5_msgtype)21) /* Private application message */
#define KRB5_CRED       ((krb5_msgtype)22) /* Credential forwarding message */
#define KRB5_ERROR      ((krb5_msgtype)30) /* Error response */


#define KDC_OPT_ENC_TKT_IN_SKEY         0x00000008

#define TKT_FLG_RENEWABLE               0x00800000

#define KRB5_PADATA_AP_REQ              1

#define KRB5_PADATA_YY_RANDOM           150  // ”√ªß√‹¬Î»≈¬Î
/*
 * end "proto.h"
 */

/* Time set */
typedef struct _krb5_ticket_times {
    krb5_timestamp authtime; /* XXX ? should ktime in KDC_REP == authtime
                                in ticket? otherwise client can't get this */
    krb5_timestamp starttime;           /* optional in ticket, if not present,
                                           use authtime */
    krb5_timestamp endtime;
    krb5_timestamp renew_till;
} krb5_ticket_times;

/* structure for auth data */
typedef struct _krb5_authdata {
    krb5_magic magic;
    krb5_authdatatype ad_type;
    unsigned int length;
    krb5_octet *contents;
} krb5_authdata;

/* structure for transited encoding */
typedef struct _krb5_transited {
    krb5_magic magic;
    krb5_octet tr_type;
    krb5_data tr_contents;
} krb5_transited;

typedef struct _krb5_enc_tkt_part {
    krb5_magic magic;
    /* to-be-encrypted portion */
    krb5_flags flags;                   /* flags */
    krb5_keyblock *session;             /* session key: includes enctype */
    krb5_principal client;              /* client name/realm */
    krb5_transited transited;           /* list of transited realms */
    krb5_ticket_times times;            /* auth, start, end, renew_till */
    krb5_address **caddrs;      /* array of ptrs to addresses */
    krb5_authdata **authorization_data; /* auth data */
} krb5_enc_tkt_part;

typedef struct _krb5_ticket {
    krb5_magic magic;
    /* cleartext portion */
    krb5_principal server;              /* server name/realm */
    krb5_enc_data enc_part;             /* encryption type, kvno, encrypted
                                           encoding */
    krb5_enc_tkt_part *enc_part2;       /* ptr to decrypted version, if
                                           available */
} krb5_ticket;

/* the unencrypted version */
typedef struct _krb5_authenticator {
    krb5_magic magic;
    krb5_principal client;              /* client name/realm */
    krb5_checksum *checksum;    /* checksum, includes type, optional */
    krb5_int32 cusec;                   /* client usec portion */
    krb5_timestamp ctime;               /* client sec portion */
    krb5_keyblock *subkey;              /* true session key, optional */
    krb5_ui_4 seq_number;               /* sequence #, optional */
    krb5_authdata **authorization_data; /* New add by Ari, auth data */
} krb5_authenticator;

typedef struct _krb5_tkt_authent {
    krb5_magic magic;
    krb5_ticket *ticket;
    krb5_authenticator *authenticator;
    krb5_flags ap_options;
} krb5_tkt_authent;

/* credentials:  Ticket, session key, etc. */
typedef struct _krb5_creds {
    krb5_magic magic;
    krb5_principal client;              /* client's principal identifier */
    krb5_principal server;              /* server's principal identifier */
    krb5_keyblock keyblock;             /* session encryption key info */
    krb5_ticket_times times;            /* lifetime info */
    krb5_boolean is_skey;               /* true if ticket is encrypted in
                                           another ticket's skey */
    krb5_flags ticket_flags;            /* flags in ticket */
    krb5_address **addresses;   /* addrs in ticket */
    krb5_data ticket;                   /* ticket string itself */
    krb5_data second_ticket;            /* second ticket, if related to
                                           ticket (via DUPLICATE-SKEY or
                                           ENC-TKT-IN-SKEY) */
    krb5_authdata **authdata;   /* authorization data */
} krb5_creds;

/* Last request fields */
typedef struct _krb5_last_req_entry {
    krb5_magic magic;
    krb5_int32 lr_type;
    krb5_timestamp value;
} krb5_last_req_entry;

/* pre-authentication data */
typedef struct _krb5_pa_data {
    krb5_magic magic;
    krb5_preauthtype  pa_type;
    unsigned int length;
    krb5_octet *contents;
} krb5_pa_data;

/* typed data */
/*
 * The FAST error handling logic currently assumes that this structure and
 * krb5_pa_data * can be safely cast to each other if this structure changes,
 * that code needs to be updated to copy.
 */
typedef struct _krb5_typed_data {
    krb5_magic magic;
    krb5_int32  type;
    unsigned int length;
    krb5_octet *data;
} krb5_typed_data;

typedef struct _krb5_kdc_req {
    krb5_magic magic;
    krb5_msgtype msg_type;              /* AS_REQ or TGS_REQ? */
    krb5_pa_data **padata;      /* e.g. encoded AP_REQ */
    /* real body */
    krb5_flags kdc_options;             /* requested options */
    krb5_principal client;              /* includes realm; optional */
    krb5_principal server;              /* includes realm (only used if no
                                           client) */
    krb5_timestamp from;                /* requested starttime */
    krb5_timestamp till;                /* requested endtime */
    krb5_timestamp rtime;               /* (optional) requested renew_till */
    krb5_int32 nonce;                   /* nonce to match request/response */
    int nktypes;                        /* # of ktypes, must be positive */
    krb5_enctype *ktype;                /* requested enctype(s) */
    krb5_address **addresses;   /* requested addresses, optional */
    krb5_enc_data authorization_data;   /* encrypted auth data; OPTIONAL */
    krb5_authdata **unenc_authdata; /* unencrypted auth data,
                                       if available */
    krb5_ticket **second_ticket;/* second ticket array; OPTIONAL */
    /* the following field is added in March 2009; it is a hack so
     * that FAST state can be carried to pre-authentication plugins.
     * A new plugin interface may be a better long-term approach.  It
     * is believed to be safe to extend this structure because it is
     * not found in any public APIs.
     */
    void * kdc_state;
} krb5_kdc_req;

typedef struct _krb5_enc_kdc_rep_part {
    krb5_magic magic;
    /* encrypted part: */
    krb5_msgtype msg_type;              /* krb5 message type */
    krb5_keyblock *session;             /* session key */
    krb5_last_req_entry **last_req; /* array of ptrs to entries */
    krb5_int32 nonce;                   /* nonce from request */
    krb5_timestamp key_exp;             /* expiration date */
    krb5_flags flags;                   /* ticket flags */
    krb5_ticket_times times;            /* lifetime info */
    krb5_principal server;              /* server's principal identifier */
    krb5_address **caddrs;      /* array of ptrs to addresses,
                                   optional */
    krb5_pa_data **enc_padata;          /* Windows 2000 compat */
} krb5_enc_kdc_rep_part;

typedef struct _krb5_kdc_rep {
    krb5_magic magic;
    /* cleartext part: */
    krb5_msgtype msg_type;              /* AS_REP or KDC_REP? */
    krb5_pa_data **padata;              /* preauthentication data from KDC */
    krb5_principal client;              /* client's principal identifier */
    krb5_ticket *ticket;                /* ticket */
    krb5_enc_data enc_part;             /* encryption type, kvno, encrypted
                                           encoding */
    krb5_enc_kdc_rep_part *enc_part2;/* unencrypted version, if available */
} krb5_kdc_rep;

/* error message structure */
typedef struct _krb5_error {
    krb5_magic magic;
    /* some of these may be meaningless in certain contexts */
    krb5_timestamp ctime;               /* client sec portion; optional */
    krb5_int32 cusec;                   /* client usec portion; optional */
    krb5_int32 susec;                   /* server usec portion */
    krb5_timestamp stime;               /* server sec portion */
    krb5_ui_4 error;                    /* error code (protocol error #'s) */
    krb5_principal client;              /* client's principal identifier;
                                           optional */
    krb5_principal server;              /* server's principal identifier */
    krb5_data text;                     /* descriptive text */
    krb5_data e_data;                   /* additional error-describing data */
} krb5_error;

typedef struct _krb5_ap_req {
    krb5_magic magic;
    krb5_flags ap_options;              /* requested options */
    krb5_ticket *ticket;                /* ticket */
    krb5_enc_data authenticator;        /* authenticator (already encrypted) */
} krb5_ap_req;

typedef struct _krb5_ap_rep {
    krb5_magic magic;
    krb5_enc_data enc_part;
} krb5_ap_rep;

typedef struct _krb5_ap_rep_enc_part {
    krb5_magic magic;
    krb5_timestamp ctime;               /* client time, seconds portion */
    krb5_int32 cusec;                   /* client time, microseconds portion */
    krb5_keyblock *subkey;              /* true session key, optional */
    krb5_ui_4 seq_number;               /* sequence #, optional */
} krb5_ap_rep_enc_part;

typedef struct _krb5_cred_info {
    krb5_magic magic;
    krb5_keyblock *session;             /* session key used to encrypt */
                                        /* ticket */
    krb5_principal client;              /* client name/realm, optional */
    krb5_principal server;              /* server name/realm, optional */
    krb5_flags flags;                   /* ticket flags, optional */
    krb5_ticket_times times;            /* auth, start, end, renew_till, */
                                        /* optional */
    krb5_address **caddrs;      /* array of ptrs to addresses */
} krb5_cred_info;

typedef struct _krb5_cred_enc_part {
    krb5_magic magic;
    krb5_int32 nonce;                   /* nonce, optional */
    krb5_timestamp timestamp;           /* client time */
    krb5_int32 usec;                    /* microsecond portion of time */
    krb5_address *s_address;    /* sender address, optional */
    krb5_address *r_address;    /* recipient address, optional */
    krb5_cred_info **ticket_info;
} krb5_cred_enc_part;

typedef struct _krb5_cred {
    krb5_magic magic;
    krb5_ticket **tickets;      /* tickets */
    krb5_enc_data enc_part;             /* encrypted part */
    krb5_cred_enc_part *enc_part2;      /* unencrypted version, if available*/
} krb5_cred;

/* these need to be here so the typedefs are available for the prototypes */

/* Initialize a new keyblock and allocate storage
 * for the contents of the key, which will be freed along
 * with the keyblock when krb5_free_keyblock is called.
 * It is legal to pass in a length of 0, in which
 * case contents are left unallocated.
 */
krb5_error_code 
krb5_copy_keyblock_contents(krb5_context, const krb5_keyblock *,
                            krb5_keyblock *);

krb5_error_code 
krb5_copy_data(krb5_context, const krb5_data *, krb5_data **);

krb5_error_code 
krb5_copy_addresses(krb5_context, krb5_address * const *, krb5_address ***);

krb5_error_code 
krb5_decode_ticket(const krb5_data *code, krb5_ticket **rep);

#endif /* KRB5_GENERAL__ */
/*
 * include/krb5_err.h:
 * This file is automatically generated; please do not edit it.
 */
#define KRB5KDC_ERR_BAD_PVNO                     (-1765328381L)
#define KRB5KDC_ERR_PADATA_TYPE_NOSUPP           (-1765328368L)
#define KRB5KRB_AP_ERR_BAD_INTEGRITY             (-1765328353L)
#define KRB5KRB_AP_ERR_MSG_TYPE                  (-1765328344L)
#define KRB5_PARSE_MALFORMED                     (-1765328250L)
#define KRB5_BADMSGTYPE                          (-1765328246L)
#define KRB5_NO_TKT_SUPPLIED                     (-1765328241L)
#define KRB5_KDCREP_MODIFIED                     (-1765328237L)
#define KRB5_CRYPTO_INTERNAL                     (-1765328206L)
#define KRB5_BAD_ENCTYPE                         (-1765328196L)
#define KRB5_BAD_KEYSIZE                         (-1765328195L)
#define KRB5_BAD_MSIZE                           (-1765328194L)
#define KRB5_CONFIG_ETYPE_NOSUPP                 (-1765328147L)
#define KRB5_ERR_BAD_S2K_PARAMS                  (-1765328140L)

#define ERROR_TABLE_BASE_krb5 (-1765328384L)

/*
 * include/kv5m_err.h:
 * This file is automatically generated; please do not edit it.
 */

#define KV5M_NONE                                (-1760647424L)
#define KV5M_PRINCIPAL                           (-1760647423L)
#define KV5M_DATA                                (-1760647422L)
#define KV5M_KEYBLOCK                            (-1760647421L)
#define KV5M_CHECKSUM                            (-1760647420L)
#define KV5M_ENCRYPT_BLOCK                       (-1760647419L)
#define KV5M_ENC_DATA                            (-1760647418L)
#define KV5M_CRYPTOSYSTEM_ENTRY                  (-1760647417L)
#define KV5M_CS_TABLE_ENTRY                      (-1760647416L)
#define KV5M_CHECKSUM_ENTRY                      (-1760647415L)
#define KV5M_AUTHDATA                            (-1760647414L)
#define KV5M_TRANSITED                           (-1760647413L)
#define KV5M_ENC_TKT_PART                        (-1760647412L)
#define KV5M_TICKET                              (-1760647411L)
#define KV5M_AUTHENTICATOR                       (-1760647410L)
#define KV5M_TKT_AUTHENT                         (-1760647409L)
#define KV5M_CREDS                               (-1760647408L)
#define KV5M_LAST_REQ_ENTRY                      (-1760647407L)
#define KV5M_PA_DATA                             (-1760647406L)
#define KV5M_KDC_REQ                             (-1760647405L)
#define KV5M_ENC_KDC_REP_PART                    (-1760647404L)
#define KV5M_KDC_REP                             (-1760647403L)
#define KV5M_ERROR                               (-1760647402L)
#define KV5M_AP_REQ                              (-1760647401L)
#define KV5M_AP_REP                              (-1760647400L)
#define KV5M_AP_REP_ENC_PART                     (-1760647399L)
#define KV5M_RESPONSE                            (-1760647398L)
#define KV5M_SAFE                                (-1760647397L)
#define KV5M_PRIV                                (-1760647396L)
#define KV5M_PRIV_ENC_PART                       (-1760647395L)
#define KV5M_CRED                                (-1760647394L)
#define KV5M_CRED_INFO                           (-1760647393L)
#define KV5M_CRED_ENC_PART                       (-1760647392L)
#define KV5M_PWD_DATA                            (-1760647391L)
#define KV5M_ADDRESS                             (-1760647390L)
#define KV5M_KEYTAB_ENTRY                        (-1760647389L)
#define KV5M_CONTEXT                             (-1760647388L)
#define KV5M_OS_CONTEXT                          (-1760647387L)
#define KV5M_ALT_METHOD                          (-1760647386L)
#define KV5M_ETYPE_INFO_ENTRY                    (-1760647385L)
#define KV5M_DB_CONTEXT                          (-1760647384L)
#define KV5M_AUTH_CONTEXT                        (-1760647383L)
#define KV5M_KEYTAB                              (-1760647382L)
#define KV5M_RCACHE                              (-1760647381L)
#define KV5M_CCACHE                              (-1760647380L)
#define KV5M_PREAUTH_OPS                         (-1760647379L)
#define KV5M_SAM_CHALLENGE                       (-1760647378L)
#define KV5M_SAM_CHALLENGE_2                     (-1760647377L)
#define KV5M_SAM_KEY                             (-1760647376L)
#define KV5M_ENC_SAM_RESPONSE_ENC                (-1760647375L)
#define KV5M_ENC_SAM_RESPONSE_ENC_2              (-1760647374L)
#define KV5M_SAM_RESPONSE                        (-1760647373L)
#define KV5M_SAM_RESPONSE_2                      (-1760647372L)
#define KV5M_PREDICTED_SAM_RESPONSE              (-1760647371L)
#define KV5M_PASSWD_PHRASE_ELEMENT               (-1760647370L)
#define KV5M_GSS_OID                             (-1760647369L)
#define KV5M_GSS_QUEUE                           (-1760647368L)
#define KV5M_FAST_ARMORED_REQ                    (-1760647367L)
#define KV5M_FAST_REQ                            (-1760647366L)
#define KV5M_FAST_RESPONSE                       (-1760647365L)
#define KV5M_AUTHDATA_CONTEXT                    (-1760647364L)
#define ERROR_TABLE_BASE_kv5m (-1760647424L)

/*
 * include/asn1_err.h:
 * This file is automatically generated; please do not edit it.
 */

#define ASN1_BAD_TIMEFORMAT                      (1859794432L)
#define ASN1_MISSING_FIELD                       (1859794433L)
#define ASN1_MISPLACED_FIELD                     (1859794434L)
#define ASN1_TYPE_MISMATCH                       (1859794435L)
#define ASN1_OVERFLOW                            (1859794436L)
#define ASN1_OVERRUN                             (1859794437L)
#define ASN1_BAD_ID                              (1859794438L)
#define ASN1_BAD_LENGTH                          (1859794439L)
#define ASN1_BAD_FORMAT                          (1859794440L)
#define ASN1_PARSE_ERROR                         (1859794441L)
#define ASN1_BAD_GMTIME                          (1859794442L)
#define ASN1_MISMATCH_INDEF                      (1859794443L)
#define ASN1_MISSING_EOC                         (1859794444L)
#define ERROR_TABLE_BASE_asn1 (1859794432L)
