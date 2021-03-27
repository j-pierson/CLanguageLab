/*
 * CS 11, C track, lab 8
 *
 * FILE: bci.c
 *       Implementation of the bytecode interpreter.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bci.h"


/* Define the virtual machine. */
vm_type vm;


/* Initialize the virtual machine. */
void init_vm(void)
{
    int i;

    /*
     * Initialize the stack.  It grows to the right i.e.
     * to higher memory.
     */

    vm.sp = 0;

    for (i = 0; i < STACK_SIZE; i++)
    {
        vm.stack[i] = 0;
    }

    /*
     * Initialize the registers to all zeroes.
     */

    for (i = 0; i < NREGS; i++)
    {
        vm.reg[i] = 0;
    }

    /*
     * Initialize the instruction buffer to all zeroes.
     */

    for (i = 0; i < MAX_INSTS; i++)
    {
        vm.inst[i] = 0;
    }

    vm.ip = 0;
}


/*
 * Helper function to read in integer values which take up varying
 * numbers of bytes from the instruction array 'vm.inst'.
 *
 * NOTES:
 * 1) This function moves 'vm.ip' past the integer's location
 *    in memory.
 * 2) This function assumes that integers take up 4 bytes and are
 *    arranged in a little-endian order (low-order bytes at the
 *    beginning).  This should hold for any pentium-based microprocessor.
 * 3) This function only works for n = 1, 2, or 4 bytes.
 *
 */

int read_n_byte_integer(int n)
{
    int i;
    unsigned char *val_ptr;
    int val = 0;

    /* This only works for 1, 2, or 4 byte integers. */
    assert((n == 1) || (n == 2) || (n == 4));

    val_ptr = (unsigned char *)(&val);

    for (i = 0; i < n; i++)
    {
        *val_ptr = vm.inst[vm.ip];
        val_ptr++;
        vm.ip++;
    }

    return val;
}


/*
 * Machine operations.
 */

/* Pushes integer n onto the top of the stack */
void do_push(int n)
{
  if (vm.sp == STACK_SIZE - 1)
  {
    fprintf(stderr, "STACK OVERFLOW: Stack is full\n");
    return;
  }

  vm.stack[vm.sp] = n;
  vm.sp++;
}

/* Removes the top entry in the stack and decrements stack pointer */
void do_pop(void)
{
  if (vm.sp == 0)
  {
    fprintf(stderr, "STACK UNDERFLOW: Stack is empty\n");
    return;
  }

  vm.sp--;
  vm.stack[vm.sp] = 0;
}

/* Pushes the value in regester n onto the top of the stack */
void do_load(int n)
{
  if (vm.sp == STACK_SIZE - 1)
  {
    fprintf(stderr, "STACK OVERFLOW: Stack is full\n");
    return;
  }
  if (n < 0)
  {
    fprintf(stderr, "ERROR: Register number must be non-negative\n");
    return;
  }
  if (n >= NREGS)
  {
    fprintf(stderr, "ERROR: Only 16 available registers\n");
    return;
  }

  vm.stack[vm.sp] = vm.reg[n];
  vm.sp++;
}

/* Puts the value of the top of the stack onto register n and pops from stack */
void do_store(int n)
{
  if (n < 0)
  {
    fprintf(stderr, "ERROR: Register number must be non-negative\n");
    return;
  }
  if (n >= NREGS)
  {
    fprintf(stderr, "ERROR: Only 16 available registers\n");
    return;
  }

  vm.reg[n] = vm.stack[vm.sp - 1];
  do_pop();
}

/* Moves instruction pointer to spot n in the instruction array */
void do_jmp(int n)
{
  if (n < 0)
  {
    fprintf(stderr, "ERROR: Instruction number must be non-negative\n");
    return;
  }
  if (n >= MAX_INSTS)
  {
    fprintf(stderr, "ERROR: Only 65536 available instructions\n");
    return;
  }

  vm.ip = n;
}

/*
Moves instruction pointer to spot n in the instruction array if zero is
on top of the stack
*/
void do_jz(int n)
{
  if (vm.stack[vm.sp - 1] == 0)
  {
    do_pop();
    do_jmp(n);
  }
  else
  {
    do_pop();
  }
}

/*
Moves instruction pointer to spot n in the instruction array if zero is not
on top of the stack
*/
void do_jnz(int n)
{
  if (vm.stack[vm.sp - 1] != 0)
  {
    do_pop();
    do_jmp(n);
  }
  else
  {
    do_pop();
  }
}

/* Pops the top two values on stack and replaces them with their sum */
void do_add(void)
{
  if (vm.sp == 1 || vm.sp == 0)
  {
    fprintf(stderr, "STACK UNDERFLOW: Fewer than two values on stack\n");
    return;
  }

  vm.sp--;
  vm.stack[vm.sp - 1] = vm.stack[vm.sp - 1] + vm.stack[vm.sp];
  vm.stack[vm.sp] = 0;
}

