/*
 * KLP Quantum-Resistant Cryptography Implementation
 * Phase 3: Post-quantum cryptographic algorithms
 * 
 * NOTE: This is a reference implementation. Full production deployment
 * requires linking against liboqs (Open Quantum Safe) library.
 * 
 * For production:
 *   - Install liboqs: https://github.com/open-quantum-safe/liboqs
 *   - Link with -loqs
 *   - Define ENABLE_LIBOQS to use real implementations
 */

#include "klp_crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ENABLE_LIBOQS
#include <oqs/oqs.h>
#endif

/* Initialize crypto library */
int klp_crypto_init() {
#ifdef ENABLE_LIBOQS
    /* liboqs initialization (if needed) */
    printf("KLP Crypto: Initialized with liboqs support\n");
    return 0;
#else
    printf("KLP Crypto: Initialized (stub mode - recompile with ENABLE_LIBOQS for full support)\n");
    return 0;
#endif
}

void klp_crypto_cleanup() {
    /* Cleanup resources */
}

/* Create crypto context */
KLPCryptoContext* klp_crypto_context_create(KLPCryptoMode mode) {
    KLPCryptoContext *ctx = (KLPCryptoContext*)calloc(1, sizeof(KLPCryptoContext));
    if (!ctx) return NULL;
    
    ctx->mode = mode;
    ctx->kyber_level = KYBER_1024;          /* Default: highest security */
    ctx->dilithium_level = DILITHIUM_5;     /* Default: highest security */
    
    return ctx;
}

void klp_crypto_context_free(KLPCryptoContext *ctx) {
    if (!ctx) return;
    
    /* Free Kyber keys */
    if (ctx->kyber_public_key) free(ctx->kyber_public_key);
    if (ctx->kyber_secret_key) free(ctx->kyber_secret_key);
    if (ctx->kyber_ciphertext) free(ctx->kyber_ciphertext);
    if (ctx->kyber_shared_secret) {
        /* Securely zero memory */
        memset(ctx->kyber_shared_secret, 0, ctx->kyber_shared_secret_len);
        free(ctx->kyber_shared_secret);
    }
    
    /* Free Dilithium keys */
    if (ctx->dilithium_public_key) free(ctx->dilithium_public_key);
    if (ctx->dilithium_secret_key) {
        memset(ctx->dilithium_secret_key, 0, ctx->dilithium_secret_key_len);
        free(ctx->dilithium_secret_key);
    }
    
    /* Free hybrid secret */
    if (ctx->hybrid_shared_secret) {
        memset(ctx->hybrid_shared_secret, 0, ctx->hybrid_shared_secret_len);
        free(ctx->hybrid_shared_secret);
    }
    
    /* Zero classical keys */
    memset(ctx->x25519_secret_key, 0, sizeof(ctx->x25519_secret_key));
    memset(ctx->ed25519_secret_key, 0, sizeof(ctx->ed25519_secret_key));
    
    free(ctx);
}

/* Kyber (KEM) operations */
int klp_kyber_keygen(KLPCryptoContext *ctx, KyberLevel level) {
    if (!ctx) return -1;
    
    ctx->kyber_level = level;
    
#ifdef ENABLE_LIBOQS
    const char *alg_name;
    switch (level) {
        case KYBER_512:  alg_name = OQS_KEM_alg_kyber_512; break;
        case KYBER_768:  alg_name = OQS_KEM_alg_kyber_768; break;
        case KYBER_1024: alg_name = OQS_KEM_alg_kyber_1024; break;
        default: return -1;
    }
    
    OQS_KEM *kem = OQS_KEM_new(alg_name);
    if (!kem) return -1;
    
    ctx->kyber_public_key_len = kem->length_public_key;
    ctx->kyber_secret_key_len = kem->length_secret_key;
    ctx->kyber_public_key = malloc(ctx->kyber_public_key_len);
    ctx->kyber_secret_key = malloc(ctx->kyber_secret_key_len);
    
    if (!ctx->kyber_public_key || !ctx->kyber_secret_key) {
        OQS_KEM_free(kem);
        return -1;
    }
    
    int result = OQS_KEM_keypair(kem, ctx->kyber_public_key, ctx->kyber_secret_key);
    OQS_KEM_free(kem);
    
    return result == OQS_SUCCESS ? 0 : -1;
#else
    /* Stub implementation */
    size_t pub_len = 1568;  /* Kyber-1024 public key size */
    size_t sec_len = 3168;  /* Kyber-1024 secret key size */
    
    ctx->kyber_public_key_len = pub_len;
    ctx->kyber_secret_key_len = sec_len;
    ctx->kyber_public_key = malloc(pub_len);
    ctx->kyber_secret_key = malloc(sec_len);
    
    if (!ctx->kyber_public_key || !ctx->kyber_secret_key) return -1;
    
    /* Fill with dummy data (NOT SECURE - for testing only) */
    klp_crypto_random_bytes(ctx->kyber_public_key, pub_len);
    klp_crypto_random_bytes(ctx->kyber_secret_key, sec_len);
    
    printf("KLP Crypto: Kyber keygen (stub) - Level %d\n", level);
    return 0;
#endif
}

