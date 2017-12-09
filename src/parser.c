#include "stackc.h"

void parseFile(Stackc *sc, const char *fileName)
{
	char *source;
	LexerToken token;
	Token *tokens;
	ParserState *parser;

	parser = malloc(sizeof(*parser));
	sc->parser = parser;

	source = readFile(sc, fileName);
	parserInit(sc, sc->parser, source, fileName);

	tokens = lexerTokenize(sc, parser);


}

void parserInit(Stackc *sc, ParserState *parser, const char *sourceText, const char *fileName)
{
	parser->fileName = fileName;
	parser->sourceText = sourceText;
	parser->sourceLength = strlen(sourceText);
	parser->line = 1;
}
