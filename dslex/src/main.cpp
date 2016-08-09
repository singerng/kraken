#include <regex.h>
#include <iostream>

int main(int argc, char **argv)
{
    RegexParser prs;
	
 	char *regex = (char*) malloc(129);
 	size_t bsize = 128;

 	while (1)
 	{
 		regex[getline(&regex, &bsize, stdin)-1] = '\0';

 		DFA test = prs.parse((const char**) &regex);
		test.print();
 	}
	
	return 0;
}