int klp_kyber_encapsulate(KLPCryptoContext *ctx, const uint8_t *public_key,
                          uint8_t **ciphertext, size_t *ciphertext_len,
                          uint8_t **shared_secret, size_t *shared_secret_len) {
    if (!ctx || !public_key || !ciphertext || !shared_secret) return -1;
    
#ifdef ENABLE_LIBOQS
    const char *alg_name;
    switch (ctx->kyber_level) {
        case KYBER_512:  alg_name = OQS_KEM_alg_kyber_512; break;
        case KYBER_768:  alg_name = OQS_KEM_alg_kyber_768; break;
        case KYBER_1024: alg_name = OQS_KEM_alg_kyber_1024; break;
        default: return -1;
    }
    
    OQS_KEM *kem = OQS_KEM_new(alg_name);
    if (!kem) return -1;
    
    *ciphertext = malloc(kem->length_ciphertext);
    *shared_secret = malloc(kem->length_shared_secret);
    *ciphertext_len = kem->length_ciphertext;
    *shared_secret_len = kem->length_shared_secret;
    
    if (!*ciphertext || !*shared_secret) {
        OQS_KEM_free(kem);
        return -1;
    }
    
    int result = OQS_KEM_encaps(kem, *ciphertext, *shared_secret, public_key);
    OQS_KEM_free(kem);
    
    return result == OQS_SUCCESS ? 0 : -1;
#else
    /* Stub implementation */
    *ciphertext_len = 1568;  /* Kyber-1024 ciphertext size */
    *shared_secret_len = 32; /* Shared secret size */
    
    *ciphertext = malloc(*ciphertext_len);
    *shared_secret = malloc(*shared_secret_len);
    
    if (!*ciphertext || !*shared_secret) return -1;
    
    klp_crypto_random_bytes(*ciphertext, *ciphertext_len);
    klp_crypto_random_bytes(*shared_secret, *shared_secret_len);
    
    printf("KLP Crypto: Kyber encapsulate (stub)\n");
    return 0;
#endif
}

int klp_kyber_decapsulate(KLPCryptoContext *ctx, const uint8_t *ciphertext,
                          size_t ciphertext_len, uint8_t **shared_secret,
                          size_t *shared_secret_len) {
    if (!ctx || !ciphertext || !shared_secret) return -1;
    
#ifdef ENABLE_LIBOQS
    const char *alg_name;
    switch (ctx->kyber_level) {
        case KYBER_512:  alg_name = OQS_KEM_alg_kyber_512; break;
        case KYBER_768:  alg_name = OQS_KEM_alg_kyber_768; break;
        case KYBER_1024: alg_name = OQS_KEM_alg_kyber_1024; break;
        default: return -1;
    }
    
    OQS_KEM *kem = OQS_KEM_new(alg_name);
    if (!kem) return -1;
    
    *shared_secret = malloc(kem->length_shared_secret);
    *shared_secret_len = kem->length_shared_secret;
    
    if (!*shared_secret) {
        OQS_KEM_free(kem);
        return -1;
    }
    
    int result = OQS_KEM_decaps(kem, *shared_secret, ciphertext, ctx->kyber_secret_key);
    OQS_KEM_free(kem);
    
    return result == OQS_SUCCESS ? 0 : -1;
#else
    (void)ciphertext_len;
    
    *shared_secret_len = 32;
    *shared_secret = malloc(*shared_secret_len);
    
    if (!*shared_secret) return -1;
    
    klp_crypto_random_bytes(*shared_secret, *shared_secret_len);
    
    printf("KLP Crypto: Kyber decapsulate (stub)\n");
    return 0;
#endif
}

