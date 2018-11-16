/* Aubrey McIntosh, PhD
 * 2018-11-14
 * Scan 
 *   accept the JSON *.sharelog files produced by ckpool
 *   emit SQL INSERT statements accepted by postgresql
 */

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stropts.h>

#include "error.h"

/* copy input to output with transformation */
static char const *inFileName;

int main(int argc, char **argv)
{
	inFileName = "-";
	int argind;
	bool ok;

	int c;
	FILE *fieldStatement;
	FILE *valueStatement;
        char name[64];
        
//  ------------------------------------------------------------
	void consume_including(const char *waypoint) {
		while(c != EOF && c != (short)*waypoint) c = getchar();
		c = getchar();
	};

//  ------------------------------------------------------------
	void skip_whitespace() {
		while(c != EOF && (c == ' ' || c == '\t' || c == '\n')) c = getchar();
	};

//  ------------------------------------------------------------
        void copy_string_and_requote(FILE *of, const char *new_delim, char name[64]) {
		int ix=0;
		consume_including("\"");
		putc(*new_delim, of);
		while(c != EOF && c != '"') {name[ix++] = c; putc(c, of); c = getchar();}
		name[ix] = '\0';
		consume_including("\"");
		putc(*new_delim, of);
        };

//  ------------------------------------------------------------
        void copy_number(FILE *of) {
		while(c != EOF && (c < '0' || '9' < c)) c = getchar();
		while(c != EOF && (('0' <= c && c <= '9') || c=='.')) {putc(c, of); c = getchar();}
        };

//  ------------------------------------------------------------
        void copy_token(FILE *of) {
		while(c != EOF && (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || c=='.')) {
			putc(c, of); c = getchar();
		}
        };

//  ------------------------------------------------------------
	void initialize_file_fieldStatement() {
		fieldStatement = tmpfile();
		fprintf(fieldStatement, "INSERT INTO test_table (Filename,");
	};

//  ------------------------------------------------------------
	void initialize_file_valueStatement(char const *inFileName) {
		valueStatement = tmpfile();
		fprintf(valueStatement, "VALUES ('%s',", inFileName);
	};

//  ------------------------------------------------------------
	void write_epilogs() {
		fprintf(fieldStatement, ")\n----\n");
		fprintf(valueStatement, ");\n----------------\n");
	};

//  ------------------------------------------------------------
	void append_valueStatement_to_fieldStatement() {
		int aux_c;
		fseek(valueStatement, 0, SEEK_SET);
		aux_c = fgetc(valueStatement);
		while(aux_c != EOF)
		{
			putc(aux_c, fieldStatement);
			aux_c = fgetc(valueStatement);
		}
	}

//  ------------------------------------------------------------
	void copy_to_output() {
		int aux_c;
		append_valueStatement_to_fieldStatement();
		fseek(fieldStatement, 0, SEEK_SET);
		aux_c = fgetc(fieldStatement);
		while(aux_c != EOF)
		{
			putchar(aux_c);
			aux_c = fgetc(fieldStatement);
		}
	}

//  ------------------------------------------------------------
        void copy_special(FILE *of, char *delim) {
		consume_including("\"");
		putc(*delim, of);
		while(c != EOF && c != '"') {
			if(c==',') {putc('.', of);} else {putc(c, of);}
			c = getchar();}
		consume_including("\"");
		putc(*delim, of);
        };

//  ------------------------------------------------------------
	void parse() {
		consume_including("{");
		while(c != EOF) {
			initialize_file_fieldStatement(); 
			initialize_file_valueStatement(inFileName);
			while(c != EOF && c != '}')
			{
				copy_string_and_requote(fieldStatement, " ", name);
				c = getchar();
				skip_whitespace();
				if(strcmp(name, "createdate") == 0) {
					copy_special(valueStatement, " ");
				} else if('0' <= c && c <= '9') {
					copy_number(valueStatement);
				} else if(c == '"') {
					copy_string_and_requote(valueStatement, "'", name);
				} else if(c == 't' || c == 'f') {
					copy_token(valueStatement);
				}
				if (c != '}') {putc(',', fieldStatement); putc(',', valueStatement);}
			}
			consume_including("}");
			consume_including("{");
			write_epilogs();
			copy_to_output();
		}
	}

//  ------------------------------------------------------------
	void setup_input() {
		argind = optind;
		ok = true;
		if (argind < argc) {
			inFileName = argv[argind];
			printf("-- The file name is %s\n", inFileName);
		}

		if (!strcmp (inFileName, "-"))
			{
				printf("-- Using stdin from -\n");
			}
		else
			{
				stdin = freopen(inFileName, "r", stdin);
				if (stdin == 0)
					{
						error (0, errno, "%s", (inFileName));
						ok = false;
					} else {
						printf("-- opened without error\n");
					}
			}
	}

//  ------------------------------------------------------------
	setup_input();
	c = getchar();
	parse();
	fclose(fieldStatement);
	fclose(valueStatement);

	return 0;
}

