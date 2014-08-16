#include <regex.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	RegexNode *test = parse_regex((const char**) &argv[1]);
	test->compute();
	test->print(0);
	
// 	char *regex = malloc(129);
// 	int bsize = 128;
// 	
// 	while (1)
// 	{
// 		getline(&regex, &bsize, stdin)
// 		RegexNode *test = parse_regex(&regex);
// 		test->print(0);
// 	}
	
	return 0;
}
