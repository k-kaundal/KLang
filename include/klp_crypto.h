/*
 * KLP Quantum-Resistant Cryptography
 * Phase 3: Post-quantum cryptographic algorithms
 * 
 * Supports:
 * - CRYSTALS-Kyber (Key Encapsulation Mechanism)
 * - CRYSTALS-Dilithium (Digital Signatures)
 * - Hybrid mode (Classical + Post-Quantum)
 */

#ifndef KLP_CRYPTO_H
#define KLP_CRYPTO_H

#include <stdint.h>
#include <stddef.h>

/* Crypto algorithm types */
typedef enum {
    KLP_CRYPTO_NONE = 0,
    KLP_CRYPTO_CLASSICAL = 1,      /* X25519 + Ed25519 */
    KLP_CRYPTO_POST_QUANTUM = 2,   /* Kyber + Dilithium */
    KLP_CRYPTO_HYBRID = 3          /* Both */
} KLPCryptoMode;

/* Key types */
typedef enum {
    KLP_KEY_PRIVATE = 0,
    KLP_KEY_PUBLIC = 1
} KLPKeyType;

/* Kyber security levels */
typedef enum {
    KYBER_512 = 1,   /* Level 1: AES-128 equivalent */
    KYBER_768 = 2,   /* Level 3: AES-192 equivalent */
    KYBER_1024 = 3   /* Level 5: AES-256 equivalent */
} KyberLevel;

/* Dilithium security levels */
typedef enum {
    DILITHIUM_2 = 2,  /* Level 2: AES-128 equivalent */
    DILITHIUM_3 = 3,  /* Level 3: AES-192 equivalent */
    DILITHIUM_5 = 5   /* Level 5: AES-256 equivalent */
} DilithiumLevel;

/* Crypto context */
typedef struct {
    KLPCryptoMode mode;
    
    /* Kyber (KEM) */
    KyberLevel kyber_level;
    uint8_t *kyber_public_key;
    size_t kyber_public_key_len;
    uint8_t *kyber_secret_key;
    size_t kyber_secret_key_len;
    uint8_t *kyber_ciphertext;
    size_t kyber_ciphertext_len;
    uint8_t *kyber_shared_secret;
    size_t kyber_shared_secret_len;
    
    /* Dilithium (Signatures) */
    DilithiumLevel dilithium_level;
    uint8_t *dilithium_public_key;
    size_t dilithium_public_key_len;
    uint8_t *dilithium_secret_key;
    size_t dilithium_secret_key_len;
    
    /* Classical (X25519 + Ed25519) */
    uint8_t x25519_public_key[32];
    uint8_t x25519_secret_key[32];
    uint8_t ed25519_public_key[32];
    uint8_t ed25519_secret_key[64];
    
    /* Hybrid shared secret */
    uint8_t *hybrid_shared_secret;
    size_t hybrid_shared_secret_len;
    
} KLPCryptoContext;

/* Crypto handshake frame */
typedef struct {
    uint32_t version;              /* Crypto version */
    KLPCryptoMode mode;            /* Crypto mode */
    KyberLevel kyber_level;        /* Kyber security level */
    DilithiumLevel dilithium_level;/* Dilithium security level */
    uint32_t public_key_len;       /* Public key length */
    uint8_t *public_key_data;      /* Public key data */
    uint32_t signature_len;        /* Signature length (if signed) */
    uint8_t *signature_data;       /* Signature data */
} KLPCryptoHandshake;

/* Initialize crypto library */
int klp_crypto_init();
void klp_crypto_cleanup();

/* Create/destroy crypto context */
KLPCryptoContext* klp_crypto_context_create(KLPCryptoMode mode);
void klp_crypto_context_free(KLPCryptoContext *ctx);

/* Kyber (KEM) operations */
int klp_kyber_keygen(KLPCryptoContext *ctx, KyberLevel level);
int klp_kyber_encapsulate(KLPCryptoContext *ctx, const uint8_t *public_key,
                          uint8_t **ciphertext, size_t *ciphertext_len,
                          uint8_t **shared_secret, size_t *shared_secret_len);
int klp_kyber_decapsulate(KLPCryptoContext *ctx, const uint8_t *ciphertext,
                          size_t ciphertext_len, uint8_t **shared_secret,
                          size_t *shared_secret_len);

/* Dilithium (Signature) operations */
int klp_dilithium_keygen(KLPCryptoContext *ctx, DilithiumLevel level);
int klp_dilithium_sign(KLPCryptoContext *ctx, const uint8_t *message,
                       size_t message_len, uint8_t **signature,
                       size_t *signature_len);
int klp_dilithium_verify(KLPCryptoContext *ctx, const uint8_t *message,
                         size_t message_len, const uint8_t *signature,
                         size_t signature_len);

/* Classical crypto operations */
int klp_x25519_keygen(KLPCryptoContext *ctx);
int klp_x25519_derive(KLPCryptoContext *ctx, const uint8_t *peer_public_key,
                      uint8_t *shared_secret);
int klp_ed25519_keygen(KLPCryptoContext *ctx);
int klp_ed25519_sign(KLPCryptoContext *ctx, const uint8_t *message,
                     size_t message_len, uint8_t *signature);
int klp_ed25519_verify(const uint8_t *public_key, const uint8_t *message,
                       size_t message_len, const uint8_t *signature);

/* Hybrid operations */
int klp_hybrid_keygen(KLPCryptoContext *ctx, KyberLevel kyber_level);
int klp_hybrid_derive(KLPCryptoContext *ctx, const uint8_t *kyber_public_key,
                      const uint8_t *x25519_public_key,
                      uint8_t **hybrid_secret, size_t *hybrid_secret_len);

/* Handshake */
KLPCryptoHandshake* klp_crypto_handshake_create(KLPCryptoContext *ctx);
void klp_crypto_handshake_free(KLPCryptoHandshake *handshake);
int klp_crypto_handshake_serialize(KLPCryptoHandshake *handshake,
                                   uint8_t **data, size_t *data_len);
KLPCryptoHandshake* klp_crypto_handshake_deserialize(const uint8_t *data,
                                                      size_t data_len);

/* Utility */
void klp_crypto_random_bytes(uint8_t *buffer, size_t length);
int klp_crypto_constant_time_compare(const uint8_t *a, const uint8_t *b,
                                     size_t length);

#endif /* KLP_CRYPTO_H */
