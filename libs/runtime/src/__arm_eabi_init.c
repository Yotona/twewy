#include <semihosting.h>

void _ExitProcess(void) {
    sys_Exit();
}
