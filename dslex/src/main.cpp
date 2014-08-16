#include <regex.h>

int main(int argc, char **argv)
{
	const char *regex = "(a|b)*aab";
	RegexNode *test = parse_regex(&regex);
	test->print(0);
	
	return 0;
}