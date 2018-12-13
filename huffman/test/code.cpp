#include "catch.hpp"
#include "../include/data.hpp"
#include "../include/code.hpp"


TEST_CASE( "code.hpp", "[compress_decompress]" ) {
	char byte[] = "test/byte";
	char txt[] = "test/catch.hpp";
	char image[] = "test/lena.jpg";
	char exec[] = "test/a.out";

	std::vector<char*> names;
	names.push_back(byte);
	names.push_back(txt);
	names.push_back(image);
	names.push_back(exec);

	for (auto name : names) {
		FILE *f = fopen(name, "rb");
		if (f == NULL) {
			printf("couldn't open test file [%s]\n", name);
			exit(-1);
		}

		printf("Testing file [%s]\n", name);
		auto bits = BitSequence(f);
		fclose(f);

		for (int i = 4; i <= 24; ++i) {
			printf("Testing bit size = %d\n", i);
			auto *tree = PrefixTree::from_content(bits, i);
			auto book = CodeBook(tree);
			auto *ctree = book.to_canonical_prefix_tree();
			auto cbook = CodeBook(tree);

			REQUIRE( book == cbook );

			int appended = (i - (bits.size() % i)) % i;
			auto encoded = book.encode(bits);
			auto decoded = ctree->decode(encoded);

			while (appended--)
				decoded.data.pop_back();

			REQUIRE( bits == decoded );

			free(tree);
			free(ctree);
		}
	}
}
