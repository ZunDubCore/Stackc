#include "stackc.h"

#define isCidstart(c) (isalpha(c) || (c)=='_')
#define isCident(c) (isalnum(c) || (c)=='_')

#define IS_HEX_ALPHA_DIGIT(c) (((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
#define IS_BASE_DIGIT(c,b) (((c) >= '0' && (c) < '0' + (((b)<10)?(b):10)) || (((b) > 10) ? IS_HEX_ALPHA_DIGIT(c) : FALSE))
#define GET_BASE_DIGIT(c) (((c) <= '9') ? ((c) - '0') : (((c) <= 'F') ? ((c) - 'A' + 10) : ((c) - 'a' + 10)))

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

void lexerInit(Stackc *sc, LexerState *lexer, const char *fileName, const char *sourceText, int sourceLength)
{
	lexer->fileName = fileName;
	lexer->sourceText = sourceText;
	lexer->sourceLength = sourceLength;
	lexer->pos = sourceText;
	lexer->end = sourceText + sourceLength;
	lexer->line = 1;
}

Token **lexerTokenize(Stackc *sc, ParserState *parser)
{
	LexerState *lexer;
	LexerToken token;
	Value *value;
	Token **tokens;
	int initSize = 1024;
	int tokenCount = 0;

	tokens = malloc(initSize * sizeof(*tokens));
	for (int i = 0; i < initSize; i++)
	{
        tokens[i] = malloc(sizeof(tokens));
    }

	lexer = malloc(sizeof(*lexer));
	lexerInit(sc, lexer, parser->fileName, parser->sourceText, parser->sourceLength);

	do
	{
		value = malloc(sizeof(*value));
		token = lexerGetToken(sc, lexer, value);

		tokens[tokenCount]->token = token;
		tokens[tokenCount]->value = value;

		tokenCount++;
	} while(token != TokenEOF);

	return tokens;
}

LexerToken lexerGetToken(Stackc *sc, LexerState *lexer, Value *value)
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
		return lexerGetWord(sc, lexer, value);
	}

	if (isdigit(thisChar))
	{
		return lexerGetNumber(sc, lexer, value);
	}

	return TokenEOF;
}

LexerToken lexerGetWord(Stackc *sc, LexerState *lexer, Value *value)
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
	value->word = word;

	return token;
}

LexerToken lexerGetNumber(Stackc *sc, LexerState *lexer, Value *value)
{
	int result = 0;
	int base = 10;

	/* Get the base */
	if (*lexer->pos == '0')
	{
		/* A binary, octal or hex literal */
		LEXER_INC(lexer);

		if (lexer->pos != lexer->end)
		{
			if (*lexer->pos == 'x' || *lexer->pos == 'X')
			{
				base = 16;
				LEXER_INC(lexer);
			}

			else if (*lexer->pos == 'b' || *lexer->pos == 'B')
			{
				base = 2;
				LEXER_INC(lexer);
			}
			else
			{
				base = 8;
			}
		}
	}

	for (; lexer->pos != lexer->end && IS_BASE_DIGIT(*lexer->pos, base); LEXER_INC(lexer))
	{
		result = result * base + GET_BASE_DIGIT(*lexer->pos);
	}

	value->number = result;

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
