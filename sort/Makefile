CC = g++
STD = -std=c++11
CFLAGS = -g -Wall $(STD)

.SUFFIXES: .cpp .o

SRC_DIR = src
OBJ_DIR = obj

SRC = none
TEST_SRC = data.cpp sort.cpp

_SRC = $(patsubst %.cpp, $(SRC_DIR)/%.cpp, $(SRC))
_TEST_SRC = $(patsubst %.cpp, $(SRC_DIR)/%.cpp, $(TEST_SRC))
_OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

TARGET = mp2_20161662

$(TARGET) : $(_OBJ)
	$(CC) -o $@ $(_OBJ)

debug: 

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $@D
	$(CC) $(CFLAGS) -c $< -o $@

test:
	@TOTAL=0
	@SUCCESS=0
	@for S in $(_TEST_SRC); do \
		echo "Testing $$S..."; \
		$(CC) $(STD) -D TEST -o test $$S; \
		./test; \
	done;
	@echo "All tests successful!"
	@rm -f test
