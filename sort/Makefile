CC = gcc
CFLAGS = -g -Wall -std=c99

.SUFFIXES: .c .o

SRC_DIR = src
OBJ_DIR = obj

SRC = 
TEST_SRC = 

_SRC = $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))
_TEST_SRC = $(patsubst %.c, $(OBJDIR)/%.o, $(TEST_SRC))
_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))

TARGET = mp2_20161662

$(TARGET) : $(_OBJ)
	$(CC) -o $@ $(_OBJ)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $@D
	$(CC) $(CFLAGS) -c $< -o $@

test:
	TOTAL = 0
	SUCCESS = 0
	
	@for S in $(_TEST_SRC); do \
		echo "Testing $$S..."; \
		gcc -D TEST -o test $$S; \
		./test; \
		if [ $? == 0 ]; then
			(( SUCCESS+=1 ))
		fi
		(( TOTAL+=1 ))
			
	done;
	@echo "$(SUCCESS) of $(TOTAL) tests successful."
	@rm -f test
