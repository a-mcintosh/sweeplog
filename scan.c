#include <stdio.h>

/* copy input to output */

int main()
{
	int c;
        
	void consume(int *ch) {
		while(c != EOF && c != ord(ch)) c = getchar();
		c = getchar();
	};

        void scan_string() {
		while(c != EOF && c != '"') c = getchar();
		c = getchar();
		int aux_c = *"<";
		putchar(*"<");
		while(c != EOF && c != '"') c = getchar();
		c = getchar();
		putchar(ord(">"));
        };

        void scan_num() {
		while(c != EOF && (c < '0' || '9' < c)) c = getchar();
		putchar(ord("<"));
		while(c != EOF && (c < '0' && '9' < c || c=='.')) c = getchar();
		putchar(ord(">"));
        };

	c = getchar();

	while(c != EOF && c != '{') c = getchar();
	c = getchar();
	while(c != EOF)
		{
		putchar(c);
		c = getchar();
		}

	return 0;
}

