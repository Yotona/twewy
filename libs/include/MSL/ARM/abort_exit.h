#ifndef MSL_ARM_ABORT_EXIT_H
#define MSL_ARM_ABORT_EXIT_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

void abort(void);
void exit(int status);
void __exit(int status);

#endif // MSL_ARM_ABORT_EXIT_H