#include "../include/data.hpp"


Data::Data() = default;

Data::~Data() = default;

Data::Data(const uchar *data, int n) {

}

Data Data::from_stream(FILE *stream, int n) {
    return Data();
}

bool Data::operator==(const Data &other) const {
    return false;
}

BitSequence::BitSequence() = default;

BitSequence::~BitSequence() = default;

Data BitSequence::compile() const {
    return Data();
}

int BitSequence::get_length() {
    return 0;
}

void BitSequence::append(const BitSequence &tail) {

}

uchar *BitSequence::get_data() {
    return nullptr;
}

bool BitSequence::operator==(const BitSequence &other) const {
    return false;
}
