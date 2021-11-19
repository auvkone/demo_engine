#ifndef DEMO_ENGINE_DEMO_MD5_H
#define DEMO_ENGINE_DEMO_MD5_H

# include <openssl/engine.h>
# include <openssl/ossl_typ.h>

extern int demo_digests(ENGINE *e, const EVP_MD **digest,
                        const int **nids, int nid);

#endif //DEMO_ENGINE_DEMO_MD5_H
