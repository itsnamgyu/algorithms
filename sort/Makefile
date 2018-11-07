CC = g++
CFLAGS = -g -Wall -std=c++11

.SUFFIXES: .cpp .o

SRC_DIR = src
OBJ_DIR = obj

SRC = 
TEST_SRC = 

_SRC = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRC))
_TEST_SRC = $(patsubst %.cpp, $(OBJDIR)/%.o, $(TEST_SRC))
_OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

TARGET = mp2_20161662

$(TARGET) : $(_OBJ)
	$(CC) -o $@ $(_OBJ)

debug: 

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
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
