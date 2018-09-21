CC = gcc
CFLAGS = -g -Wall

.SUFFIXES: .c .o

SRC = src/input.c src/main.c src/mss.c
GEN_SRC = src/input.c src/generate.c

GEN_DIR = analysis

OBJDIR = obj
OBJ = $(patsubst src/%.c, $(OBJDIR)/%.o, $(SRC))
GEN_OBJ = $(patsubst src/%.c, $(OBJDIR)/%.o, $(GEN_SRC))

TARGET = mss20161662

TEST = test

$(TARGET) : $(OBJ)
	$(CC) -o $@ $(OBJ)

generate : $(GEN_OBJ)
	$(CC) -o generator $(GEN_OBJ)
	./generator
	mkdir -p $(GEN_DIR)
	mv input* $(GEN_DIR)
	rm generator

$(OBJDIR)/%.o : src/%.c
	@mkdir -p $@D
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *test_input.txt
	rm -f result_*.txt
	rm -f $(TARGET)
	rm -rf $(OBJDIR)
	rm -f $(TEST)

utest:
	gcc -D TEST -o test src/input.c
	@./$(TEST)
	gcc -D TEST -o test src/mss.c
	@./$(TEST)
	@echo "all tests successful"
	@rm -f $(TEST)
