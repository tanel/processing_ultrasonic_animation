/*
 * {- join("\n * ", @autowarntext) -}
 *
 * Copyright 2008-2022 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */





#ifndef OPENSSL_CMS_H
# define OPENSSL_CMS_H
# pragma once

# include <openssl/macros.h>
# ifndef OPENSSL_NO_DEPRECATED_3_0
#  define HEADER_CMS_H
# endif

# include <openssl/opensslconf.h>

# ifndef OPENSSL_NO_CMS
# include <openssl/x509.h>
# include <openssl/x509v3.h>
# include <openssl/cmserr.h>
# ifdef __cplusplus
extern "C" {
# endif

typedef struct CMS_EnvelopedData_st CMS_EnvelopedData;
typedef struct CMS_ContentInfo_st CMS_ContentInfo;
typedef struct CMS_SignerInfo_st CMS_SignerInfo;
typedef struct CMS_SignedData_st CMS_SignedData;
typedef struct CMS_CertificateChoices CMS_CertificateChoices;
typedef struct CMS_RevocationInfoChoice_st CMS_RevocationInfoChoice;
typedef struct CMS_RecipientInfo_st CMS_RecipientInfo;
typedef struct CMS_ReceiptRequest_st CMS_ReceiptRequest;
typedef struct CMS_Receipt_st CMS_Receipt;
typedef struct CMS_RecipientEncryptedKey_st CMS_RecipientEncryptedKey;
typedef struct CMS_OtherKeyAttribute_st CMS_OtherKeyAttribute;



SKM_DEFINE_STACK_OF_INTERNAL(CMS_SignerInfo, CMS_SignerInfo, CMS_SignerInfo)
#define sk_CMS_SignerInfo_num(sk) OPENSSL_sk_num(ossl_check_const_CMS_SignerInfo_sk_type(sk))
#define sk_CMS_SignerInfo_value(sk, idx) ((CMS_SignerInfo *)OPENSSL_sk_value(ossl_check_const_CMS_SignerInfo_sk_type(sk), (idx)))
#define sk_CMS_SignerInfo_new(cmp) ((STACK_OF(CMS_SignerInfo) *)OPENSSL_sk_new(ossl_check_CMS_SignerInfo_compfunc_type(cmp)))
#define sk_CMS_SignerInfo_new_null() ((STACK_OF(CMS_SignerInfo) *)OPENSSL_sk_new_null())
#define sk_CMS_SignerInfo_new_reserve(cmp, n) ((STACK_OF(CMS_SignerInfo) *)OPENSSL_sk_new_reserve(ossl_check_CMS_SignerInfo_compfunc_type(cmp), (n)))
#define sk_CMS_SignerInfo_reserve(sk, n) OPENSSL_sk_reserve(ossl_check_CMS_SignerInfo_sk_type(sk), (n))
#define sk_CMS_SignerInfo_free(sk) OPENSSL_sk_free(ossl_check_CMS_SignerInfo_sk_type(sk))
#define sk_CMS_SignerInfo_zero(sk) OPENSSL_sk_zero(ossl_check_CMS_SignerInfo_sk_type(sk))
#define sk_CMS_SignerInfo_delete(sk, i) ((CMS_SignerInfo *)OPENSSL_sk_delete(ossl_check_CMS_SignerInfo_sk_type(sk), (i)))
#define sk_CMS_SignerInfo_delete_ptr(sk, ptr) ((CMS_SignerInfo *)OPENSSL_sk_delete_ptr(ossl_check_CMS_SignerInfo_sk_type(sk), ossl_check_CMS_SignerInfo_type(ptr)))
#define sk_CMS_SignerInfo_push(sk, ptr) OPENSSL_sk_push(ossl_check_CMS_SignerInfo_sk_type(sk), ossl_check_CMS_SignerInfo_type(ptr))
#define sk_CMS_SignerInfo_unshift(sk, ptr) OPENSSL_sk_unshift(ossl_check_CMS_SignerInfo_sk_type(sk), ossl_check_CMS_SignerInfo_type(ptr))
#define sk_CMS_SignerInfo_pop(sk) ((CMS_SignerInfo *)OPENSSL_sk_pop(ossl_check_CMS_SignerInfo_sk_type(sk)))
#define sk_CMS_SignerInfo_shift(sk) ((CMS_SignerInfo *)OPENSSL_sk_shift(ossl_check_CMS_SignerInfo_sk_type(sk)))
#define sk_CMS_SignerInfo_pop_free(sk, freefunc) OPENSSL_sk_pop_free(ossl_check_CMS_SignerInfo_sk_type(sk),ossl_check_CMS_SignerInfo_freefunc_type(freefunc))
#define sk_CMS_SignerInfo_insert(sk, ptr, idx) OPENSSL_sk_insert(ossl_check_CMS_SignerInfo_sk_type(sk), ossl_check_CMS_SignerInfo_type(ptr), (idx))
#define sk_CMS_SignerInfo_set(sk, idx, ptr) ((CMS_SignerInfo *)OPENSSL_sk_set(ossl_check_CMS_SignerInfo_sk_type(sk), (idx), ossl_check_CMS_SignerInfo_type(ptr)))
#define sk_CMS_SignerInfo_find(sk, ptr) OPENSSL_sk_find(ossl_check_CMS_SignerInfo_sk_type(sk), ossl_check_CMS_SignerInfo_type(ptr))
#define sk_CMS_SignerInfo_find_ex(sk, ptr) OPENSSL_sk_find_ex(ossl_check_CMS_SignerInfo_sk_type(sk), ossl_check_CMS_SignerInfo_type(ptr))
#define sk_CMS_SignerInfo_find_all(sk, ptr, pnum) OPENSSL_sk_find_all(ossl_check_CMS_SignerInfo_sk_type(sk), ossl_check_CMS_SignerInfo_type(ptr), pnum)
#define sk_CMS_SignerInfo_sort(sk) OPENSSL_sk_sort(ossl_check_CMS_SignerInfo_sk_type(sk))
#define sk_CMS_SignerInfo_is_sorted(sk) OPENSSL_sk_is_sorted(ossl_check_const_CMS_SignerInfo_sk_type(sk))
#define sk_CMS_SignerInfo_dup(sk) ((STACK_OF(CMS_SignerInfo) *)OPENSSL_sk_dup(ossl_check_const_CMS_SignerInfo_sk_type(sk)))
#define sk_CMS_SignerInfo_deep_copy(sk, copyfunc, freefunc) ((STACK_OF(CMS_SignerInfo) *)OPENSSL_sk_deep_copy(ossl_check_const_CMS_SignerInfo_sk_type(sk), ossl_check_CMS_SignerInfo_copyfunc_type(copyfunc), ossl_check_CMS_SignerInfo_freefunc_type(freefunc)))
#define sk_CMS_SignerInfo_set_cmp_func(sk, cmp) ((sk_CMS_SignerInfo_compfunc)OPENSSL_sk_set_cmp_func(ossl_check_CMS_SignerInfo_sk_type(sk), ossl_check_CMS_SignerInfo_compfunc_type(cmp)))



SKM_DEFINE_STACK_OF_INTERNAL(CMS_RecipientEncryptedKey, CMS_RecipientEncryptedKey, CMS_RecipientEncryptedKey)
#define sk_CMS_RecipientEncryptedKey_num(sk) OPENSSL_sk_num(ossl_check_const_CMS_RecipientEncryptedKey_sk_type(sk))
#define sk_CMS_RecipientEncryptedKey_value(sk, idx) ((CMS_RecipientEncryptedKey *)OPENSSL_sk_value(ossl_check_const_CMS_RecipientEncryptedKey_sk_type(sk), (idx)))
#define sk_CMS_RecipientEncryptedKey_new(cmp) ((STACK_OF(CMS_RecipientEncryptedKey) *)OPENSSL_sk_new(ossl_check_CMS_RecipientEncryptedKey_compfunc_type(cmp)))
#define sk_CMS_RecipientEncryptedKey_new_null() ((STACK_OF(CMS_RecipientEncryptedKey) *)OPENSSL_sk_new_null())
#define sk_CMS_RecipientEncryptedKey_new_reserve(cmp, n) ((STACK_OF(CMS_RecipientEncryptedKey) *)OPENSSL_sk_new_reserve(ossl_check_CMS_RecipientEncryptedKey_compfunc_type(cmp), (n)))
#define sk_CMS_RecipientEncryptedKey_reserve(sk, n) OPENSSL_sk_reserve(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), (n))
#define sk_CMS_RecipientEncryptedKey_free(sk) OPENSSL_sk_free(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk))
#define sk_CMS_RecipientEncryptedKey_zero(sk) OPENSSL_sk_zero(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk))
#define sk_CMS_RecipientEncryptedKey_delete(sk, i) ((CMS_RecipientEncryptedKey *)OPENSSL_sk_delete(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), (i)))
#define sk_CMS_RecipientEncryptedKey_delete_ptr(sk, ptr) ((CMS_RecipientEncryptedKey *)OPENSSL_sk_delete_ptr(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), ossl_check_CMS_RecipientEncryptedKey_type(ptr)))
#define sk_CMS_RecipientEncryptedKey_push(sk, ptr) OPENSSL_sk_push(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), ossl_check_CMS_RecipientEncryptedKey_type(ptr))
#define sk_CMS_RecipientEncryptedKey_unshift(sk, ptr) OPENSSL_sk_unshift(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), ossl_check_CMS_RecipientEncryptedKey_type(ptr))
#define sk_CMS_RecipientEncryptedKey_pop(sk) ((CMS_RecipientEncryptedKey *)OPENSSL_sk_pop(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk)))
#define sk_CMS_RecipientEncryptedKey_shift(sk) ((CMS_RecipientEncryptedKey *)OPENSSL_sk_shift(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk)))
#define sk_CMS_RecipientEncryptedKey_pop_free(sk, freefunc) OPENSSL_sk_pop_free(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk),ossl_check_CMS_RecipientEncryptedKey_freefunc_type(freefunc))
#define sk_CMS_RecipientEncryptedKey_insert(sk, ptr, idx) OPENSSL_sk_insert(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), ossl_check_CMS_RecipientEncryptedKey_type(ptr), (idx))
#define sk_CMS_RecipientEncryptedKey_set(sk, idx, ptr) ((CMS_RecipientEncryptedKey *)OPENSSL_sk_set(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), (idx), ossl_check_CMS_RecipientEncryptedKey_type(ptr)))
#define sk_CMS_RecipientEncryptedKey_find(sk, ptr) OPENSSL_sk_find(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), ossl_check_CMS_RecipientEncryptedKey_type(ptr))
#define sk_CMS_RecipientEncryptedKey_find_ex(sk, ptr) OPENSSL_sk_find_ex(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), ossl_check_CMS_RecipientEncryptedKey_type(ptr))
#define sk_CMS_RecipientEncryptedKey_find_all(sk, ptr, pnum) OPENSSL_sk_find_all(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), ossl_check_CMS_RecipientEncryptedKey_type(ptr), pnum)
#define sk_CMS_RecipientEncryptedKey_sort(sk) OPENSSL_sk_sort(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk))
#define sk_CMS_RecipientEncryptedKey_is_sorted(sk) OPENSSL_sk_is_sorted(ossl_check_const_CMS_RecipientEncryptedKey_sk_type(sk))
#define sk_CMS_RecipientEncryptedKey_dup(sk) ((STACK_OF(CMS_RecipientEncryptedKey) *)OPENSSL_sk_dup(ossl_check_const_CMS_RecipientEncryptedKey_sk_type(sk)))
#define sk_CMS_RecipientEncryptedKey_deep_copy(sk, copyfunc, freefunc) ((STACK_OF(CMS_RecipientEncryptedKey) *)OPENSSL_sk_deep_copy(ossl_check_const_CMS_RecipientEncryptedKey_sk_type(sk), ossl_check_CMS_RecipientEncryptedKey_copyfunc_type(copyfunc), ossl_check_CMS_RecipientEncryptedKey_freefunc_type(freefunc)))
#define sk_CMS_RecipientEncryptedKey_set_cmp_func(sk, cmp) ((sk_CMS_RecipientEncryptedKey_compfunc)OPENSSL_sk_set_cmp_func(ossl_check_CMS_RecipientEncryptedKey_sk_type(sk), ossl_check_CMS_RecipientEncryptedKey_compfunc_type(cmp)))



SKM_DEFINE_STACK_OF_INTERNAL(CMS_RecipientInfo, CMS_RecipientInfo, CMS_RecipientInfo)
#define sk_CMS_RecipientInfo_num(sk) OPENSSL_sk_num(ossl_check_const_CMS_RecipientInfo_sk_type(sk))
#define sk_CMS_RecipientInfo_value(sk, idx) ((CMS_RecipientInfo *)OPENSSL_sk_value(ossl_check_const_CMS_RecipientInfo_sk_type(sk), (idx)))
#define sk_CMS_RecipientInfo_new(cmp) ((STACK_OF(CMS_RecipientInfo) *)OPENSSL_sk_new(ossl_check_CMS_RecipientInfo_compfunc_type(cmp)))
#define sk_CMS_RecipientInfo_new_null() ((STACK_OF(CMS_RecipientInfo) *)OPENSSL_sk_new_null())
#define sk_CMS_RecipientInfo_new_reserve(cmp, n) ((STACK_OF(CMS_RecipientInfo) *)OPENSSL_sk_new_reserve(ossl_check_CMS_RecipientInfo_compfunc_type(cmp), (n)))
#define sk_CMS_RecipientInfo_reserve(sk, n) OPENSSL_sk_reserve(ossl_check_CMS_RecipientInfo_sk_type(sk), (n))
#define sk_CMS_RecipientInfo_free(sk) OPENSSL_sk_free(ossl_check_CMS_RecipientInfo_sk_type(sk))
#define sk_CMS_RecipientInfo_zero(sk) OPENSSL_sk_zero(ossl_check_CMS_RecipientInfo_sk_type(sk))
#define sk_CMS_RecipientInfo_delete(sk, i) ((CMS_RecipientInfo *)OPENSSL_sk_delete(ossl_check_CMS_RecipientInfo_sk_type(sk), (i)))
#define sk_CMS_RecipientInfo_delete_ptr(sk, ptr) ((CMS_RecipientInfo *)OPENSSL_sk_delete_ptr(ossl_check_CMS_RecipientInfo_sk_type(sk), ossl_check_CMS_RecipientInfo_type(ptr)))
#define sk_CMS_RecipientInfo_push(sk, ptr) OPENSSL_sk_push(ossl_check_CMS_RecipientInfo_sk_type(sk), ossl_check_CMS_RecipientInfo_type(ptr))
#define sk_CMS_RecipientInfo_unshift(sk, ptr) OPENSSL_sk_unshift(ossl_check_CMS_RecipientInfo_sk_type(sk), ossl_check_CMS_RecipientInfo_type(ptr))
#define sk_CMS_RecipientInfo_pop(sk) ((CMS_RecipientInfo *)OPENSSL_sk_pop(ossl_check_CMS_RecipientInfo_sk_type(sk)))
#define sk_CMS_RecipientInfo_shift(sk) ((CMS_RecipientInfo *)OPENSSL_sk_shift(ossl_check_CMS_RecipientInfo_sk_type(sk)))
#define sk_CMS_RecipientInfo_pop_free(sk, freefunc) OPENSSL_sk_pop_free(ossl_check_CMS_RecipientInfo_sk_type(sk),ossl_check_CMS_RecipientInfo_freefunc_type(freefunc))
#define sk_CMS_RecipientInfo_insert(sk, ptr, idx) OPENSSL_sk_insert(ossl_check_CMS_RecipientInfo_sk_type(sk), ossl_check_CMS_RecipientInfo_type(ptr), (idx))
#define sk_CMS_RecipientInfo_set(sk, idx, ptr) ((CMS_RecipientInfo *)OPENSSL_sk_set(ossl_check_CMS_RecipientInfo_sk_type(sk), (idx), ossl_check_CMS_RecipientInfo_type(ptr)))
#define sk_CMS_RecipientInfo_find(sk, ptr) OPENSSL_sk_find(ossl_check_CMS_RecipientInfo_sk_type(sk), ossl_check_CMS_RecipientInfo_type(ptr))
#define sk_CMS_RecipientInfo_find_ex(sk, ptr) OPENSSL_sk_find_ex(ossl_check_CMS_RecipientInfo_sk_type(sk), ossl_check_CMS_RecipientInfo_type(ptr))
#define sk_CMS_RecipientInfo_find_all(sk, ptr, pnum) OPENSSL_sk_find_all(ossl_check_CMS_RecipientInfo_sk_type(sk), ossl_check_CMS_RecipientInfo_type(ptr), pnum)
#define sk_CMS_RecipientInfo_sort(sk) OPENSSL_sk_sort(ossl_check_CMS_RecipientInfo_sk_type(sk))
#define sk_CMS_RecipientInfo_is_sorted(sk) OPENSSL_sk_is_sorted(ossl_check_const_CMS_RecipientInfo_sk_type(sk))
#define sk_CMS_RecipientInfo_dup(sk) ((STACK_OF(CMS_RecipientInfo) *)OPENSSL_sk_dup(ossl_check_const_CMS_RecipientInfo_sk_type(sk)))
#define sk_CMS_RecipientInfo_deep_copy(sk, copyfunc, freefunc) ((STACK_OF(CMS_RecipientInfo) *)OPENSSL_sk_deep_copy(ossl_check_const_CMS_RecipientInfo_sk_type(sk), ossl_check_CMS_RecipientInfo_copyfunc_type(copyfunc), ossl_check_CMS_RecipientInfo_freefunc_type(freefunc)))
#define sk_CMS_RecipientInfo_set_cmp_func(sk, cmp) ((sk_CMS_RecipientInfo_compfunc)OPENSSL_sk_set_cmp_func(ossl_check_CMS_RecipientInfo_sk_type(sk), ossl_check_CMS_RecipientInfo_compfunc_type(cmp)))



SKM_DEFINE_STACK_OF_INTERNAL(CMS_RevocationInfoChoice, CMS_RevocationInfoChoice, CMS_RevocationInfoChoice)
#define sk_CMS_RevocationInfoChoice_num(sk) OPENSSL_sk_num(ossl_check_const_CMS_RevocationInfoChoice_sk_type(sk))
#define sk_CMS_RevocationInfoChoice_value(sk, idx) ((CMS_RevocationInfoChoice *)OPENSSL_sk_value(ossl_check_const_CMS_RevocationInfoChoice_sk_type(sk), (idx)))
#define sk_CMS_RevocationInfoChoice_new(cmp) ((STACK_OF(CMS_RevocationInfoChoice) *)OPENSSL_sk_new(ossl_check_CMS_RevocationInfoChoice_compfunc_type(cmp)))
#define sk_CMS_RevocationInfoChoice_new_null() ((STACK_OF(CMS_RevocationInfoChoice) *)OPENSSL_sk_new_null())
#define sk_CMS_RevocationInfoChoice_new_reserve(cmp, n) ((STACK_OF(CMS_RevocationInfoChoice) *)OPENSSL_sk_new_reserve(ossl_check_CMS_RevocationInfoChoice_compfunc_type(cmp), (n)))
#define sk_CMS_RevocationInfoChoice_reserve(sk, n) OPENSSL_sk_reserve(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), (n))
#define sk_CMS_RevocationInfoChoice_free(sk) OPENSSL_sk_free(ossl_check_CMS_RevocationInfoChoice_sk_type(sk))
#define sk_CMS_RevocationInfoChoice_zero(sk) OPENSSL_sk_zero(ossl_check_CMS_RevocationInfoChoice_sk_type(sk))
#define sk_CMS_RevocationInfoChoice_delete(sk, i) ((CMS_RevocationInfoChoice *)OPENSSL_sk_delete(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), (i)))
#define sk_CMS_RevocationInfoChoice_delete_ptr(sk, ptr) ((CMS_RevocationInfoChoice *)OPENSSL_sk_delete_ptr(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), ossl_check_CMS_RevocationInfoChoice_type(ptr)))
#define sk_CMS_RevocationInfoChoice_push(sk, ptr) OPENSSL_sk_push(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), ossl_check_CMS_RevocationInfoChoice_type(ptr))
#define sk_CMS_RevocationInfoChoice_unshift(sk, ptr) OPENSSL_sk_unshift(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), ossl_check_CMS_RevocationInfoChoice_type(ptr))
#define sk_CMS_RevocationInfoChoice_pop(sk) ((CMS_RevocationInfoChoice *)OPENSSL_sk_pop(ossl_check_CMS_RevocationInfoChoice_sk_type(sk)))
#define sk_CMS_RevocationInfoChoice_shift(sk) ((CMS_RevocationInfoChoice *)OPENSSL_sk_shift(ossl_check_CMS_RevocationInfoChoice_sk_type(sk)))
#define sk_CMS_RevocationInfoChoice_pop_free(sk, freefunc) OPENSSL_sk_pop_free(ossl_check_CMS_RevocationInfoChoice_sk_type(sk),ossl_check_CMS_RevocationInfoChoice_freefunc_type(freefunc))
#define sk_CMS_RevocationInfoChoice_insert(sk, ptr, idx) OPENSSL_sk_insert(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), ossl_check_CMS_RevocationInfoChoice_type(ptr), (idx))
#define sk_CMS_RevocationInfoChoice_set(sk, idx, ptr) ((CMS_RevocationInfoChoice *)OPENSSL_sk_set(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), (idx), ossl_check_CMS_RevocationInfoChoice_type(ptr)))
#define sk_CMS_RevocationInfoChoice_find(sk, ptr) OPENSSL_sk_find(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), ossl_check_CMS_RevocationInfoChoice_type(ptr))
#define sk_CMS_RevocationInfoChoice_find_ex(sk, ptr) OPENSSL_sk_find_ex(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), ossl_check_CMS_RevocationInfoChoice_type(ptr))
#define sk_CMS_RevocationInfoChoice_find_all(sk, ptr, pnum) OPENSSL_sk_find_all(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), ossl_check_CMS_RevocationInfoChoice_type(ptr), pnum)
#define sk_CMS_RevocationInfoChoice_sort(sk) OPENSSL_sk_sort(ossl_check_CMS_RevocationInfoChoice_sk_type(sk))
#define sk_CMS_RevocationInfoChoice_is_sorted(sk) OPENSSL_sk_is_sorted(ossl_check_const_CMS_RevocationInfoChoice_sk_type(sk))
#define sk_CMS_RevocationInfoChoice_dup(sk) ((STACK_OF(CMS_RevocationInfoChoice) *)OPENSSL_sk_dup(ossl_check_const_CMS_RevocationInfoChoice_sk_type(sk)))
#define sk_CMS_RevocationInfoChoice_deep_copy(sk, copyfunc, freefunc) ((STACK_OF(CMS_RevocationInfoChoice) *)OPENSSL_sk_deep_copy(ossl_check_const_CMS_RevocationInfoChoice_sk_type(sk), ossl_check_CMS_RevocationInfoChoice_copyfunc_type(copyfunc), ossl_check_CMS_RevocationInfoChoice_freefunc_type(freefunc)))
#define sk_CMS_RevocationInfoChoice_set_cmp_func(sk, cmp) ((sk_CMS_RevocationInfoChoice_compfunc)OPENSSL_sk_set_cmp_func(ossl_check_CMS_RevocationInfoChoice_sk_type(sk), ossl_check_CMS_RevocationInfoChoice_compfunc_type(cmp)))




DECLARE_ASN1_ITEM(CMS_EnvelopedData)
DECLARE_ASN1_ALLOC_FUNCTIONS(CMS_SignedData)
DECLARE_ASN1_FUNCTIONS(CMS_ContentInfo)
DECLARE_ASN1_FUNCTIONS(CMS_ReceiptRequest)
DECLARE_ASN1_PRINT_FUNCTION(CMS_ContentInfo)

CMS_ContentInfo *CMS_ContentInfo_new_ex(OSSL_LIB_CTX *libctx, const char *propq);

# define CMS_SIGNERINFO_ISSUER_SERIAL    0
# define CMS_SIGNERINFO_KEYIDENTIFIER    1

# define CMS_RECIPINFO_NONE              -1
# define CMS_RECIPINFO_TRANS             0
# define CMS_RECIPINFO_AGREE             1
# define CMS_RECIPINFO_KEK               2
# define CMS_RECIPINFO_PASS              3
# define CMS_RECIPINFO_OTHER             4

/* S/MIME related flags */

# define CMS_TEXT                        0x1
# define CMS_NOCERTS                     0x2
# define CMS_NO_CONTENT_VERIFY           0x4
# define CMS_NO_ATTR_VERIFY              0x8
# define CMS_NOSIGS                      \
                        (CMS_NO_CONTENT_VERIFY|CMS_NO_ATTR_VERIFY)
# define CMS_NOINTERN                    0x10
# define CMS_NO_SIGNER_CERT_VERIFY       0x20
# define CMS_NOVERIFY                    0x20
# define CMS_DETACHED                    0x40
# define CMS_BINARY                      0x80
# define CMS_NOATTR                      0x100
# define CMS_NOSMIMECAP                  0x200
# define CMS_NOOLDMIMETYPE               0x400
# define CMS_CRLFEOL                     0x800
# define CMS_STREAM                      0x1000
# define CMS_NOCRL                       0x2000
# define CMS_PARTIAL                     0x4000
# define CMS_REUSE_DIGEST                0x8000
# define CMS_USE_KEYID                   0x10000
# define CMS_DEBUG_DECRYPT               0x20000
# define CMS_KEY_PARAM                   0x40000
# define CMS_ASCIICRLF                   0x80000
# define CMS_CADES                       0x100000
# define CMS_USE_ORIGINATOR_KEYID        0x200000

const ASN1_OBJECT *CMS_get0_type(const CMS_ContentInfo *cms);

BIO *CMS_dataInit(CMS_ContentInfo *cms, BIO *icont);
int CMS_dataFinal(CMS_ContentInfo *cms, BIO *bio);

ASN1_OCTET_STRING **CMS_get0_content(CMS_ContentInfo *cms);
int CMS_is_detached(CMS_ContentInfo *cms);
int CMS_set_detached(CMS_ContentInfo *cms, int detached);

# ifdef OPENSSL_PEM_H
DECLARE_PEM_rw(CMS, CMS_ContentInfo)
# endif
int CMS_stream(unsigned char ***boundary, CMS_ContentInfo *cms);
CMS_ContentInfo *d2i_CMS_bio(BIO *bp, CMS_ContentInfo **cms);
int i2d_CMS_bio(BIO *bp, CMS_ContentInfo *cms);

BIO *BIO_new_CMS(BIO *out, CMS_ContentInfo *cms);
int i2d_CMS_bio_stream(BIO *out, CMS_ContentInfo *cms, BIO *in, int flags);
int PEM_write_bio_CMS_stream(BIO *out, CMS_ContentInfo *cms, BIO *in,
                             int flags);
CMS_ContentInfo *SMIME_read_CMS(BIO *bio, BIO **bcont);
CMS_ContentInfo *SMIME_read_CMS_ex(BIO *bio, int flags, BIO **bcont, CMS_ContentInfo **ci);
int SMIME_write_CMS(BIO *bio, CMS_ContentInfo *cms, BIO *data, int flags);

int CMS_final(CMS_ContentInfo *cms, BIO *data, BIO *dcont,
              unsigned int flags);
int CMS_final_digest(CMS_ContentInfo *cms,
                     const unsigned char *md, unsigned int mdlen, BIO *dcont,
                     unsigned int flags);

CMS_ContentInfo *CMS_sign(X509 *signcert, EVP_PKEY *pkey,
                          STACK_OF(X509) *certs, BIO *data,
                          unsigned int flags);
CMS_ContentInfo *CMS_sign_ex(X509 *signcert, EVP_PKEY *pkey,
                             STACK_OF(X509) *certs, BIO *data,
                             unsigned int flags, OSSL_LIB_CTX *libctx,
                             const char *propq);

CMS_ContentInfo *CMS_sign_receipt(CMS_SignerInfo *si,
                                  X509 *signcert, EVP_PKEY *pkey,
                                  STACK_OF(X509) *certs, unsigned int flags);

int CMS_data(CMS_ContentInfo *cms, BIO *out, unsigned int flags);
CMS_ContentInfo *CMS_data_create(BIO *in, unsigned int flags);
CMS_ContentInfo *CMS_data_create_ex(BIO *in, unsigned int flags,
                                    OSSL_LIB_CTX *libctx, const char *propq);

int CMS_digest_verify(CMS_ContentInfo *cms, BIO *dcont, BIO *out,
                      unsigned int flags);
CMS_ContentInfo *CMS_digest_create(BIO *in, const EVP_MD *md,
                                   unsigned int flags);
CMS_ContentInfo *CMS_digest_create_ex(BIO *in, const EVP_MD *md,
                                      unsigned int flags, OSSL_LIB_CTX *libctx,
                                      const char *propq);

int CMS_EncryptedData_decrypt(CMS_ContentInfo *cms,
                              const unsigned char *key, size_t keylen,
                              BIO *dcont, BIO *out, unsigned int flags);
CMS_ContentInfo *CMS_EncryptedData_encrypt(BIO *in, const EVP_CIPHER *cipher,
                                           const unsigned char *key,
                                           size_t keylen, unsigned int flags);
CMS_ContentInfo *CMS_EncryptedData_encrypt_ex(BIO *in, const EVP_CIPHER *cipher,
                                              const unsigned char *key,
                                              size_t keylen, unsigned int flags,
                                              OSSL_LIB_CTX *libctx,
                                              const char *propq);

int CMS_EncryptedData_set1_key(CMS_ContentInfo *cms, const EVP_CIPHER *ciph,
                               const unsigned char *key, size_t keylen);

int CMS_verify(CMS_ContentInfo *cms, STACK_OF(X509) *certs,
               X509_STORE *store, BIO *dcont, BIO *out, unsigned int flags);

int CMS_verify_receipt(CMS_ContentInfo *rcms, CMS_ContentInfo *ocms,
                       STACK_OF(X509) *certs,
                       X509_STORE *store, unsigned int flags);

STACK_OF(X509) *CMS_get0_signers(CMS_ContentInfo *cms);

CMS_ContentInfo *CMS_encrypt(STACK_OF(X509) *certs, BIO *in,
                             const EVP_CIPHER *cipher, unsigned int flags);
CMS_ContentInfo *CMS_encrypt_ex(STACK_OF(X509) *certs, BIO *in,
                                const EVP_CIPHER *cipher, unsigned int flags,
                                OSSL_LIB_CTX *libctx, const char *propq);

int CMS_decrypt(CMS_ContentInfo *cms, EVP_PKEY *pkey, X509 *cert,
                BIO *dcont, BIO *out, unsigned int flags);

int CMS_decrypt_set1_pkey(CMS_ContentInfo *cms, EVP_PKEY *pk, X509 *cert);
int CMS_decrypt_set1_pkey_and_peer(CMS_ContentInfo *cms, EVP_PKEY *pk,
                                   X509 *cert, X509 *peer);
int CMS_decrypt_set1_key(CMS_ContentInfo *cms,
                         unsigned char *key, size_t keylen,
                         const unsigned char *id, size_t idlen);
int CMS_decrypt_set1_password(CMS_ContentInfo *cms,
                              unsigned char *pass, ossl_ssize_t passlen);

STACK_OF(CMS_RecipientInfo) *CMS_get0_RecipientInfos(CMS_ContentInfo *cms);
int CMS_RecipientInfo_type(CMS_RecipientInfo *ri);
EVP_PKEY_CTX *CMS_RecipientInfo_get0_pkey_ctx(CMS_RecipientInfo *ri);
CMS_ContentInfo *CMS_AuthEnvelopedData_create(const EVP_CIPHER *cipher);
CMS_ContentInfo *
CMS_AuthEnvelopedData_create_ex(const EVP_CIPHER *cipher, OSSL_LIB_CTX *libctx,
                                const char *propq);
CMS_ContentInfo *CMS_EnvelopedData_create(const EVP_CIPHER *cipher);
CMS_ContentInfo *CMS_EnvelopedData_create_ex(const EVP_CIPHER *cipher,
                                             OSSL_LIB_CTX *libctx,
                                             const char *propq);
BIO *CMS_EnvelopedData_decrypt(CMS_EnvelopedData *env, BIO *detached_data,
                               EVP_PKEY *pkey, X509 *cert,
                               ASN1_OCTET_STRING *secret, unsigned int flags,
                               OSSL_LIB_CTX *libctx, const char *propq);

CMS_RecipientInfo *CMS_add1_recipient_cert(CMS_ContentInfo *cms,
                                           X509 *recip, unsigned int flags);
CMS_RecipientInfo *CMS_add1_recipient(CMS_ContentInfo *cms, X509 *recip,
     EVP_PKEY *originatorPrivKey, X509 * originator, unsigned int flags);
int CMS_RecipientInfo_set0_pkey(CMS_RecipientInfo *ri, EVP_PKEY *pkey);
int CMS_RecipientInfo_ktri_cert_cmp(CMS_RecipientInfo *ri, X509 *cert);
int CMS_RecipientInfo_ktri_get0_algs(CMS_RecipientInfo *ri,
                                     EVP_PKEY **pk, X509 **recip,
                                     X509_ALGOR **palg);
int CMS_RecipientInfo_ktri_get0_signer_id(CMS_RecipientInfo *ri,
                                          ASN1_OCTET_STRING **keyid,
                                          X509_NAME **issuer,
                                          ASN1_INTEGER **sno);

CMS_RecipientInfo *CMS_add0_recipient_key(CMS_ContentInfo *cms, int nid,
                                          unsigned char *key, size_t keylen,
                                          unsigned char *id, size_t idlen,
                                          ASN1_GENERALIZEDTIME *date,
                                          ASN1_OBJECT *otherTypeId,
                                          ASN1_TYPE *otherType);

int CMS_RecipientInfo_kekri_get0_id(CMS_RecipientInfo *ri,
                                    X509_ALGOR **palg,
                                    ASN1_OCTET_STRING **pid,
                                    ASN1_GENERALIZEDTIME **pdate,
                                    ASN1_OBJECT **potherid,
                                    ASN1_TYPE **pothertype);

int CMS_RecipientInfo_set0_key(CMS_RecipientInfo *ri,
                               unsigned char *key, size_t keylen);

int CMS_RecipientInfo_kekri_id_cmp(CMS_RecipientInfo *ri,
                                   const unsigned char *id, size_t idlen);

int CMS_RecipientInfo_set0_password(CMS_RecipientInfo *ri,
                                    unsigned char *pass,
                                    ossl_ssize_t passlen);

CMS_RecipientInfo *CMS_add0_recipient_password(CMS_ContentInfo *cms,
                                               int iter, int wrap_nid,
                                               int pbe_nid,
                                               unsigned char *pass,
                                               ossl_ssize_t passlen,
                                               const EVP_CIPHER *kekciph);

int CMS_RecipientInfo_decrypt(CMS_ContentInfo *cms, CMS_RecipientInfo *ri);
int CMS_RecipientInfo_encrypt(const CMS_ContentInfo *cms, CMS_RecipientInfo *ri);

int CMS_uncompress(CMS_ContentInfo *cms, BIO *dcont, BIO *out,
                   unsigned int flags);
CMS_ContentInfo *CMS_compress(BIO *in, int comp_nid, unsigned int flags);

int CMS_set1_eContentType(CMS_ContentInfo *cms, const ASN1_OBJECT *oid);
const ASN1_OBJECT *CMS_get0_eContentType(CMS_ContentInfo *cms);

CMS_CertificateChoices *CMS_add0_CertificateChoices(CMS_ContentInfo *cms);
int CMS_add0_cert(CMS_ContentInfo *cms, X509 *cert);
int CMS_add1_cert(CMS_ContentInfo *cms, X509 *cert);
STACK_OF(X509) *CMS_get1_certs(CMS_ContentInfo *cms);

CMS_RevocationInfoChoice *CMS_add0_RevocationInfoChoice(CMS_ContentInfo *cms);
int CMS_add0_crl(CMS_ContentInfo *cms, X509_CRL *crl);
int CMS_add1_crl(CMS_ContentInfo *cms, X509_CRL *crl);
STACK_OF(X509_CRL) *CMS_get1_crls(CMS_ContentInfo *cms);

int CMS_SignedData_init(CMS_ContentInfo *cms);
CMS_SignerInfo *CMS_add1_signer(CMS_ContentInfo *cms,
                                X509 *signer, EVP_PKEY *pk, const EVP_MD *md,
                                unsigned int flags);
EVP_PKEY_CTX *CMS_SignerInfo_get0_pkey_ctx(CMS_SignerInfo *si);
EVP_MD_CTX *CMS_SignerInfo_get0_md_ctx(CMS_SignerInfo *si);
STACK_OF(CMS_SignerInfo) *CMS_get0_SignerInfos(CMS_ContentInfo *cms);

void CMS_SignerInfo_set1_signer_cert(CMS_SignerInfo *si, X509 *signer);
int CMS_SignerInfo_get0_signer_id(CMS_SignerInfo *si,
                                  ASN1_OCTET_STRING **keyid,
                                  X509_NAME **issuer, ASN1_INTEGER **sno);
int CMS_SignerInfo_cert_cmp(CMS_SignerInfo *si, X509 *cert);
int CMS_set1_signers_certs(CMS_ContentInfo *cms, STACK_OF(X509) *certs,
                           unsigned int flags);
void CMS_SignerInfo_get0_algs(CMS_SignerInfo *si, EVP_PKEY **pk,
                              X509 **signer, X509_ALGOR **pdig,
                              X509_ALGOR **psig);
ASN1_OCTET_STRING *CMS_SignerInfo_get0_signature(CMS_SignerInfo *si);
int CMS_SignerInfo_sign(CMS_SignerInfo *si);
int CMS_SignerInfo_verify(CMS_SignerInfo *si);
int CMS_SignerInfo_verify_content(CMS_SignerInfo *si, BIO *chain);
BIO *CMS_SignedData_verify(CMS_SignedData *sd, BIO *detached_data,
                           STACK_OF(X509) *scerts, X509_STORE *store,
                           STACK_OF(X509) *extra, STACK_OF(X509_CRL) *crls,
                           unsigned int flags,
                           OSSL_LIB_CTX *libctx, const char *propq);

int CMS_add_smimecap(CMS_SignerInfo *si, STACK_OF(X509_ALGOR) *algs);
int CMS_add_simple_smimecap(STACK_OF(X509_ALGOR) **algs,
                            int algnid, int keysize);
int CMS_add_standard_smimecap(STACK_OF(X509_ALGOR) **smcap);

int CMS_signed_get_attr_count(const CMS_SignerInfo *si);
int CMS_signed_get_attr_by_NID(const CMS_SignerInfo *si, int nid,
                               int lastpos);
int CMS_signed_get_attr_by_OBJ(const CMS_SignerInfo *si, const ASN1_OBJECT *obj,
                               int lastpos);
X509_ATTRIBUTE *CMS_signed_get_attr(const CMS_SignerInfo *si, int loc);
X509_ATTRIBUTE *CMS_signed_delete_attr(CMS_SignerInfo *si, int loc);
int CMS_signed_add1_attr(CMS_SignerInfo *si, X509_ATTRIBUTE *attr);
int CMS_signed_add1_attr_by_OBJ(CMS_SignerInfo *si,
                                const ASN1_OBJECT *obj, int type,
                                const void *bytes, int len);
int CMS_signed_add1_attr_by_NID(CMS_SignerInfo *si,
                                int nid, int type,
                                const void *bytes, int len);
int CMS_signed_add1_attr_by_txt(CMS_SignerInfo *si,
                                const char *attrname, int type,
                                const void *bytes, int len);
void *CMS_signed_get0_data_by_OBJ(const CMS_SignerInfo *si,
                                  const ASN1_OBJECT *oid,
                                  int lastpos, int type);

int CMS_unsigned_get_attr_count(const CMS_SignerInfo *si);
int CMS_unsigned_get_attr_by_NID(const CMS_SignerInfo *si, int nid,
                                 int lastpos);
int CMS_unsigned_get_attr_by_OBJ(const CMS_SignerInfo *si,
                                 const ASN1_OBJECT *obj, int lastpos);
X509_ATTRIBUTE *CMS_unsigned_get_attr(const CMS_SignerInfo *si, int loc);
X509_ATTRIBUTE *CMS_unsigned_delete_attr(CMS_SignerInfo *si, int loc);
int CMS_unsigned_add1_attr(CMS_SignerInfo *si, X509_ATTRIBUTE *attr);
int CMS_unsigned_add1_attr_by_OBJ(CMS_SignerInfo *si,
                                  const ASN1_OBJECT *obj, int type,
                                  const void *bytes, int len);
int CMS_unsigned_add1_attr_by_NID(CMS_SignerInfo *si,
                                  int nid, int type,
                                  const void *bytes, int len);
int CMS_unsigned_add1_attr_by_txt(CMS_SignerInfo *si,
                                  const char *attrname, int type,
                                  const void *bytes, int len);
void *CMS_unsigned_get0_data_by_OBJ(CMS_SignerInfo *si, ASN1_OBJECT *oid,
                                    int lastpos, int type);

int CMS_get1_ReceiptRequest(CMS_SignerInfo *si, CMS_ReceiptRequest **prr);
CMS_ReceiptRequest *CMS_ReceiptRequest_create0(
    unsigned char *id, int idlen, int allorfirst,
    STACK_OF(GENERAL_NAMES) *receiptList,
    STACK_OF(GENERAL_NAMES) *receiptsTo);
CMS_ReceiptRequest *CMS_ReceiptRequest_create0_ex(
    unsigned char *id, int idlen, int allorfirst,
    STACK_OF(GENERAL_NAMES) *receiptList,
    STACK_OF(GENERAL_NAMES) *receiptsTo,
    OSSL_LIB_CTX *libctx);

int CMS_add1_ReceiptRequest(CMS_SignerInfo *si, CMS_ReceiptRequest *rr);
void CMS_ReceiptRequest_get0_values(CMS_ReceiptRequest *rr,
                                    ASN1_STRING **pcid,
                                    int *pallorfirst,
                                    STACK_OF(GENERAL_NAMES) **plist,
                                    STACK_OF(GENERAL_NAMES) **prto);
int CMS_RecipientInfo_kari_get0_alg(CMS_RecipientInfo *ri,
                                    X509_ALGOR **palg,
                                    ASN1_OCTET_STRING **pukm);
STACK_OF(CMS_RecipientEncryptedKey)
*CMS_RecipientInfo_kari_get0_reks(CMS_RecipientInfo *ri);

int CMS_RecipientInfo_kari_get0_orig_id(CMS_RecipientInfo *ri,
                                        X509_ALGOR **pubalg,
                                        ASN1_BIT_STRING **pubkey,
                                        ASN1_OCTET_STRING **keyid,
                                        X509_NAME **issuer,
                                        ASN1_INTEGER **sno);

int CMS_RecipientInfo_kari_orig_id_cmp(CMS_RecipientInfo *ri, X509 *cert);

int CMS_RecipientEncryptedKey_get0_id(CMS_RecipientEncryptedKey *rek,
                                      ASN1_OCTET_STRING **keyid,
                                      ASN1_GENERALIZEDTIME **tm,
                                      CMS_OtherKeyAttribute **other,
                                      X509_NAME **issuer, ASN1_INTEGER **sno);
int CMS_RecipientEncryptedKey_cert_cmp(CMS_RecipientEncryptedKey *rek,
                                       X509 *cert);
int CMS_RecipientInfo_kari_set0_pkey(CMS_RecipientInfo *ri, EVP_PKEY *pk);
int CMS_RecipientInfo_kari_set0_pkey_and_peer(CMS_RecipientInfo *ri, EVP_PKEY *pk, X509 *peer);
EVP_CIPHER_CTX *CMS_RecipientInfo_kari_get0_ctx(CMS_RecipientInfo *ri);
int CMS_RecipientInfo_kari_decrypt(CMS_ContentInfo *cms,
                                   CMS_RecipientInfo *ri,
                                   CMS_RecipientEncryptedKey *rek);

int CMS_SharedInfo_encode(unsigned char **pder, X509_ALGOR *kekalg,
                          ASN1_OCTET_STRING *ukm, int keylen);

/* Backward compatibility for spelling errors. */
# define CMS_R_UNKNOWN_DIGEST_ALGORITM CMS_R_UNKNOWN_DIGEST_ALGORITHM
# define CMS_R_UNSUPPORTED_RECPIENTINFO_TYPE \
    CMS_R_UNSUPPORTED_RECIPIENTINFO_TYPE

#  ifdef  __cplusplus
}
#  endif
# endif
#endif