/* Dilithium (Signature) operations */
int klp_dilithium_keygen(KLPCryptoContext *ctx, DilithiumLevel level) {
    if (!ctx) return -1;
    
    ctx->dilithium_level = level;
    
#ifdef ENABLE_LIBOQS
    const char *alg_name;
    switch (level) {
        case DILITHIUM_2: alg_name = OQS_SIG_alg_dilithium_2; break;
        case DILITHIUM_3: alg_name = OQS_SIG_alg_dilithium_3; break;
        case DILITHIUM_5: alg_name = OQS_SIG_alg_dilithium_5; break;
        default: return -1;
    }
    
    OQS_SIG *sig = OQS_SIG_new(alg_name);
    if (!sig) return -1;
    
    ctx->dilithium_public_key_len = sig->length_public_key;
    ctx->dilithium_secret_key_len = sig->length_secret_key;
    ctx->dilithium_public_key = malloc(ctx->dilithium_public_key_len);
    ctx->dilithium_secret_key = malloc(ctx->dilithium_secret_key_len);
    
    if (!ctx->dilithium_public_key || !ctx->dilithium_secret_key) {
        OQS_SIG_free(sig);
        return -1;
    }
    
    int result = OQS_SIG_keypair(sig, ctx->dilithium_public_key, ctx->dilithium_secret_key);
    OQS_SIG_free(sig);
    
    return result == OQS_SUCCESS ? 0 : -1;
#else
    /* Stub implementation */
    size_t pub_len = 2592;  /* Dilithium5 public key size */
    size_t sec_len = 4864;  /* Dilithium5 secret key size */
    
    ctx->dilithium_public_key_len = pub_len;
    ctx->dilithium_secret_key_len = sec_len;
    ctx->dilithium_public_key = malloc(pub_len);
    ctx->dilithium_secret_key = malloc(sec_len);
    
    if (!ctx->dilithium_public_key || !ctx->dilithium_secret_key) return -1;
    
    klp_crypto_random_bytes(ctx->dilithium_public_key, pub_len);
    klp_crypto_random_bytes(ctx->dilithium_secret_key, sec_len);
    
    printf("KLP Crypto: Dilithium keygen (stub) - Level %d\n", level);
    return 0;
#endif
}

int klp_dilithium_sign(KLPCryptoContext *ctx, const uint8_t *message,
                       size_t message_len, uint8_t **signature,
                       size_t *signature_len) {
    if (!ctx || !message || !signature) return -1;
    
#ifdef ENABLE_LIBOQS
    const char *alg_name;
    switch (ctx->dilithium_level) {
        case DILITHIUM_2: alg_name = OQS_SIG_alg_dilithium_2; break;
        case DILITHIUM_3: alg_name = OQS_SIG_alg_dilithium_3; break;
        case DILITHIUM_5: alg_name = OQS_SIG_alg_dilithium_5; break;
        default: return -1;
    }
    
    OQS_SIG *sig = OQS_SIG_new(alg_name);
    if (!sig) return -1;
    
    *signature = malloc(sig->length_signature);
    if (!*signature) {
        OQS_SIG_free(sig);
        return -1;
    }
    
    int result = OQS_SIG_sign(sig, *signature, signature_len, message, message_len,
                              ctx->dilithium_secret_key);
    OQS_SIG_free(sig);
    
    return result == OQS_SUCCESS ? 0 : -1;
#else
    (void)message;
    (void)message_len;
    
    *signature_len = 4595;  /* Dilithium5 signature size */
    *signature = malloc(*signature_len);
    
    if (!*signature) return -1;
    
    klp_crypto_random_bytes(*signature, *signature_len);
    
    printf("KLP Crypto: Dilithium sign (stub)\n");
    return 0;
#endif
}

