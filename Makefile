SRC = $(shell find . -name "*.c")
OBJ = $(addsuffix .out, $(basename $(SRC)))

$(info Making $(OBJ))

.PHONY: target

target: $(OBJ)

%.out:%.c
	@gcc -o $@ $^
