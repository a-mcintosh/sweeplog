/* Aubrey McIntosh, PhD
 * 2018-11-14
 */

#include <stdio.h>
#include <string.h>
#include <getopt.h>

/* copy input to output */
static char const *infile;

int main(int argc, char **argv)
{
	infile = "-";
	int argind;
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
		fseek(fields, 0, SEEK_SET);
		fprintf(fields, "INSERT INTO test_table (");
	};

	void finish_scan() {
		fprintf(fields, ")\n\n");
		fprintf(values, ");\n\n");
	};

	void init_values() {
		fseek(values, 0, SEEK_SET);
		fprintf(values, "VALUES (");
	};

	void copy_values() {
		int aux_c;
		fseek(values, 0, SEEK_SET);
		aux_c = fgetc(values);
		while(aux_c != EOF)
		{
			putc(aux_c, fields);
			aux_c = fgetc(values);
		}
	}

	void copy_to_output() {
		int aux_c;
		copy_values();
		fseek(fields, 0, SEEK_SET);
		aux_c = fgetc(fields);
		while(aux_c != EOF)
		{
			putchar(aux_c);
			aux_c = fgetc(fields);
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
		while(c != EOF) {
			init_fields(); 
			init_values();
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
			consume("}");
			consume("{");
			finish_scan();
			copy_to_output();
		}
	}

//  --------------------------------
	argind = optind;
	c = getchar();
	parse();
	fclose(fields);
	fclose(values);

	return 0;
}

