.PHONY: default

CC = g++
STD = -std=c++11
CFLAGS = -g -Wall $(STD)

.SUFFIXES: .cpp .o

SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj

SRC_MAIN = main.cpp  # in SRC
SRC_REQ = code.cpp data.cpp  # in SRC
SRC_TEST = catch.cpp data.cpp  # in TEST

_SRC_MAIN = $(patsubst %.cpp, $(SRC_DIR)/%.cpp, $(SRC_MAIN))
_SRC_REQ = $(patsubst %.cpp, $(SRC_DIR)/%.cpp, $(SRC_REQ))
_SRC_TEST = $(patsubst %.cpp, $(TEST_DIR)/%.cpp, $(SRC_TEST))

_OBJ_MAIN = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC_MAIN))
_OBJ_REQ= $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC_REQ))
_OBJ_TEST = $(patsubst %.cpp, $(OBJ_DIR)/%_test.o, $(SRC_TEST))

_MAIN_OBJ = $(_OBJ_MAIN) $(_OBJ_REQ)
_TEST_OBJ = $(_OBJ_TEST) $(_OBJ_REQ)

TARGET = mp3_20161662

default : $(TARGET)

$(TARGET) : $(_MAIN_OBJ)
	$(CC) -o $@ $(_MAIN_OBJ)

tester : $(_TEST_OBJ)
	$(CC) -o $@ $(_TEST_OBJ)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $@D
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%_test.o : $(TEST_DIR)/%.cpp
	@mkdir -p $@D
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)

cleanall:
	$(MAKE) clean
	rm -rf obj
