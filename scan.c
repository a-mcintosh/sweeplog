/* Aubrey McIntosh, PhD
 * 2018-11-14
 */

#include <stdio.h>

/* copy input to output */

int main()
{
	int c;
	FILE *fields = tmpfile();
	FILE *values = tmpfile();
        char name[64];
        
	void consume(char *ch) {
		while(c != EOF && c != (short)*ch) c = getchar();
		c = getchar();
	};

	void skip_white() {
		while(c != EOF && c == ' ') c = getchar();
	};

        void scan_string(FILE *of, char *delim) {
		int ix=0;
		while(c != EOF && c != '"') c = getchar();
		c = getchar();
		putc(*delim, of);
		while(c != EOF && c != '"') {name[ix++] = c; putc(c, of); c = getchar();}
		name[ix] = '\0';
		c = getchar();
		putc(*delim, of);
        };

        void scan_num(FILE *of) {
		while(c != EOF && (c < '0' || '9' < c)) c = getchar();
		while(c != EOF && (('0' <= c && c <= '9') || c=='.')) {putc(c, of); c = getchar();}
        };

        void scan_token(FILE *of) {
		while(c != EOF && (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || c=='.')) {
			putc(c, of); c = getchar();
		}
        };

	void init_fields() {
		fprintf(fields, "INSERT INTO test_table (");
	};

	void finish_scan() {
		fprintf(fields, ")\n");
		fprintf(values, ");\n");
	};

	void init_values() {
		fprintf(values, "VALUES (");
	};

	void copy_values() {
		fseek(values, 0, SEEK_SET);
		c = fgetc(values);
		while(c != EOF)
		{
			putc(c, fields);
			c = fgetc(values);
		}
	}

	void copy_to_output() {
		copy_values();
		fseek(fields, 0, SEEK_SET);
		c = fgetc(fields);
		while(c != EOF)
		{
			putchar(c);
			c = fgetc(fields);
		}
	}

        void scan_special(FILE *of, char *delim) {
		while(c != EOF && c != '"') c = getchar();
		c = getchar();
		putc(*delim, of);
		while(c != EOF && c != '"') {
			if(c==',') {putc('.', of);} else {putc(c, of);}
			c = getchar();}
		c = getchar();
		putc(*delim, of);
        };

	void parse() {
		consume("{");
		while(c != EOF && c != '}')
			{
			scan_string(fields, " ");
			c = getchar();
			skip_white();
			if(strcmp(name, "createdate") == 0) {
				scan_special(values, " ");
			} else if('0' <= c && c <= '9') {
				scan_num(values);
			} else if(c == '"') {
				scan_string(values, "'");
			} else if(c == 't' || c == 'f') {
				scan_token(values);
			}
			if (c != '}') {putc(',', fields); putc(',', values);}
			}
		finish_scan();
	}

	c = getchar();
	init_fields();
	init_values();
	parse();
	copy_to_output();
	fclose(fields);
	fclose(values);

	return 0;
}

