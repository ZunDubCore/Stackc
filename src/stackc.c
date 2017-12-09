#include "stackc.h"

int main(int argc, const char *argv[])
{
	Stackc *sc;

	if (argc < 2)
	{
		error("format: stackc <program>\n");
	}

	sc = malloc(sizeof(sc));

	stackcInit(sc);

	parseFile(sc, argv[1]);

	free(sc);

	return EXIT_SUCCESS;
}

void stackcInit(Stackc *sc)
{

}

void error(const char *errorMsg, ...)
{
	va_list args;

	fprintf(stderr, "Error: ");
	va_start(args, errorMsg);
	vfprintf(stderr, errorMsg, args);
	exit(EXIT_FAILURE);
}
