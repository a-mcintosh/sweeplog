#include <stdio.h>

/* copy input to output */

int main()
{
	int c;
        
	void consume(char *ch) {
		while(c != EOF && c != (short)*ch) c = getchar();
		c = getchar();
	};

	void skip_white() {
		while(c != EOF && c == ' ') c = getchar();
	};

        void scan_string() {
		while(c != EOF && c != '"') c = getchar();
		c = getchar();
		putchar(*"<");
		while(c != EOF && c != '"') {putchar(c); c = getchar();}
		c = getchar();
		putchar(*">");
        };

        void scan_num() {
		while(c != EOF && (c < '0' || '9' < c)) c = getchar();
		putchar(*"N");
		while(c != EOF && (('0' <= c && c <= '9') || c=='.')) {putchar(c); c = getchar();}
		putchar(*"N");
        };

	c = getchar();

	consume("{");
	while(c != EOF && c != '}')
		{
		scan_string();
		putchar(c);
		c = getchar();
		skip_white();
		putchar(*"!"); putchar(c);
		if('0' <= c && c <= '9') {
			scan_num();
		} else if(c == '"') {
			scan_string();
		}
		putchar(*"\n");
		}

	return 0;
}