int klp_dilithium_verify(KLPCryptoContext *ctx, const uint8_t *message,
                         size_t message_len, const uint8_t *signature,
                         size_t signature_len) {
    if (!ctx || !message || !signature) return -1;
    
#ifdef ENABLE_LIBOQS
    const char *alg_name;
    switch (ctx->dilithium_level) {
        case DILITHIUM_2: alg_name = OQS_SIG_alg_dilithium_2; break;
        case DILITHIUM_3: alg_name = OQS_SIG_alg_dilithium_3; break;
        case DILITHIUM_5: alg_name = OQS_SIG_alg_dilithium_5; break;
        default: return -1;
    }
    
    OQS_SIG *sig = OQS_SIG_new(alg_name);
    if (!sig) return -1;
    
    int result = OQS_SIG_verify(sig, message, message_len, signature, signature_len,
                                ctx->dilithium_public_key);
    OQS_SIG_free(sig);
    
    return result == OQS_SUCCESS ? 0 : -1;
#else
    (void)message;
    (void)message_len;
    (void)signature;
    (void)signature_len;
    
    printf("KLP Crypto: Dilithium verify (stub) - always returns success\n");
    return 0;
#endif
}

/* Classical crypto operations - stubs */
int klp_x25519_keygen(KLPCryptoContext *ctx) {
    if (!ctx) return -1;
    klp_crypto_random_bytes(ctx->x25519_public_key, 32);
    klp_crypto_random_bytes(ctx->x25519_secret_key, 32);
    printf("KLP Crypto: X25519 keygen (stub)\n");
    return 0;
}

int klp_x25519_derive(KLPCryptoContext *ctx, const uint8_t *peer_public_key,
                      uint8_t *shared_secret) {
    if (!ctx || !peer_public_key || !shared_secret) return -1;
    klp_crypto_random_bytes(shared_secret, 32);
    printf("KLP Crypto: X25519 derive (stub)\n");
    return 0;
}

int klp_ed25519_keygen(KLPCryptoContext *ctx) {
    if (!ctx) return -1;
    klp_crypto_random_bytes(ctx->ed25519_public_key, 32);
    klp_crypto_random_bytes(ctx->ed25519_secret_key, 64);
    printf("KLP Crypto: Ed25519 keygen (stub)\n");
    return 0;
}

int klp_ed25519_sign(KLPCryptoContext *ctx, const uint8_t *message,
                     size_t message_len, uint8_t *signature) {
    if (!ctx || !message || !signature) return -1;
    (void)message_len;
    klp_crypto_random_bytes(signature, 64);
    printf("KLP Crypto: Ed25519 sign (stub)\n");
    return 0;
}

int klp_ed25519_verify(const uint8_t *public_key, const uint8_t *message,
                       size_t message_len, const uint8_t *signature) {
    if (!public_key || !message || !signature) return -1;
    (void)message_len;
    printf("KLP Crypto: Ed25519 verify (stub) - always returns success\n");
    return 0;
}

/* Hybrid operations */
int klp_hybrid_keygen(KLPCryptoContext *ctx, KyberLevel kyber_level) {
    if (!ctx) return -1;
    
    /* Generate both Kyber and X25519 keys */
    if (klp_kyber_keygen(ctx, kyber_level) != 0) return -1;
    if (klp_x25519_keygen(ctx) != 0) return -1;
    
    printf("KLP Crypto: Hybrid keygen complete\n");
    return 0;
}

int klp_hybrid_derive(KLPCryptoContext *ctx, const uint8_t *kyber_public_key,
                      const uint8_t *x25519_public_key,
                      uint8_t **hybrid_secret, size_t *hybrid_secret_len) {
    if (!ctx || !kyber_public_key || !x25519_public_key) return -1;
    
    /* Derive both secrets */
    uint8_t *kyber_secret = NULL;
    size_t kyber_secret_len = 0;
    uint8_t x25519_secret[32];
    
    if (klp_kyber_encapsulate(ctx, kyber_public_key, &ctx->kyber_ciphertext,
                              &ctx->kyber_ciphertext_len, &kyber_secret,
                              &kyber_secret_len) != 0) {
        return -1;
    }
    
    if (klp_x25519_derive(ctx, x25519_public_key, x25519_secret) != 0) {
        free(kyber_secret);
        return -1;
    }
    
    /* Combine secrets (XOR in this stub, use KDF in production) */
    *hybrid_secret_len = kyber_secret_len + 32;
    *hybrid_secret = malloc(*hybrid_secret_len);
    
    if (!*hybrid_secret) {
        free(kyber_secret);
        return -1;
    }
    
    memcpy(*hybrid_secret, kyber_secret, kyber_secret_len);
    memcpy(*hybrid_secret + kyber_secret_len, x25519_secret, 32);
    
    free(kyber_secret);
    memset(x25519_secret, 0, sizeof(x25519_secret));
    
    printf("KLP Crypto: Hybrid derive complete\n");
    return 0;
}

