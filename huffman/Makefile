.PHONY: default

CC = g++
STD = -std=c++11
CFLAGS = -g -Wall $(STD)

.SUFFIXES: .cpp .o

SRC_DIR = src
OBJ_DIR = obj

#SRC = main.cpp data.cpp
SRC = 

_SRC = $(patsubst %.cpp, $(SRC_DIR)/%.cpp, $(SRC))
_OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

TARGET = mp3_20161662

default : $(TARGET)

$(TARGET) : $(_OBJ)
	$(CC) -o $@ $(_OBJ)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $@D
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r $(TARGET)
	rm -rf obj
