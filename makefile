CC		= gcc
LD		= gcc
PROG_NAME	= Assignment_2

SRC_DIR		=./src
INC_DIR		=./inc
OBJ_DIR		=./build
BIN_DIR		=./bin
INC_LIST  = $(wildcard $(INC_DIR)/*.h)
SRC_LIST	= $(wildcard $(SRC_DIR)/*.c)
OBJ_LIST	= $(wildcard $(OBJ_DIR)/*.o)


.PHONY : all clean $(PROG_NAME) compile

all: $(PROG_NAME)

compile:
	$(CC) $(INC_LIST) $(SRC_LIST) -I $(INC_DIR) -c

copy: compile
	@mv $(notdir $(SRC_LIST:.c=.o)) $(OBJ_DIR)/

$(PROG_NAME): copy
	$(LD) $(OBJ_LIST) -o $(BIN_DIR)/$@

clean:
	rm -f $(BIN_DIR)/$(PROG_NAME) $(OBJ_DIR)/*.o
