#ifndef _MOS_LINKAGE_H
#define _MOS_LINKAGE_H

#define SYMBOL_NAME_STR(X) #X
#define SYMBOL_NAME(X) X

#define ENTRY(name) \
  .globl SYMBOL_NAME(name); \
  ALIGN; \
  SYMBOL_NAME_LABEL(name)

#endif
