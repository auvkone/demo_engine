#include <stdio.h>
#include <string.h>
#include <openssl/engine.h>
#include "demo_digests.h"

static const char *engine_id = "demo";
static const char *engine_name = "A demo engine";

static int demo_destroy(ENGINE *e)
{
    (void)e;

    demo_destroy_digests();

    return 1;
}

static int bind(ENGINE *e, const char *id)
{
    if (id && (strcmp(id, engine_id) != 0)) {
        printf("ENGINE_id defined already! %s - %s\n", id, engine_id);
        return 0;
    }

    if (!ENGINE_set_id(e, engine_id) ||
        !ENGINE_set_name(e, engine_name) ||
        !ENGINE_set_digests(e, demo_digests) ||
        !ENGINE_set_destroy_function(e, demo_destroy)) {
        fprintf(stderr, "ENGINE set failed\n");
        return 0;
    }

    return 1;
}

IMPLEMENT_DYNAMIC_BIND_FN(bind)
IMPLEMENT_DYNAMIC_CHECK_FN()
