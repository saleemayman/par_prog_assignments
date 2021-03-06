NAME = loop_fission
CC = gcc
CFLAGS = -std=gnu99 -fopenmp -fstrict-overflow -Werror -Wall -Wshadow  -pedantic
LDFLAGS = -lm -lrt  

#Compiler flags
#if mode variable is empty, setting release build mode
ifeq ($(mode),debug)
	CFLAGS += -ggdb
else
	mode = release
	CFLAGS += -O2
endif

SRC = $(wildcard *.c)

OBJ_SEQ = main.o $(NAME)_seq.o helper.o
OBJ_PAR = main.o $(NAME)_par.o helper.o
OBJ_UNIT = unit_test.o $(NAME)_par.o $(NAME)_ref.o helper.o

all: sequential parallel

sequential: $(NAME)_seq

parallel: $(NAME)_par

$(NAME)_seq : $(OBJ_SEQ)
	$(CC) -o $@ $(OBJ_SEQ) $(CFLAGS) $(LDFLAGS)

$(NAME)_par : $(OBJ_PAR)
	$(CC) -o $@ $(OBJ_PAR) $(CFLAGS) $(LDFLAGS)

unit_test: $(OBJ_UNIT)
	$(CC) -o $@ $(OBJ_UNIT) $(CFLAGS) $(LDFLAGS)
 
%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	rm -f $(NAME)_seq $(NAME)_par unit_test *.o *.d
	
-include $(SRC:.c=.d)

.PHONY : all sequential parallel clean
