#! /usr/bin/env python

"""
bca: an assembler for the VM byte-code.  It converts files written in the
     VM assembly language (which end in .bca) to files in the VM
     machine language (which end in .bcm).
"""

import sys, string, re, types, struct

usage_str = "usage: bca filename"

if len(sys.argv) != 2:
    print >> sys.stderr, usage_str
    sys.exit(1)

#
# Definitions.
#

NREGS = 16   # Number of registers.

#
# Each operation is described by:
# -- the operation name
# -- the byte code
# -- the number of additional bytes required for the instruction
#

ops = {"NOP":   (0x00, 0),
       "PUSH":  (0x01, 4),
       "POP":   (0x02, 0),
       "LOAD":  (0x03, 1),
       "STORE": (0x04, 1),
       "JMP":   (0x05, 2),
       "JZ":    (0x06, 2),
       "JNZ":   (0x07, 2),
       "ADD":   (0x08, 0),
       "SUB":   (0x09, 0),
       "MUL":   (0x0a, 0),
       "DIV":   (0x0b, 0),
       "PRINT": (0x0c, 0),
       "STOP":  (0x0d, 0)}


def check_op(op):
    if not ops.has_key(op):
        print >> sys.stderr, "Invalid opcode: %s" % op
        sys.exit(1)


#
# Read in all the lines of the file.
# Each line in the file has the format:
# [label] operation [argument]
# where [label] and [argument] are integers, and [operation] is a string.
# Labels and arguments are optional, although many operations require
# arguments.  Labels do not correspond to line numbers, but are just
# arbitrary numbers.
#

infilename  = sys.argv[1]
infile      = open(infilename, "r")

# The output filename is the same as the input filename, with the ".bca"
# suffix (for "byte code assembler") replaced by ".bcm" (for "byte code
# machine" code).
outfilename = re.sub("\.bca$", ".bcm", infilename)
outfile     = open(outfilename, "w")

# Get all the lines in the input file.
lines = infile.readlines()


#
# Read the lines into the instruction array.
#

instructions = []

for line in lines:
    # Strip away newlines.
    line = line[:-1]

    # Strip away comments.
    line = re.sub("#.*$", "", line)

    # Skip empty lines.
    if len(line) == 0:
        continue

    # Convert to uppercase.
    line = line.upper()

    # Extract fields from the line.
    words = line.split()

    # Convert strings representing integers to integers.
    # Also check the validity of the operations by doing
    # numeric conversions where needed.
    if len(words) == 3:
        # The instruction is of the form: label op arg
        label, op, arg = words
        label = string.atoi(label)
        arg   = string.atoi(arg)
        check_op(op)
        inst = (label, op, arg)
    elif len(words) == 2:
        # The instruction is in one of two forms:
        #
        # 1) op arg
        # 2) label op
        #
        if words[0] in ops.keys():
            # Must be: op arg
            op, arg = words
            arg = string.atoi(arg)
            check_op(op)
            inst = (op, arg)
        else:
            # Must be: label op
            label, op = words
            label = string.atoi(label)
            check_op(op)
            inst = (label, op)
    elif len(words) == 1:
        # The instruction is of the form: op
        op = words[0]
        check_op(op)
        inst = (op,)
    else:
        print >> sys.stderr, "invalid line: %s" % line
        sys.exit(1)

    # Store these in the instruction array.
    instructions.append(inst)

#print instructions

#
# Make one pass through the code to find the label->instruction mapping.
#

labels = {}
count  = 0

for inst in instructions:
    if len(inst) == 3:
        # The instruction is of the form: label op arg
        label, op, arg = inst
        # Store the location of the instruction.
        labels[label] = count
    elif len(inst) == 2:
        # The instruction is in one of two forms:
        #
        # 1) op arg
        # 2) label op
        #
        # We distinguish these by their types.
        if type(inst[0]) == types.IntType:
            # Must be: label op
            label, op = inst
            # Store the location of the instruction.
            labels[label] = count
        else:
            # Must be: op arg
            op, arg = inst
    else:
        # The instruction is of the form: op
        op = inst[0]

    # Increment the counter, adding 1 for the opcode and another
    # value for the size of the argument (if any).
    #print op, ops[op]
    opcode, incr = ops[op]
    incr  += 1
    count += incr

#print labels


#
# Make another pass to convert the code to byte code.
#

# Helper functions.
def write_single_opcode(bytecode, op):
    # Just write out the bytecode corresponding to 'op'.
    opcode, incr = ops[op]

    # Error checking.
    assert incr == 0

    # Write out the byte code.
    return bytecode + chr(opcode)


def write_full_instruction(bytecode, op, arg):
    # Write out the bytecode corresponding to 'op' as well as
    # the argument, which can be 1, 2, or 4 bytes long.
    opcode, incr = ops[op]

    # Error checking.
    assert incr == 1 or incr == 2 or incr == 4

    # Write out the bytecode.
    bytecode += chr(opcode)

    if incr == 1:
        # Argument is a register.  Convert to an unsigned byte.
        if arg < 0 or arg >= NREGS:
            # The code tried to access a non-existent register.
            print >> sys.stderr, "register %d is invalid" % arg
            sys.exit(1)
        bytecode += struct.pack("B", arg)
    elif incr == 2:
        # Argument is a label.  Find the corresponding instruction
        # address and convert it to an unsigned short.
        addr = labels[arg]
        bytecode += struct.pack("H", addr)
    else:  # 4
        # Argument is a signed integer.
        bytecode += struct.pack("i", arg)

    return bytecode


bytecode = ""

for inst in instructions:
    if len(inst) == 3:
        # The instruction is of the form: label op arg
        label, op, arg = inst
        bytecode = write_full_instruction(bytecode, op, arg)
    elif len(inst) == 2:
        # The instruction is in one of two forms:
        #
        # 1) op arg
        # 2) label op
        #
        # We distinguish these by their types.
        if type(inst[0]) == types.IntType:
            # Must be: label op
            label, op = inst
            bytecode = write_single_opcode(bytecode, op)
        else:
            # Must be: op arg
            op, arg = inst
            bytecode = write_full_instruction(bytecode, op, arg)
    else:
        # The instruction is of the form: op
        op = inst[0]
        bytecode = write_single_opcode(bytecode, op)

#
# Write out the byte code to a file.
#

for byte in bytecode:
    outfile.write(byte)

infile.close()
outfile.close()
