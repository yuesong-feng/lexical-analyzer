# Lexical-analyzer
编译原理，C++实现C-语言的词法分析器
```cpp
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
```

分析前的C-语言片段：
```c
/* A program to perform finding the maximum number of the three. */

int max(int x, int y, int z)
{
	int result;
	if (x > y)
	{
		result = x;
		if (z > result)
		{
			result = z;
		}
	}
	else
	{
		result = y;
		if (z > result)
		{
			result = z;
		}
	}
	return result;
}

void main(void)
{
	int a;
	int b;
	int c;
	/* int d; */
	a = input();
	b = input();
	c = input();
	output(max(a, b, c));
	
	/*
	d = 2 * max(a, b, c);
	output(d);
	*/
}
```

经过词法分析后的代码片段：
```
3:	reserved word: int
3:	ID, name = max
3:	(
3:	reserved word: int
3:	ID, name = x
3:	,
3:	reserved word: int
3:	ID, name = y
3:	,
3:	reserved word: int
3:	ID, name = z
3:	)
4:	{
5:	reserved word: int
5:	ID, name = result
5:	;
6:	reserved word: if
6:	(
6:	ID, name = x
6:	>
6:	ID, name = y
6:	)
7:	{
8:	ID, name = result
8:	=
8:	ID, name = x
8:	;
9:	reserved word: if
9:	(
9:	ID, name = z
9:	>
9:	ID, name = result
9:	)
10:	{
11:	ID, name = result
11:	=
11:	ID, name = z
11:	;
12:	}
13:	}
14:	reserved word: else
15:	{
16:	ID, name = result
16:	=
16:	ID, name = y
16:	;
17:	reserved word: if
17:	(
17:	ID, name = z
17:	>
17:	ID, name = result
17:	)
18:	{
19:	ID, name = result
19:	=
19:	ID, name = z
19:	;
20:	}
21:	}
22:	reserved word: return
22:	ID, name = result
22:	;
23:	}
25:	reserved word: void
25:	ID, name = main
25:	(
25:	reserved word: void
25:	)
26:	{
27:	reserved word: int
27:	ID, name = a
27:	;
28:	reserved word: int
28:	ID, name = b
28:	;
29:	reserved word: int
29:	ID, name = c
29:	;
31:	ID, name = a
31:	=
31:	ID, name = input
31:	(
31:	)
31:	;
32:	ID, name = b
32:	=
32:	ID, name = input
32:	(
32:	)
32:	;
33:	ID, name = c
33:	=
33:	ID, name = input
33:	(
33:	)
33:	;
34:	ID, name = output
34:	(
34:	ID, name = max
34:	(
34:	ID, name = a
34:	,
34:	ID, name = b
34:	,
34:	ID, name = c
34:	)
34:	)
34:	;
40:	}
EOF
```
