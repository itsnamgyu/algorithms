CC = gcc
CFLAGS = -g -Wall

.SUFFIXES: .c .o

SRC = input.c main.c mss.c

OBJDIR = obj
OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

TARGET = mss20161662

TEST = test

$(TARGET) : $(OBJ)
	$(CC) -o $@ $(OBJ)

$(OBJDIR)/%.o : %.c
	@mkdir -p $@D
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *test_input.txt
	rm -f result_*.txt
	rm -f $(TARGET)
	rm -rf $(OBJDIR)
	rm -f $(TEST)

test:
	gcc -D TEST -o test input.c
	@./$(TEST)
	gcc -D TEST -o test mss.c
	@./$(TEST)
	@echo "all tests successful"
	@rm -f $(TEST)
