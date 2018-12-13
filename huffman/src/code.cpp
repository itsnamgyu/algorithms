#include "../include/code.hpp"
#include <cassert>
#include <cstring>
#include <algorithm>
#include <queue>


Symbol::Symbol(int code_length, BitSequence &symbol) :
	code_length(code_length),
	symbol(symbol.to_primitive()),
	size(symbol.size())
{}

Symbol::Symbol(int code_length, uint symbol, int size) :
	code_length(code_length),
	symbol(symbol),
	size(size)
{}

BitSequence Symbol::to_bits() const {
	return BitSequence(symbol, size);
}


bool Symbol::operator==(const Symbol &other) const {
	return other.code_length == code_length &&
		other.symbol == symbol &&
		other.size == size;
}

static bool symbol_comp(const Symbol &left, const Symbol &right) {
	if (left.code_length == right.code_length)
		return left.symbol < right.symbol;
	return left.code_length < right.code_length;
}

PrefixTree::PrefixTree() {
	children[0] = nullptr;
	children[1] = nullptr;
}

PrefixTree::PrefixTree(BitSequence symbol, int frequency) :
	symbol(symbol),
	frequency(frequency)
{
	children[0] = nullptr;
	children[1] = nullptr;
}

PrefixTree::PrefixTree(const Symbol &symbol) :
	symbol(symbol.to_bits()),
	frequency(0)
{
	children[0] = nullptr;
	children[1] = nullptr;
}

struct compare {
	// greater -> min heap pqueue
    bool operator()(const PrefixTree *left, const PrefixTree *right) const {
		return left->frequency > right->frequency;
    }
};

PrefixTree *PrefixTree::from_content(BitSequence &bits, int symbol_size) {
	assert(0 <= symbol_size && symbol_size <= 32);
	//assert(symbol_size == 4 || symbol_size == 8 || symbol_size == 16);

	unsigned long long count = 1 << symbol_size;
	auto frequencies = std::vector<int>(count, 0);

	// get BitSequence from string with redundant 0 bits such that
	// total size if multiple of `symbol_size`.
	int appended = 0;
	while (bits.size() % symbol_size != 0) {
		bits.append(0);
		appended++;
	}

	// count symbols
	std::vector<uint> symbols = bits.to_primitives(symbol_size);
	for (const auto &symbol : symbols)
		frequencies[symbol] += 1;

	// remove alignment bits
	while (appended--) bits.data.pop_back();

	// initialize pqueue with single-node trees
	std::priority_queue<PrefixTree*, std::vector<PrefixTree*>, compare> trees;
	for (uint i = 0; i < count; ++i)
		if (frequencies[i])
			trees.push(new PrefixTree(BitSequence(i, symbol_size),
						frequencies[i]));

	if (trees.size() == 1) {
		// single symbol codes don't work
		// hotfix w/ a null symbol
		uint symbol = trees.top()->symbol.to_primitive();
		symbol = symbol ^ 1;
		trees.push(new PrefixTree(BitSequence(symbol, symbol_size), 1));
	}

	/*
	while (!trees.empty()) {
		if (isprint(trees.top()->symbol.to_primitive())) {
			printf("%10c: %4d\n", (char) trees.top()->symbol.to_primitive(),
					trees.top()->frequency);
		} else {
			printf("0X%8X: %4d\n", (char) trees.top()->symbol.to_primitive(),
					trees.top()->frequency);
		}
		trees.pop();
	}
	exit(-1);
	*/

	// merge trees
	while (trees.size() > 1) {
		PrefixTree *parent = new PrefixTree();
		PrefixTree *left = trees.top();
		trees.pop();
		PrefixTree *right = trees.top();
		trees.pop();

		parent->children[0] = left;
		parent->children[1] = right;
		parent->frequency = left->frequency + right->frequency;
		trees.push(parent);
	}

	return trees.top();
}

PrefixTree::~PrefixTree() {
	if (children[0] != nullptr) delete children[0];
	if (children[1] != nullptr) delete children[1];
}

BitSequence PrefixTree::decode(BitSequence &bits) const {
	auto decoded = BitSequence();
	const PrefixTree *tree = this;
	for (int i = 0; i < bits.size(); ++i) {
		uchar bit = bits[i];
		tree = tree->children[bit];

		// leaf node: add symbol and reset tree node
		if (tree->children[0] == nullptr) {
			assert(tree->children[1] == nullptr);
			decoded.append(tree->symbol);
			tree = this;
		}
	}
    return decoded;
}


CodeBook::CodeBook() = default;

