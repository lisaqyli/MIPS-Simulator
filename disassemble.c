#include <stdio.h>
#include <stdlib.h>

#include "disassemble.h"

void disassemble(inst_t inst)
{
  /* you could use itype or type instead of rtype */
  /* remember that opcode field is at the same place for all types */
  switch (inst.rtype.opcode)
  {
  case 0x0: // opcode == 0x0 (SPECIAL)

    switch (inst.rtype.funct)
    {
    case 0xc: // funct == 0xc (SYSCALL)
      printf("syscall\n");
      break;
    
    case 0x24: // funct == 0x24 (AND)
      printf("and\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
      break;      

    case 0x25: // funct == 0x25 (OR)
      printf("or\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
      break;
    case 0x0: //fucnt == 0x0 (SLL)
      printf("sll\t$%d,$%d,%d\n", inst.rtype.rd, inst.rtype.rt, inst.rtype.shamt);
      break;
    case 0x2: // funct == 0x2 (SRL)
      printf("srl\t$%d,$%d,%d\n", inst.rtype.rd, inst.rtype.rt, inst.rtype.shamt);
      break;
    case 0x3: // funct == 0x3 (SRA)
      printf("sra\t$%d, $%d, $%d\n", inst.rtype.rd, inst.rtype.rt, inst.rtype.shamt);
      break;
    case 0x8: //funct == 0x8 (JR)
      printf("jr\t$%d\n", inst.jtype.addr);
      break;
    case 0x9: //funct == 0x9 (JALR)
      printf("jalr\t$%d\n", inst.jtype.addr);
      break;
    case 0x20: //funct == 0x20 (ADD)
      printf("add\t$%d, $%d, $%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
      break;
    case 0x21: //funct == 0x21 (ADDU)
      printf("addu\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
      break;
    case 0x26: //funct == 0x26 (XOR)
      printf("xor\t$%d, $%d, $%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
      break;
    case 0x2a: //funct == 0x2a (SLT)
      printf("slt\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
      break;

    default: // undefined funct
      fprintf(stderr, "%s: illegal instruction: %08x\n", __FUNCTION__, inst.bits);
      exit(-1);
      break;
    }
    break;

  case 0xd: // opcode == 0xd (ORI)
    printf("ori\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
    break;

  case 0x2: // opcode == 0x2 (J)
    printf("j\t0x%x\n", inst.jtype.addr*4);
    break;

  case 0x3: //opcode == 0x3 (JAL)
    printf("jal\t0x%x\n", inst.jtype.addr*4);
    break;

  case 0x4: //opcode == 0x4 (BEQ)
    printf("beq\t$%d,$%d,%d\n", inst.itype.rs, inst.itype.rt, inst.itype.imm*4);
    break;

  case 0x5: //funct == 0x5 (BNE)
    printf("bne\t$%d,$%d,%d\n", inst.itype.rs, inst.itype.rt, inst.itype.imm*4);
    break;

  case 0x9: //funct == 0x9 (ADDIU)
   printf("addiu\t$%d,$%d,%d\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
   break;

  case 0xe: //funct == 0xe (XORI)
    printf("xori\t$%d, $%d, 0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
    break;

  case 0xf: //funct == 0xf (LUI)
    printf("lui\t$%d,0x%x\n", inst.itype.rt, inst.itype.imm);
    break;

  case 0x20: //funct == 0x20 (LB)
    printf("lb\t$%d, 0x%x\n", inst.itype.rt, inst.itype.imm);
    break;

  case 0x23: //funct == 0x23 (LW)
    printf("lw\t$%d,0($%d)\n", inst.itype.rt, inst.itype.imm+4);
    break;

  case 0x24: //funct == 0x24 (LBU)
    printf("lbu\t$%d, 0x%x\n", inst.itype.rt, inst.itype.imm);
    break;

  case 0x28: //funct == 0x28 (SB)
    printf("sb\t$%d, 0x%x\n", inst.itype.rt, inst.itype.imm);
    break;

  case 0x2b: // funct == (SW)
    printf("sw\t$%d,0($%d)\n", inst.itype.rt, inst.itype.imm+4);
    break;

  default: // undefined opcode
    fprintf(stderr, "%s: illegal instruction: %08x\n", __FUNCTION__, inst.bits);
    exit(-1);
    break;
  }
}
