CC = gcc
CFLAGS = -g -Wall

.SUFFIXES: .c .o

SRC = src/input.c src/main.c src/mss.c
TEST_SRC = src/input.c src/mss.c
GEN_SRC = src/input.c src/generate.c

OBJDIR = obj
OBJ = $(patsubst src/%.c, $(OBJDIR)/%.o, $(SRC))
GEN_OBJ = $(patsubst src/%.c, $(OBJDIR)/%.o, $(GEN_SRC))

TARGET = mss20161662

$(TARGET) : $(OBJ)
	$(CC) -o $@ $(OBJ)

generator : $(GEN_OBJ)
	$(CC) -o $@ $(GEN_OBJ)

generate : generator
	./generator
	mkdir -p analysis
	mv input* analysis
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
	rm -f analysis/input*
	rm -f generator

unittest:
	@for S in $(TEST_SRC); do \
		echo "Testing $$S..."; \
		gcc -D TEST -o test $$S; \
		./test; \
	done;
	@echo "All tests successful."
	@rm -f test
