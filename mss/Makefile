CC = gcc
CFLAGS = -g -Wall

.SUFFIXES: .c .o

SRC = src/input.c src/main.c src/mss.c
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

utest:
	gcc -D TEST -o test src/input.c
	@./test
	gcc -D TEST -o test src/mss.c
	@./test
	@echo "all tests successful"
	@rm -f test