/* Handshake */
KLPCryptoHandshake* klp_crypto_handshake_create(KLPCryptoContext *ctx) {
    if (!ctx) return NULL;
    
    KLPCryptoHandshake *hs = (KLPCryptoHandshake*)calloc(1, sizeof(KLPCryptoHandshake));
    if (!hs) return NULL;
    
    hs->version = 1;
    hs->mode = ctx->mode;
    hs->kyber_level = ctx->kyber_level;
    hs->dilithium_level = ctx->dilithium_level;
    
    /* Copy public key data */
    if (ctx->mode == KLP_CRYPTO_POST_QUANTUM || ctx->mode == KLP_CRYPTO_HYBRID) {
        hs->public_key_len = ctx->kyber_public_key_len;
        hs->public_key_data = malloc(hs->public_key_len);
        if (hs->public_key_data) {
            memcpy(hs->public_key_data, ctx->kyber_public_key, hs->public_key_len);
        }
    }
    
    return hs;
}

void klp_crypto_handshake_free(KLPCryptoHandshake *handshake) {
    if (!handshake) return;
    if (handshake->public_key_data) free(handshake->public_key_data);
    if (handshake->signature_data) free(handshake->signature_data);
    free(handshake);
}

int klp_crypto_handshake_serialize(KLPCryptoHandshake *handshake,
                                   uint8_t **data, size_t *data_len) {
    if (!handshake || !data || !data_len) return -1;
    
    /* Calculate total size */
    *data_len = 16 + handshake->public_key_len + handshake->signature_len;
    *data = malloc(*data_len);
    
    if (!*data) return -1;
    
    /* Serialize */
    uint8_t *ptr = *data;
    memcpy(ptr, &handshake->version, 4); ptr += 4;
    memcpy(ptr, &handshake->mode, 4); ptr += 4;
    memcpy(ptr, &handshake->public_key_len, 4); ptr += 4;
    memcpy(ptr, &handshake->signature_len, 4); ptr += 4;
    
    if (handshake->public_key_len > 0) {
        memcpy(ptr, handshake->public_key_data, handshake->public_key_len);
        ptr += handshake->public_key_len;
    }
    
    if (handshake->signature_len > 0) {
        memcpy(ptr, handshake->signature_data, handshake->signature_len);
    }
    
    return 0;
}

KLPCryptoHandshake* klp_crypto_handshake_deserialize(const uint8_t *data,
                                                      size_t data_len) {
    if (!data || data_len < 16) return NULL;
    
    KLPCryptoHandshake *hs = (KLPCryptoHandshake*)calloc(1, sizeof(KLPCryptoHandshake));
    if (!hs) return NULL;
    
    const uint8_t *ptr = data;
    memcpy(&hs->version, ptr, 4); ptr += 4;
    memcpy(&hs->mode, ptr, 4); ptr += 4;
    memcpy(&hs->public_key_len, ptr, 4); ptr += 4;
    memcpy(&hs->signature_len, ptr, 4); ptr += 4;
    
    if (hs->public_key_len > 0) {
        hs->public_key_data = malloc(hs->public_key_len);
        if (hs->public_key_data) {
            memcpy(hs->public_key_data, ptr, hs->public_key_len);
            ptr += hs->public_key_len;
        }
    }
    
    if (hs->signature_len > 0) {
        hs->signature_data = malloc(hs->signature_len);
        if (hs->signature_data) {
            memcpy(hs->signature_data, ptr, hs->signature_len);
        }
    }
    
    return hs;
}

/* Utility */
void klp_crypto_random_bytes(uint8_t *buffer, size_t length) {
    /* NOT CRYPTOGRAPHICALLY SECURE - Use /dev/urandom or OS crypto API in production */
    for (size_t i = 0; i < length; i++) {
        buffer[i] = (uint8_t)rand();
    }
}

int klp_crypto_constant_time_compare(const uint8_t *a, const uint8_t *b,
                                     size_t length) {
    uint8_t diff = 0;
    for (size_t i = 0; i < length; i++) {
        diff |= a[i] ^ b[i];
    }
    return diff == 0 ? 0 : -1;
}
