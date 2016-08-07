#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	printf("hey\n");
	
 	char *regex = (char*) malloc(129);
 	size_t bsize = 128;

 	while (1)
 	{
 		regex[getline(&regex, &bsize, stdin)-1] = '\0';

        RegexParser *prs = new RegexParser();

 		RegexNode *test = prs->parse((const char**) &regex);

        printf("PARSED");

        break;
 	}
	
	return 0;
}
