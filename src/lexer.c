#include "stackc.h"

#define isCidstart(c) (isalpha(c) || (c)=='_')
#define isCident(c) (isalnum(c) || (c)=='_')

#define LEXER_INC(l) ( (l)->pos++ )

typedef struct ReservedWord
{
	const char *word;
	LexerToken token;
} ReservedWord;

static ReservedWord reservedWords[] =
{
	{"halt", TokenHalt},
	{"push", TokenPush},
	{"pop", TokenPop}
};

void lexerInit(Stackc *sc, LexerState *lexer, const char *sourceText, const char *fileName)
{
	lexer->fileName = fileName;
	lexer->sourceText = sourceText;
	lexer->sourceLength = strlen(sourceText);
	lexer->pos = sourceText;
	lexer->end = sourceText + lexer->sourceLength;
	lexer->line = 1;
}

LexerToken lexerGetToken(Stackc *sc, LexerState *lexer)
{
	char thisChar;

	/* Skip white characters */
	while (lexer->pos != lexer->end && isspace(*lexer->pos))
	{
		if (*lexer->pos == '\n')
		{
			/* New line found */
			lexer->line++;
			LEXER_INC(lexer);
			return TokenEndOfLine;
		}

		LEXER_INC(lexer);
	}

	if (lexer->pos == lexer->end || *lexer->pos == '\0')
	{
		return TokenEOF;
	}

	thisChar = *lexer->pos;

	if (isCidstart(thisChar))
	{
		return lexerGetWord(sc, lexer);
	}

	if (isdigit(thisChar))
	{
		return lexerGetNumber(sc, lexer);
	}

	return TokenEOF;
}

LexerToken lexerGetWord(Stackc *sc, LexerState *lexer)
{
	const char *startPos = lexer->pos;
	LexerToken token = TokenNone;
	char *word;
	int len;

	do
	{
		LEXER_INC(lexer);
	}
	while (lexer->pos != lexer->end && isCident(*lexer->pos));

	len = lexer->pos - startPos;
	word = malloc(len + 1);	/* (len + 1) for '\0' ending */
	strncpy(word, startPos, len);
	word[len] = '\0';

	token = lexerCheckReservedWord(sc, word);

	return token;
}

LexerToken lexerGetNumber(Stackc *sc, LexerState *lexer)
{
	int result = 0;
	int base = 10;

	for (; lexer->pos != lexer->end && isdigit(*lexer->pos); LEXER_INC(lexer))
	{
		result = result * base + (*lexer->pos - '0');
	}

	__printf("number: %d\n", result);

	return TokenNumber;
}

LexerToken lexerCheckReservedWord(Stackc *sc, const char *word)
{
	for (int count = 0; count < (sizeof(reservedWords) / sizeof(ReservedWord)); count++)
	{
		if (!strcmp(word, reservedWords[count].word))
			return reservedWords[count].token;
	}

	return TokenNone;
}
