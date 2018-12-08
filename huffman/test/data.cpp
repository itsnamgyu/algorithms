#include "catch.hpp"
#include "../include/data.hpp"
#include "../include/code.hpp"

TEST_CASE( "data.hpp", "[tag]" ) {
	std::vector<uchar> data;
	data.push_back(0x34);
	data.push_back(0x23);

	auto string = ByteString(data);
	
	SECTION( "initialize ByteString" ) {
		REQUIRE( string.data[0] == 0x34 );
		REQUIRE( string.data[1] == 0x23 );
		REQUIRE( string.data.size() == 2 );
	}

	auto bits5 = BitSequence(0x5, 4);
	auto bits3 = BitSequence(0x3, 4);

	SECTION( "initialize BitSequence" ) {
		REQUIRE( bits5.data[0] == 0 );
		REQUIRE( bits5.data[1] == 1 );
		REQUIRE( bits5.data[2] == 0 );
		REQUIRE( bits5.data[3] == 1 );
		REQUIRE( bits5.get_length() == 4 );

		REQUIRE( bits3.data[0] == 0 );
		REQUIRE( bits3.data[1] == 0 );
		REQUIRE( bits3.data[2] == 1 );
		REQUIRE( bits3.data[3] == 1 );
		REQUIRE( bits3.get_length() == 4 );
	}

	bits5.append(bits3);
	auto bits53 = BitSequence(0x53, 8);

	SECTION( "append BitSequence" ) {
		REQUIRE( bits5 == bits53);
	}
}
