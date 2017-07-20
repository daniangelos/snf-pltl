BISON=bison
BISONFLAGS=-v -d
FLEX=flex

CC=gcc

#LIBS=-lm -lpthread
#EXTRALIBS=-ltcmalloc

#production

CFLAGS=-O3 -Wall
LFLAGS=-O3
PFLAGS=-O3

# debugging and testing

#CFLAGS=-Wall -g -O0
#LFLAGS=-Wall -g -O0
#PFLAGS=-g -O0

SRCS=helpers.c snf.c clauses.c renaming.c simpl.c main.c
OBJS = $(SRCS:.c=.o)

PARSERC = lex.yy.c parser.tab.c
PARSEROBJS = lex.yy.o parser.tab.o

TARGET=pltl

all:	$(TARGET)
	cp $(TARGET) ../.

$(TARGET): $(PARSEROBJS) $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(PARSEROBJS) $(OBJS) $(LFLAGS) #$(LIBS) $(EXTRALIBS)


lex.yy.o: $(PARSERC)
	$(CC) -c $(PFLAGS) lex.yy.c -o lex.yy.o

parser.tab.o: $(PARSERC)
	$(CC) -c $(CFLAGS) parser.tab.c -o parser.tab.o

parser.tab.c:
	$(BISON) $(BISONFLAGS) parser.y

lex.yy.c:
	$(FLEX) tokenizer.l

.c.o:	
	$(CC) $(CFLAGS) -c $<  -o $@

clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f ../$(TARGET)

clean_parser:
	rm -f lex.yy.c parser.tab.c
	rm -f lex.yy.o parser.tab.o
	rm -f parser.tab.h

clean_all: clean clean_parser