/* Pops the top two values on stack and replaces them with their difference */
void do_sub(void)
{
  if (vm.sp == 1 || vm.sp == 0)
  {
    fprintf(stderr, "STACK UNDERFLOW: Fewer than two values on stack\n");
    return;
  }

  vm.sp--;
  vm.stack[vm.sp - 1] = vm.stack[vm.sp - 1] - vm.stack[vm.sp];
  vm.stack[vm.sp] = 0;
}

/* Pops the top two values on stack and replaces them with their product */
void do_mul(void)
{
  if (vm.sp == 1 || vm.sp == 0)
  {
    fprintf(stderr, "STACK UNDERFLOW: Fewer than two values on stack\n");
    return;
  }

  vm.sp--;
  vm.stack[vm.sp - 1] = vm.stack[vm.sp - 1] * vm.stack[vm.sp];
  vm.stack[vm.sp] = 0;
}

/* Pops the top two values on stack and replaces them with their quotient */
void do_div(void)
{
  if (vm.sp == 1 || vm.sp == 0)
  {
    fprintf(stderr, "STACK UNDERFLOW: Fewer than two values on stack\n");
    return;
  }

  vm.sp--;
  vm.stack[vm.sp - 1] = vm.stack[vm.sp - 1] / vm.stack[vm.sp];
  vm.stack[vm.sp] = 0;
}

/* Prints the value on the top of the stack and pops from stack */
void do_print(void)
{
  if (vm.sp == 0)
  {
    fprintf(stderr, "STACK UNDERFLOW: Stack is empty\n");
    return;
  }

  printf("%d\n", vm.stack[vm.sp - 1]);
  do_pop();
}


/*
 * Stored program execution.
 */

/* Load the stored program into the VM. */
void load_program(FILE *fp)
{
    int nread;
    unsigned char *inst = vm.inst;

    do
    {
        /*
         * Read a single byte at a time and load it into the
         * 'vm.insts' array.  'fread' returns the number of bytes read,
         * or 0 if EOF is hit.
         */

        nread = fread(inst, 1, 1, fp);
        inst++;
    }
    while (nread > 0);
}



/* Execute the stored program in the VM. */
void execute_program(void)
{
    int val;

    vm.ip = 0;
    vm.sp = 0;

    while (1)
    {
        /*
         * Read each instruction and select what to do based on the
         * instruction.  For each instruction you may also have to
         * read in some number of bytes as the arguments to the
         * instruction.
         */

        switch (vm.inst[vm.ip])
        {
        case NOP:
            /* Skip to the next instruction. */
            vm.ip++;
            break;

        case PUSH:
            vm.ip++;

            /* Read in the next 4 bytes. */
            val = read_n_byte_integer(4);
            do_push(val);
            break;

        case POP:
            vm.ip++;

            do_pop();
            break;

        case LOAD:
            vm.ip++;

            /* Read in the next byte. */
            val = read_n_byte_integer(1);
            do_load(val);
            break;

        case STORE:
            vm.ip++;

            /* Read in the next byte. */
            val = read_n_byte_integer(1);
            do_store(val);
            break;

        case JMP:
            vm.ip++;

            /* Read in the next two bytes. */
            val = read_n_byte_integer(2);
            do_jmp(val);
            break;

        case JZ:
            vm.ip++;

            /* Read in the next two bytes. */
            val = read_n_byte_integer(2);
            do_jz(val);
            break;

        case JNZ:
            vm.ip++;

            /* Read in the next two bytes. */
            val = read_n_byte_integer(2);
            do_jnz(val);
            break;

        case ADD:
            vm.ip++;

            do_add();
            break;

        case SUB:
            vm.ip++;

            do_sub();
            break;

        case MUL:
            vm.ip++;

            do_mul();
            break;

        case DIV:
            vm.ip++;

            do_div();
            break;

        case PRINT:
            vm.ip++;

            do_print();
            break;

        case STOP:
            return;

        default:
            fprintf(stderr, "execute_program: invalid instruction: %x\n",
                    vm.inst[vm.ip]);
            fprintf(stderr, "\taborting program!\n");
            return;
        }
    }
}


/* Run the program given the file name in which it's stored. */
void run_program(char *filename)
{
    FILE *fp;

    /* Open the file containing the bytecode. */
    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "bci.c: run_program: "
               "error opening file %s; aborting.\n", filename);
        exit(1);
    }

    /* Initialize the virtual machine. */
    init_vm();

    /* Read the bytecode into the instruction buffer. */
    load_program(fp);

    /* Execute the program. */
    execute_program();

    /* Clean up. */
    fclose(fp);
}