CodeBook::CodeBook(PrefixTree *tree) {
	std::queue<PrefixTree*> bfs_queue;

	// nullptr indicates increase in depth level
	bfs_queue.push(tree);
	bfs_queue.push(nullptr);
	int level = 0;

	// bfs search (track level)
	int non_null = 1;
	while (non_null) {
		PrefixTree *tree = bfs_queue.front();
		if (tree == nullptr) {  // next level
			bfs_queue.push(nullptr);
			level += 1;
		} else {
			non_null -= 1;
			if (tree->children[0] == nullptr) {
				// leaf node
				assert(tree->children[1] == nullptr);
				symbols.emplace_back(level, tree->symbol);
			} else {
				bfs_queue.push(tree->children[0]);
				bfs_queue.push(tree->children[1]);
				non_null += 2;
			}
		}
		bfs_queue.pop();
	}

	assert(bfs_queue.front() == nullptr);
	std::sort(symbols.begin(), symbols.end(), symbol_comp);

	/*
	for (const auto &symbol : symbols) {
		printf("symbol [ %3c ]: %3d\n", symbol.symbol, symbol.code_length);
	}
	exit(-1);
	*/

}

PrefixTree *CodeBook::to_canonical_prefix_tree() {
	assert (symbols.size() != 0);  // when?
	if (symbols.size() == 1) {
		return new PrefixTree(symbols.front());
	} else  {
		PrefixTree *head = new PrefixTree();
		// bfs-ish
		std::queue<PrefixTree**> bfs_queue;

		// nullptr indicates increase in depth level
		bfs_queue.push(&head->children[0]);
		bfs_queue.push(&head->children[1]);
		bfs_queue.push(nullptr);
		int level = 1;

		// bfs search (track level)
		int index = 0;
		int non_null = 2;
		while (non_null) {
			PrefixTree **tree = bfs_queue.front();
			bfs_queue.pop();
			if (tree == nullptr) {  // next level
				level += 1;
				bfs_queue.push(nullptr);
			} else {
				non_null -= 1;
				if (symbols[index].code_length == level) {
					auto bits = symbols[index].to_bits();
					*tree = new PrefixTree(bits, -1);
					index++;
				} else {
					*tree = new PrefixTree();
					bfs_queue.push(&(*tree)->children[0]);
					bfs_queue.push(&(*tree)->children[1]);
					non_null += 2;
				}
			}
		}

		assert(bfs_queue.front() == nullptr);
		assert(index == symbols.size());
		return head;
	}
}

CodeBook CodeBook::from_code_length_list(
		BitSequence &bits, int length_size) {
	auto book = CodeBook();
	assert(bits.size() % length_size == 0);

	int symbol_count = bits.size() / length_size;
	int symbol_size = 0;
	for (int i = 0; i < 32; ++i)
		if (1 << i == symbol_count)
			symbol_size = i;
	assert(0 < symbol_size);
	assert(symbol_size <= 31);

	std::vector<uint> lengths = bits.to_primitives(length_size);
	for (uint i = 0; i < symbol_count; ++i) {
		if (lengths[i])
			book.symbols.push_back(Symbol(lengths[i], i, symbol_size));
	}
	// TODO

	std::sort(book.symbols.begin(), book.symbols.end(), symbol_comp);

    return book;
}

BitSequence CodeBook::to_code_length_list() {
	int symbol_size = symbols[0].size;
	int symbol_count = 1 << symbol_size;

	// overlaps with get_length_size()
	auto lengths = std::vector<uint>(symbol_count, 0);
	int length_size = 0;
	for (const auto &symbol : symbols) {
		lengths[symbol.symbol] = symbol.code_length;
		while ((1 << length_size) <= symbol.code_length)
			length_size += 1;
	}

	BitSequence bits;
	for (int i = 0; i < symbol_count; ++i)
		bits.append(BitSequence(lengths[i], length_size));

    return bits;
}

CodeBook CodeBook::from_counted_length_symbol_list(
		BitSequence &bits, int symbol_size,
		int count_size,  int max_code_length) {
	BitSequence count_part = bits.subset(-1, count_size * max_code_length);
	BitSequence symbol_part = bits.subset(count_size * max_code_length, -1);

	// symbol_counts[i]: number of symbols of length i - 1
	std::vector<uint> symbol_counts = count_part.to_primitives(count_size);

	// count total number of symbols
	int symbol_count = 0;
	for (const auto &n : symbol_counts)
		symbol_count += (int) n;

	// check validity of total size of CodeBook
	assert(symbol_part.size() == symbol_size * symbol_count);
	
	// parse symbols
	std::vector<uint> symbols = symbol_part.to_primitives(symbol_size);

	// add symbols with their respective lengths to the symbol vector
	auto book = CodeBook();
	int head = 0;
	// i = current length - 1
	for (int i = 0; i < symbol_counts.size(); ++i) {
		for (int j = 0; j < symbol_counts[i]; ++j) {
			auto symbol = Symbol(i + 1, symbols[head + j], symbol_size);
			book.symbols.push_back(symbol);
		}
		head += symbol_counts[i];
	}

	std::sort(book.symbols.begin(), book.symbols.end(), symbol_comp);

    return book;
}

