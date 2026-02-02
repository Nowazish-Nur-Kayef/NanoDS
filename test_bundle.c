
#define NANODS_IMPLEMENTATION
#include "nanods_bundled.h"

int main(void) {
    IntVector vec;
    nv_init_int(&vec);
    nv_push_int(&vec, 42);
    nv_free_int(&vec);
    return 0;
}
