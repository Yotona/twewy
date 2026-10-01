#include <cri/sj.h>

void SJ_SplitChunk(SJCK* ck, int nbyte, SJCK* ck1, SJCK* ck2) {
    *ck1 = *ck;

    ck2->length = ck1->length;
    if (ck1->length > nbyte) {
        ck1->length = nbyte;
    }

    ck2->length -= ck1->length;
    if (ck2->length == 0) {
        ck2->data = NULL;
    } else {
        ck2->data = ck1->data + ck1->length;
    }
}
