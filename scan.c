#include <stdio.h>

/* copy input to output */

int main()
{
	int c;
        
	void consume(char *ch) {
		while(c != EOF && c != (short)*ch) c = getchar();
		c = getchar();
	};

        void scan_string() {
		while(c != EOF && c != '"') c = getchar();
		c = getchar();
		int aux_c = *"<";
		putchar(*"<");
		while(c != EOF && c != '"') c = getchar();
		c = getchar();
		putchar(*">");
        };

        void scan_num() {
		while(c != EOF && (c < '0' || '9' < c)) c = getchar();
		putchar(*"<");
		while(c != EOF && (c < '0' && '9' < c || c=='.')) c = getchar();
		putchar(*">");
        };

	c = getchar();

	consume("{");
	while(c != EOF)
		{
		putchar(c);
		c = getchar();
		}

	return 0;
}

