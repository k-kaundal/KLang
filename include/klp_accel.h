/*
 * KLP Hardware Acceleration
 * Phase 4: SIMD, GPU, and Hardware Crypto Acceleration
 */

#ifndef KLP_ACCEL_H
#define KLP_ACCEL_H

#include <stdint.h>
#include <stddef.h>

/* CPU Features */
typedef enum {
    KLP_CPU_FEATURE_SSE2 = 1 << 0,
    KLP_CPU_FEATURE_SSE3 = 1 << 1,
    KLP_CPU_FEATURE_AVX = 1 << 2,
    KLP_CPU_FEATURE_AVX2 = 1 << 3,
    KLP_CPU_FEATURE_AVX512 = 1 << 4,
    KLP_CPU_FEATURE_NEON = 1 << 5,   /* ARM NEON */
    KLP_CPU_FEATURE_AES_NI = 1 << 6, /* Hardware AES */
    KLP_CPU_FEATURE_SHA = 1 << 7     /* Hardware SHA */
} KLPCPUFeature;

/* GPU Backend */
typedef enum {
    KLP_GPU_NONE = 0,
    KLP_GPU_CUDA = 1,
    KLP_GPU_OPENCL = 2,
    KLP_GPU_METAL = 3,  /* Apple */
    KLP_GPU_VULKAN = 4
} KLPGPUBackend;

/* Acceleration context */
typedef struct {
    uint32_t cpu_features;      /* Detected CPU features */
    KLPGPUBackend gpu_backend;  /* Selected GPU backend */
    void *gpu_context;          /* GPU-specific context */
    int use_hardware_crypto;    /* Use AES-NI, etc. */
} KLPAccelContext;

/* Initialize acceleration */
int klp_accel_init();
void klp_accel_cleanup();

/* CPU feature detection */
uint32_t klp_accel_detect_cpu_features();
int klp_accel_has_feature(uint32_t features, KLPCPUFeature feature);
const char* klp_accel_cpu_feature_name(KLPCPUFeature feature);

/* Create acceleration context */
KLPAccelContext* klp_accel_context_create();
void klp_accel_context_free(KLPAccelContext *ctx);

/* SIMD Operations */

/* Fast memory copy (uses SIMD if available) */
void klp_accel_memcpy(void *dest, const void *src, size_t n);

/* Fast memory set */
void klp_accel_memset(void *dest, int c, size_t n);

/* Byte-order conversion (vectorized) */
void klp_accel_hton32_array(uint32_t *data, size_t count);
void klp_accel_ntoh32_array(uint32_t *data, size_t count);

/* XOR operation (for crypto) */
void klp_accel_xor_bytes(uint8_t *dest, const uint8_t *src1, 
                         const uint8_t *src2, size_t length);

/* Checksum calculation */
uint32_t klp_accel_checksum(const uint8_t *data, size_t length);

/* GPU Operations */

/* Initialize GPU backend */
int klp_accel_gpu_init(KLPAccelContext *ctx, KLPGPUBackend backend);
void klp_accel_gpu_cleanup(KLPAccelContext *ctx);

/* GPU memory management */
void* klp_accel_gpu_malloc(KLPAccelContext *ctx, size_t size);
void klp_accel_gpu_free(KLPAccelContext *ctx, void *ptr);
int klp_accel_gpu_memcpy_to_device(KLPAccelContext *ctx, void *gpu_ptr, 
                                    const void *host_ptr, size_t size);
int klp_accel_gpu_memcpy_to_host(KLPAccelContext *ctx, void *host_ptr, 
                                   const void *gpu_ptr, size_t size);

/* Tensor operations on GPU */
int klp_accel_gpu_compress_tensor(KLPAccelContext *ctx, const void *input,
                                   size_t input_size, void **output,
                                   size_t *output_size);
int klp_accel_gpu_decompress_tensor(KLPAccelContext *ctx, const void *input,
                                     size_t input_size, void **output,
                                     size_t *output_size);

/* Hardware Crypto */

/* AES encryption (uses AES-NI if available) */
int klp_accel_aes_encrypt(KLPAccelContext *ctx, const uint8_t *key,
                           size_t key_len, const uint8_t *plaintext,
                           size_t plaintext_len, uint8_t **ciphertext,
                           size_t *ciphertext_len);
int klp_accel_aes_decrypt(KLPAccelContext *ctx, const uint8_t *key,
                           size_t key_len, const uint8_t *ciphertext,
                           size_t ciphertext_len, uint8_t **plaintext,
                           size_t *plaintext_len);

/* SHA hashing (uses SHA extensions if available) */
void klp_accel_sha256(const uint8_t *data, size_t length, uint8_t *hash);
void klp_accel_sha512(const uint8_t *data, size_t length, uint8_t *hash);

/* Hardware RNG */
int klp_accel_hardware_rng(uint8_t *buffer, size_t length);

/* Utilities */
void klp_accel_print_features(uint32_t features);
int klp_accel_benchmark_simd();  /* Benchmark SIMD vs scalar */

#endif /* KLP_ACCEL_H */