BitSequence CodeBook::to_counted_length_symbol_list() {
	BitSequence bits;
	// symbol_counts[i]: number of symbols of length i - 1
	std::vector<uint> symbol_counts = get_symbol_counts();
	int count_size = get_count_size(symbol_counts);

	// generate bit compressed version of symbol_counts
	auto count_part = BitSequence(symbol_counts, count_size);

	// generate bit compressed version of symbols (in length->canonical order)
	auto symbol_part = BitSequence();
	for (const auto &symbol: symbols)
		symbol_part.append(symbol.to_bits());

	// TODO: that's like 2-4 copies...
	count_part.append(symbol_part);
    return count_part;
}

std::vector<BitSequence> CodeBook::get_code_table() const {
	int symbol_size = get_symbol_size();
	// init symbol to code table
	auto code_table = std::vector<BitSequence>(1 << symbol_size);

	// populate code table
	uint code = 0;
	int last_length = 1;
	bool first = true;
	for (const auto &symbol : symbols) {
		if (first) {
			first = false;
		} else {
			code += 1;
			code = code << (symbol.code_length - last_length);
		}
		last_length = symbol.code_length;
		code_table[symbol.symbol] = BitSequence(code, symbol.code_length);
	}

	return code_table;
}

BitSequence CodeBook::encode(BitSequence &bits) const {
	auto code_table = get_code_table();

	// append empty bits for symbol size alignment
	int symbol_size = get_symbol_size();
	int appended = 0;
	while (bits.size() % symbol_size != 0) {
		bits.append(0);
		appended++;
	}

	// encode data
	BitSequence encoded;
	BitSequence empty;
	for (int head = 0; head < bits.size(); head += symbol_size) {
		uint symbol = 0;
		for (int i = 0; i < symbol_size; ++i)
			symbol = (symbol << 1) + bits[head + i];
		assert(!(code_table[symbol] == empty));
		encoded.append(code_table[symbol]);
	}

	// unalign original data
	while (appended--)
		bits.data.pop_back();

    return encoded;
}

void CodeBook::print_code_table() const {
	auto code_table = get_code_table();
	BitSequence empty;
	
	for (int i = 0; i < code_table.size(); ++i) {
		if (!(code_table[i] == empty)) {
			if (get_symbol_size() == 8) {
				printf("%4X | %3d | %c: ", i, i, i);
				code_table[i].print_binary();
				printf("\n");
			} else {
				printf("%4X | %3d: ", i, i);
				code_table[i].print_binary();
				printf("\n");
			}
		}
	}
}

bool CodeBook::operator==(const CodeBook &other) const {
	return symbols == other.symbols;
}


int CodeBook::get_length_size() const {
	int length_size = 0;
	for (const auto &symbol : symbols)
		while ((1 << length_size) <= symbol.code_length)
			length_size += 1;
	return length_size;
}

int CodeBook::get_count_size() const {
	// intermediate function to prevent redundant repetition
	std::vector<uint> symbol_counts = get_symbol_counts();
	return get_count_size(symbol_counts);
}

int CodeBook::get_count_size(const std::vector<uint> &symbol_counts) const {
	// get max count (max value in symbol_counts) and get the number of
	// bits required to represent every value in symbol_counts
	// note that this is the maximum number of symbols with the
	// same code length
	int max_count = (int) *max_element(std::begin(symbol_counts),
			std::end(symbol_counts));
	int count_size = 0;
	while((1 << ++count_size) <= max_count);

	return count_size;
}

int CodeBook::get_symbol_size() const {
	return symbols.front().size;
}

int CodeBook::get_max_code_length() const {
	return symbols.back().code_length;
}

std::vector<uint> CodeBook::get_symbol_counts() const {
	// Optimization Note.
	// Currently O(N), N = number of symbols

	auto symbol_counts = std::vector<uint>(get_max_code_length(), 0);
	// count number of symbols the corresponds to each length
	for (const auto &symbol: symbols)
		symbol_counts[symbol.code_length - 1] += 1;

	return symbol_counts;
}
