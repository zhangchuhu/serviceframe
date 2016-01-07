#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>

#include "RC4_Helper.h"

using namespace std;
using namespace core;

std::string rc4Helper::genRC4Key(){
	unsigned char buf[DEF_SESSIONKEY_LENGTH];
	RAND_pseudo_bytes(buf, DEF_SESSIONKEY_LENGTH);
	return string((const char*)buf);
}

std::string rc4Helper::encRC4Key(const std::string &pubkey, const std::string &e, const std::string &rc4){
	RSA *key = RSA_new();

	key->n = BN_bin2bn((const unsigned char *)pubkey.data(), pubkey.length(), key->n);
	key->e = BN_bin2bn((const unsigned char *)e.data(), e.length(),	key->e);
	unsigned char ctext[100];

	if (RSA_size(key) >100) {
		RSA_free(key);
		return string("");
	}
	int num = RSA_public_encrypt(DEF_SESSIONKEY_LENGTH, (const unsigned char*)rc4.data(), ctext, key, 
		RSA_PKCS1_PADDING);

	string rc4key_en = std::string((const char *)ctext, num);

	RSA_free(key);

	return rc4key_en;
}
