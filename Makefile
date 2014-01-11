CC = cc
CCFLAGS = -O2 -Wall -ggdb -fPIC -fpic

MY_EXAM = exam

MY_OBJS = exam.o main.o

all:$(MY_EXAM)

$(MY_EXAM):$(MY_OBJS)
	$(CC) -o $@ $(MY_OBJS)

clean:
	rm -rf $(MY_OBJS) 
	rm -rf $(MY_EXAM)

.PRECIOUS:%.c
.SUFFIXES:
.SUFFIXES:.c .o
.c.o:
	$(CC) $(CCFLAGS) -c -o $*.o $<

