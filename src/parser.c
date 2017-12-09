#include "stackc.h"

void parseFile(Stackc *sc, const char *fileName)
{
	char *source;
	int length;
	LexerToken token;
	Token **tokens;
	ParserState *parser;

	parser = malloc(sizeof(*parser));
	sc->parser = parser;

	source = readFile(sc, fileName);
	length = strlen(source);

	parserInit(sc, sc->parser, fileName, source, length);

	tokens = lexerTokenize(sc, parser);
}

void parserInit(Stackc *sc, ParserState *parser, const char *fileName, const char *sourceText, int sourceLength)
{
	parser->fileName = fileName;
	parser->sourceText = sourceText;
	parser->sourceLength = sourceLength;
	parser->line = 1;
}
