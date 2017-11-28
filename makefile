CFLAGS=-Wall -Werror -ggdb
LDFLAGS=-pthread
FILES=main
OBJECTS=$(addsuffix .o , $(FILES))

all: bin

bin: $(OBJECTS)
	gcc -o $@ $^ $(LDFLAGS)

test_variables:
	echo $(FILES)
	echo $(OBJECTS)
