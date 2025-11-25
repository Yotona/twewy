#include <CriWare/sj.h>

void SJERR_CallErr(const char* msg) {
    SVM_CallErr1(msg);
}