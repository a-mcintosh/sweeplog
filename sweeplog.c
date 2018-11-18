/* Aubrey McIntosh, PhD
 * 2018-11-14
 * Scan 
 *   accept the JSON *.sharelog files produced by ckpool
 *   emit SQL INSERT statements accepted by postgresql
 *
 * Alternate names for consideration
 *  sharelog_sweeper
 *  J2S
 */

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <errno.h>
#include <libgen.h>

#include "error.h"

/* transform .sharelog input to .sql output */
static char *inFileName;

int main(int argc, char **argv)
{
	inFileName = "-";
	int argind;
	bool ok;

	int c;
	FILE *shareFieldStatement;
	FILE *shareValueStatement;
	FILE *logFieldStatement;
	FILE *logValueStatement;
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
	void initialize_files(char *inFileName) {
		char aux_FileName[128];
		strcpy(aux_FileName, inFileName);

		shareFieldStatement = tmpfile();
		fprintf(shareFieldStatement, "INSERT INTO test_share (");

		shareValueStatement = tmpfile();
		fprintf(shareValueStatement, "VALUES (");

		logFieldStatement = tmpfile();
		fprintf(logFieldStatement, "INSERT INTO test_log (filepath, filename, hash");

		logValueStatement = tmpfile();
		fprintf(logValueStatement, \
			"VALUES ('%s','%s',", dirname(aux_FileName), basename(aux_FileName));
	};

//  ------------------------------------------------------------
	void write_epilogs() {
		fprintf(shareFieldStatement, ")\n----\n");
		fprintf(shareValueStatement, ");\n----------------\n");
		fprintf(logFieldStatement, ")\n----\n");
		fprintf(logValueStatement, ");\n----------------\n");
	};

//  ------------------------------------------------------------
	void append_shareValueStatement_to_shareFieldStatement() {
		int aux_c;
		fseek(shareValueStatement, 0, SEEK_SET);
		aux_c = fgetc(shareValueStatement);
		while(aux_c != EOF)
		{
			putc(aux_c, shareFieldStatement);
			aux_c = fgetc(shareValueStatement);
		}
	}

//  ------------------------------------------------------------
	void copy_to_output(FILE *inf) {
		int aux_c;
		fseek(inf, 0, SEEK_SET);
		aux_c = fgetc(inf);
		while(aux_c != EOF)
		{
			putchar(aux_c);
			aux_c = fgetc(inf);
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
			initialize_files(inFileName); 
			while(c != EOF && c != '}')
			{
				copy_string_and_requote(shareFieldStatement, " ", name);
				c = getchar();
				skip_whitespace();
				if(strcmp(name, "hash") == 0) {
					copy_string_and_requote(shareValueStatement, "'", name);
					fprintf(logValueStatement, "'%s'", name);
				} else if(strcmp(name, "createdate") == 0) {
					copy_special(shareValueStatement, " ");
				} else if('0' <= c && c <= '9') {
					copy_number(shareValueStatement);
				} else if(c == '"') {
					copy_string_and_requote(shareValueStatement, "'", name);
				} else if(c == 't' || c == 'f') {
					copy_token(shareValueStatement);
				}
				if (c != '}') {putc(',', shareFieldStatement); putc(',', shareValueStatement);}
			}
			consume_including("}");
			consume_including("{");
			write_epilogs();
			copy_to_output(logFieldStatement);
			copy_to_output(logValueStatement);
			copy_to_output(shareFieldStatement);
			copy_to_output(shareValueStatement);
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
	fclose(shareFieldStatement);
	fclose(shareValueStatement);
	fclose(logFieldStatement);
	fclose(logValueStatement);

	return 0;
}

