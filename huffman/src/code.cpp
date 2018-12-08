#include "../include/code.hpp"


PrefixTree PrefixTree::from_content(ByteString &string, int symbol_size) {
    return PrefixTree();
}

PrefixTree::~PrefixTree() {

}

BitSequence PrefixTree::decode(BitSequence bits) const {
    return BitSequence();
}


Symbol::Symbol(int code_length, uint symbol) : code_length(code_length), symbol(symbol) {

}


CodeBook::CodeBook(PrefixTree *tree) {

}

PrefixTree CodeBook::to_canonical_prefix_tree() {
    return PrefixTree();
}

CodeBook CodeBook::from_code_length_list(BitSequence &bits, int symbol_size) {
    return CodeBook(nullptr);
}

CodeBook CodeBook::from_counted_length_symbol_list(BitSequence &bits, int symbol_size) {
    return CodeBook(nullptr);
}


BitSequence CodeBook::to_code_length_list() {
    return BitSequence();
}

BitSequence CodeBook::to_counted_length_symbol_list() {
    return BitSequence();
}

BitSequence CodeBook::encode(ByteString &string) {
    return BitSequence();
}

bool CodeBook::operator==(const CodeBook &other) const {
    return false;
}
