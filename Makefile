SRC = $(shell find . -name "*.c")
OBJ = $(addsuffix .out, $(basename $(SRC)))

$(info Making $(OBJ))

.PHONY: target clean

target: $(OBJ)

clean:
	-rm $$(find . -name "*.out")

%.out:%.c
	@gcc -o $@ $^
