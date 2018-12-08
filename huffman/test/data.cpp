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

	auto bits5 = BitSequence(0x5, 8);
	auto bits3 = BitSequence(0x3, 8);

	SECTION( "initialize BitSequence" ) {
		REQUIRE( bits5.data[0] == 0 );
		REQUIRE( bits5.data[1] == 0 );
		REQUIRE( bits5.data[2] == 0 );
		REQUIRE( bits5.data[3] == 0 );
		REQUIRE( bits5.data[4] == 0 );
		REQUIRE( bits5.data[5] == 1 );
		REQUIRE( bits5.data[6] == 0 );
		REQUIRE( bits5.data[7] == 1 );
		REQUIRE( bits5.get_length() == 8 );

		REQUIRE( bits3.data[0] == 0 );
		REQUIRE( bits3.data[1] == 0 );
		REQUIRE( bits3.data[2] == 0 );
		REQUIRE( bits3.data[3] == 0 );
		REQUIRE( bits3.data[4] == 0 );
		REQUIRE( bits3.data[5] == 0 );
		REQUIRE( bits3.data[6] == 1 );
		REQUIRE( bits3.data[7] == 1 );
		REQUIRE( bits3.get_length() == 8 );
	}
}
