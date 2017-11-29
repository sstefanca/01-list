CFLAGS=-Wall -Werror -ggdb
LDFLAGS=-pthread
FILES=main list

OBJECTS=$(addsuffix .o , $(FILES))

all: bin

bin: $(OBJECTS)
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm -fv bin $(OBJECTS) *~

test_variables:
	echo $(FILES)
	echo $(OBJECTS)

.PHONY: all clean test_variables
