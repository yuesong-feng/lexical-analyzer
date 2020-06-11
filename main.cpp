#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<ctype.h>
#include<string.h>
typedef enum { START, INNUM, INID, INEQ, INNEQ, INLE, INRE, INLS, INCMT, ENDCMT, DONE }StateType;
void Save(char c,char *buf){
	for (int i = 0; i < 100; i++) {
		if (buf[i] == '\0') {
			buf[i] = c;
			break;
		}
	}
}
void getNext() {
	StateType state= START;
	int c;
	int count = 1;
	FILE* fp = NULL;
	FILE* out = NULL;
	fp = fopen("test.c-", "r");
	out = fopen("result.txt","w");
	char buf[100];
	memset(buf, 0, 100 * sizeof(char));
	while ((c = fgetc(fp))!=EOF) {
		if (c == '\n') { count++; }
		switch (state) {
		case START:
			if (isdigit(c)) { Save(c, buf); state = INNUM; }
			else if (isalpha(c)) { Save(c, buf);  state = INID; }
			else if (c == ' ' || c == '\n' || c == '\t') { state = START; }
			else if (c == '=') { Save(c,buf);state = INEQ; }
			else if (c == '<') { Save(c,buf);state = INLE; }
			else if (c == '>') { Save(c,buf); state = INRE; }
			else if (c == '!') { Save(c,buf);state = INNEQ; }
			else if (c == '/') { Save(c,buf);state = INLS; }
			else if (c == '+' || c == '-' || c == ',' || c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') { Save(c,buf); fseek(fp, -1L, 1); state = DONE; }
			break;
		case INNUM:
			if (isdigit(c)) { Save(c, buf); state = INNUM; }
			else { fseek(fp, -2L, 1); state = DONE; }
			break;
		case INID:
			if (isalpha(c)) { Save(c, buf); state = INID; }
			else { fseek(fp,-2L,1); state = DONE; }
			break;
		case INEQ:
			if (c == '=') { Save(c,buf); state = DONE; }
			else { fseek(fp, -1L, 1); state = DONE; }
			break;
		case INNEQ:
			if (c == '=') { Save(c, buf); state = DONE; }
			else { fseek(fp, -1L, 1); state = DONE; }
			break;
		case INLE:
			if (c == '=') { Save(c, buf); state = DONE; }
			else { fseek(fp, -1L, 1); state = DONE; }
			break;
		case INRE:
			if (c == '=') { Save(c, buf); state = DONE; }
			else { fseek(fp, -1L, 1); state = DONE; }
			break;
		case INLS:
			if (c == '*') { Save(c, buf); state = INCMT; }
			else { fseek(fp, -1L, 1); state = DONE; }
			break;
		case INCMT:
			if (c == '*') { Save(c, buf); state = ENDCMT; }
			else { Save(c, buf); state = INCMT; }
			break;
		case ENDCMT:
			if (c == '/') { memset(buf, 0, 100 * sizeof(char)); state = START; }
			else { Save(c, buf); state = INCMT; }
			break;
		case DONE:
			if (!strcmp(buf, "else") || !strcmp(buf, "if") || !strcmp(buf, "int") || !strcmp(buf, "return") || !strcmp(buf, "void") || !strcmp(buf, "while"))
				fprintf(out, "%d:\treserved word: %s\n", count, buf);
			else if (isalpha(buf[0]))
				fprintf(out, "%d:\tID, name = %s\n", count, buf);
			else if (isdigit(buf[0]))
				fprintf(out, "%d:\tNUM, val = %s\n", count, buf);
			else
				fprintf(out, "%d:\t%s\n", count, buf);
			memset(buf, 0, 100 * sizeof(char));
			state = START;
			break;
		}
	}
	fprintf(out,"EOF\n");
	fclose(fp);
	fclose(out);
}
int main() {
	getNext();
	return 0;
}
