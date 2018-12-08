#include "catch.hpp"
#include "../include/data.hpp"
#include "../include/code.hpp"

TEST_CASE( "ByteString", "[tag]" ) {
	std::vector<uchar> data;
	data.push_back(0x34);
	data.push_back(0x23);

	auto string = ByteString(data);
	
	SECTION( "initializing ByteString" ) {
		REQUIRE( string.data[0] == 0x34 );
		REQUIRE( string.data[1] == 0x23 );
		REQUIRE( string.data.size() == 2 );
	}
}
