MODULES = 
BOARD ?= edu-ciaa-nxp
MUJU ?= ./muju
CFLAGS := -Wall -Werror

include $(MUJU)/module/base/makefile
