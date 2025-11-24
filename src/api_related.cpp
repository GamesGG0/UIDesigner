#include <geode.devtools/include/API.hpp>


void example() {
	int current = 0;
	devtools::combo("Example Combo", current, { "Item 1", "Item 2", "Item 3" }, 5);
	devtools::newLine();
	devtools::sameLine();
	devtools::nextItemWidth(10);
	devtools::separator();

	std::string a;
	devtools::inputMultiline("a", a);

	int b;
	devtools::radio("hi", b, b);
}


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#pragma message("EXPANSION: " STR(GEODE_EVENT_EXPORT_NORES(&combo, (label, current, items, maxHeight))))