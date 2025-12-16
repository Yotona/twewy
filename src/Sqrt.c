#include "Sqrt.h"

s32 Sqrti(s32 n) {
    if (n <= 0) {
        return 0;
    }

    s32 guess = 1;
    s32 temp  = n;

    while (guess < temp) {
        guess *= 2;
        temp >>= 1;
    }

    s32 prevGuess;
    do {
        prevGuess = guess;
        guess     = (guess + n / guess) >> 1;
    } while (prevGuess > guess);

    return prevGuess;
}
