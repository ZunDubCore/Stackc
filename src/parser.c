#include "stackc.h"

void parseFile(Stackc *sc, const char *fileName)
{
	char *source;
	LexerToken token;
	ParserState *parser;

	parser = malloc(sizeof(*parser));
	sc->parser = parser;

	source = readFile(sc, fileName);
	parserInit(sc, sc->parser, source, fileName);

	while ((token = lexerGetToken(sc, sc->parser->lexer)) != TokenEOF)
	{
		__printf("token %d\n", token);
	}
}

void parserInit(Stackc *sc, ParserState *parser, const char *sourceText, const char *fileName)
{
	LexerState *lexer;

	lexer = malloc(sizeof(*lexer));
	parser->lexer = lexer;

	lexerInit(sc, parser->lexer, sourceText, fileName);
}
