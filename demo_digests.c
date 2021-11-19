#include "demo_digests.h"
#include <openssl/evp.h>
#include <openssl/md5.h>

static int md5_init(EVP_MD_CTX *ctx);
static int md5_update(EVP_MD_CTX *ctx, const void *data, size_t count);
static int md5_final(EVP_MD_CTX *ctx, unsigned char *md);

static int md5_init(EVP_MD_CTX *ctx)
{
    MD5_Init(EVP_MD_CTX_md_data(ctx));
    return 1;
}

static int md5_update(EVP_MD_CTX *ctx, const void *data, size_t count)
{
    MD5_Update(EVP_MD_CTX_md_data(ctx), data, count);
    return 1;
}

static int md5_final(EVP_MD_CTX *ctx, unsigned char *md)
{
    MD5_Final(md, EVP_MD_CTX_md_data(ctx));
    return 1;
}

static EVP_MD *_hidden_md5_md = NULL;

static const EVP_MD *digest_md5(void)
{
    EVP_MD *md = NULL;
    int res = 1;

    if (_hidden_md5_md == NULL) {
        if ((md = EVP_MD_meth_new(NID_md5, NID_md5WithRSAEncryption)) == NULL) {
            printf("Failed to allocate digest methods for nid %d\n", NID_md5);
            return NULL;
        }

        res &= EVP_MD_meth_set_result_size(md, MD5_DIGEST_LENGTH);
        res &= EVP_MD_meth_set_input_blocksize(md, MD5_CBLOCK);
        res &= EVP_MD_meth_set_app_datasize(md,
                                            sizeof(EVP_MD*) + sizeof(MD5_CTX));
        res &= EVP_MD_meth_set_flags(md, 0);
        res &= EVP_MD_meth_set_init(md, md5_init);
        res &= EVP_MD_meth_set_update(md, md5_update);
        res &= EVP_MD_meth_set_final(md, md5_final);

        if (0 == res) {
            printf("Failed to set cipher methods for nid %d\n", NID_md5);
            EVP_MD_meth_free(md);
            md = NULL;
        }

        _hidden_md5_md = md;
    }

    return _hidden_md5_md;
}

static int digest_nids[] = { NID_md5, 0 };

void demo_destroy_digests(void)
{
    EVP_MD_meth_free(_hidden_md5_md);
    _hidden_md5_md = NULL;
}

int demo_digests(ENGINE *e, const EVP_MD **digest, const int **nids, int nid)
{
    int ok = 1;

    (void)e;

    if (!digest) {
        /* We are returning a list of supported nids */
        *nids = digest_nids;
        return (sizeof(digest_nids) - 1) / sizeof(digest_nids[0]);
    }

    switch (nid) {
        case NID_md5:
            *digest = digest_md5();
            break;
        default:
            ok = 0;
            *digest = NULL;
            break;
    }

    return ok;
}
