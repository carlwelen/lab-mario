#ifndef DTEKV_LIB_H
#define DTEKV_LIB_H

void printc(char s);
void print(char *s);
void print_dec(unsigned int x);
void print_hex32(unsigned int x);
void handle_exception(unsigned arg0, unsigned arg1, unsigned arg2,
                      unsigned arg3, unsigned arg4, unsigned arg5,
                      unsigned mcause, unsigned syscall_num);
int nextprime(int inval);

#endif
