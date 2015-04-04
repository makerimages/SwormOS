# Makefile for the IA-32 arch-dependent source files.

ARCH_SRC := $(wildcard $(ARCHDIR)/*.[sc] $(ARCHDIR)/*/*.[sc])
ARCH_SRC += $(wildcard $(ARCHDIR)/*.cc $(ARCHDIR)/*/*.cc)

ARCH_OBJ := $(subst .cc,.o,$(ARCH_SRC))
ARCH_OBJ := $(subst .c,.o,$(ARCH_OBJ))
ARCH_OBJ := $(subst .s,.o,$(ARCH_OBJ))
