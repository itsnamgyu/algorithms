CC = g++
STD = -std=c++11
CFLAGS = -g -Wall $(STD)

.SUFFIXES: .cpp .o

HARDSORT_MAX = 32
SRC_DIR = src
OBJ_DIR = obj

SRC = main.cpp data.cpp sort.cpp insertion_sort.cpp merge_sort.cpp quick_sort.cpp heap_sort.cpp bucket_sort.cpp
ANALYSIS_SRC = analyze.cpp data.cpp sort.cpp insertion_sort.cpp merge_sort.cpp quick_sort.cpp heap_sort.cpp bucket_sort.cpp
TEST_SRC = data.cpp sort.cpp insertion_sort.cpp merge_sort.cpp quick_sort.cpp heap_sort.cpp bucket_sort.cpp

_SRC = $(patsubst %.cpp, $(SRC_DIR)/%.cpp, $(SRC))
_TEST_SRC = $(patsubst %.cpp, $(SRC_DIR)/%.cpp, $(TEST_SRC))
_ANALYSIS_SRC = $(patsubst %.cpp, $(SRC_DIR)/%.cpp, $(ANALYSIS_SRC))
_OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))
_ANALYSIS_OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(ANALYSIS_SRC))
_DEPENDANCY_OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(DEPENDANCY_SRC))

TARGET = mp2_20161662

$(TARGET) : $(_OBJ)
	$(CC) -o $@ $(_OBJ)

analyze : $(_ANALYSIS_OBJ)
	$(CC) -o $@ $(_ANALYSIS_OBJ)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@mkdir -p $@D
	$(CC) $(CFLAGS) -c $< -o $@

test:
	@TOTAL=0
	@SUCCESS=0
	@for S in $(_TEST_SRC); do \
		echo "Testing $$S..."; \
		$(CC) $(STD) -D TEST -o test_exec $$S; \
		./test_exec; \
	done;
	@echo "All tests successful!"
	@rm -f test_exec

clean:
	rm -rf obj
	rm -f test_data.txt
	rm -f input.txt
	rm -f test_exec
	rm -f analyze
	rm -f analyze_all
