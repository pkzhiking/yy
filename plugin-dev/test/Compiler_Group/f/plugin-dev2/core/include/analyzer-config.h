/*
 *  analyzer-config.h
 *
 *  Created on: Apr 2, 2012
 *      Author: Guo Jiuliang
 *  Description: Declarations for GCC internal usage
 */

#define FIRST_PSEUDO_REGISTER 53
enum reg_class
{
  NO_REGS,
  AREG, DREG, CREG, BREG, SIREG, DIREG,
  AD_REGS,			/* %eax/%edx for DImode */
  CLOBBERED_REGS,		/* call-clobbered integers */
  Q_REGS,			/* %eax %ebx %ecx %edx */
  NON_Q_REGS,			/* %esi %edi %ebp %esp */
  INDEX_REGS,			/* %eax %ebx %ecx %edx %esi %edi %ebp */
  LEGACY_REGS,			/* %eax %ebx %ecx %edx %esi %edi %ebp %esp */
  GENERAL_REGS,			/* %eax %ebx %ecx %edx %esi %edi %ebp %esp
				   %r8 %r9 %r10 %r11 %r12 %r13 %r14 %r15 */
  FP_TOP_REG, FP_SECOND_REG,	/* %st(0) %st(1) */
  FLOAT_REGS,
  SSE_FIRST_REG,
  SSE_REGS,
  MMX_REGS,
  FP_TOP_SSE_REGS,
  FP_SECOND_SSE_REGS,
  FLOAT_SSE_REGS,
  FLOAT_INT_REGS,
  INT_SSE_REGS,
  FLOAT_INT_SSE_REGS,
  ALL_REGS, LIM_REG_CLASSES
};
/* Available call abi.  */
enum calling_abi
{
  SYSV_ABI = 0,
  MS_ABI = 1
};
typedef struct ix86_args {
  int words;			/* # words passed so far */
  int nregs;			/* # registers available for passing */
  int regno;			/* next available register number */
  int fastcall;			/* fastcall or thiscall calling convention
				   is used */
  int sse_words;		/* # sse words passed so far */
  int sse_nregs;		/* # sse registers available for passing */
  int warn_avx;			/* True when we want to warn about AVX ABI.  */
  int warn_sse;			/* True when we want to warn about SSE ABI.  */
  int warn_mmx;			/* True when we want to warn about MMX ABI.  */
  int sse_regno;		/* next available sse register number */
  int mmx_words;		/* # mmx words passed so far */
  int mmx_nregs;		/* # mmx registers available for passing */
  int mmx_regno;		/* next available mmx register number */
  int maybe_vaarg;		/* true for calls to possibly vardic fncts.  */
  int caller;			/* true if it is caller.  */
  int float_in_sse;		/* Set to 1 or 2 for 32bit targets if
				   SFmode/DFmode arguments should be passed
				   in SSE registers.  Otherwise 0.  */
  enum calling_abi call_abi;	/* Set to SYSV_ABI for sysv abi. Otherwise
 				   MS_ABI for ms abi.  */
} CUMULATIVE_ARGS;
#define N_REG_CLASSES ((int) LIM_REG_CLASSES)
//DEGUG CONTROL
//#define PLUGIN_DEGUG 1
#define LOGGER_ON 1
#define LOGGER_ON 1
# define _analyzer_assert(expr,msg) \
    if (!(expr)) { fprintf(stderr, "%s:%d analyzer assertion failure (msg:%s): %s\n", \
			  __FILE__, __LINE__,msg,# expr); abort(); }
//#define PLUGIN_DEBUG 1
