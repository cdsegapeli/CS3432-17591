#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
const int MAX = 50;

/* Return true if c is a whitespace character ('\t' or ' ') not includeing zero terminators. */
int space_char(char c){

	int space = 0;

	if (c == ' ' || c == '\t' || c == '\n'){
		space = 1;
	}

	return space;
}

/* Return true if c is a non-whitespace character not including zer terminators. */
int non_space_char(char c){

	int non_space = 0;

	if ((c != ' ') && (c != '\t') && (c != '\0')){
		non_space = 1;
	}

	return non_space;
}

/* Returns a pointer to the first character of the next space-separated word in zero-terminated str.
   Return a zero pointer if the str does not contain any words. */
char *word_start(char *str){

	char *start;

	/* if the string is empty */
	if (*str == '\0'){
		start = 0;
	}

	start = str;
	while (space_char(*start)){
		start++;
	}

	return start;
}

/* Returns a pointer terminator char following *word. */
char *word_terminator(char *word){

	char *terminator = word;

	/* move ther pointer along while it is a non white space character */
	while ((non_space_char(*terminator)) && (*terminator != '\0')){
		terminator++;
	}

	return terminator;
}

/* Counts the number of words in the string. */
int count_words(char *str){

	int num_words = 0;
	char *start = str;
	char *end = str;

	while (*end != '\0'){
		start = word_start(end);
		end = word_terminator(start);
		num_words++;
	}

	return num_words;
}

/* Returns a freshly allocated new zero-terminated string containg <len> chars from <inStr> */
char *copy_str(char *inStr, short len){

	int i = 1;
	char* word = (char*) malloc(sizeof(char) * (len+1));
	word[len] = '\0';

	while (i <= len){
		word[i] = *inStr;
		inStr++;
		i++;
	}

	return word;
}

/* Returns a freshly allocated zero-terminated vector of freshly allocated 
   space-sparated tokens from zero-terminated str.
   For example, tokenize("hello world string") would result in:
   		tokens[0] = "hello"
   		tokens[1] = "world"
   		tokens[2] = "string"
   		tokens[3] = 0
*/
char **tokenize(char* str){

	int size = count_words(str) + 1;
	char** tokens = (char**) malloc(sizeof(char*) * size);
	char *start = str;
	char *end = str;
	int i;

	tokens[size - 1] = 0;

	for (i = 0; i < size-1; i++){
		start = word_start(end);
		end = word_terminator(start);
		int len = end - start;
		tokens[i] = copy_str(start, len);
	}

	return tokens;
}

/* Prints all tokens. */
void print_tokens(char **tokens){

	int i = 0;
	while (tokens[i] != 0){

		int j = 1;
		printf("token[%d] = ", i);

		while (tokens[i][j] != '\0' && tokens[i][j] != '\n'){

			putchar(tokens[i][j]);
			j++;
		}

		putchar('\n');
		i++;
	}

	printf("token[%d] = ", i);
	putchar('0');
	putchar('\n');
}


int main(){

	char str[MAX];
	while(1){
		putchar('\n');
		printf("Enter a string or enter 'q' to quit.\n>");
		fgets(str, MAX, stdin);

		if (str[0] == 'q' && str[2] == '\0'){
			goto exit;
		}
		else{
			printf("%s\n", str);
			char **tokens = tokenize(&str[0]);
			print_tokens(tokens);
		}
	}
exit:
	return 0;
}
