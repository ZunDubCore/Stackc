#include "stackc.h"

char *readFile(Stackc *sc, const char *fileName)
{
	struct stat fileInfo;
	char *readText;
	FILE *inFile;
	int bytesRead;
	char *p;

	if (stat(fileName, &fileInfo))
		error("can't read file %s\n", fileName);

	readText = malloc(fileInfo.st_size + 1);

	if (readText == NULL)
		error("out of memory\n");

	inFile = fopen(fileName, "r");

	if (inFile == NULL)
		error("can't read file %s\n", fileName);

	bytesRead = fread(readText, 1, fileInfo.st_size, inFile);

	if (bytesRead == 0)
		error("can't read file %s\n", fileName);

	readText[bytesRead] = '\0';
	fclose(inFile);

	if ((readText[0] == '#') && (readText[1] == '!'))
	{
		for (p = readText; (*p != '\r') && (*p != '\n'); ++p)
		{
			*p = ' ';
		}
	}

	return readText;
}

char *concat(const char *s1, const char *s2)
{
	char *result;

	result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);

	return result;
}

int __printf(const char *format, ...)
{
	va_list arg;
	int ret;

	va_start(arg, format);
	ret = __vfprintf(stdout, format, arg);
	va_end(arg);

	return ret;
}

int __fprintf(FILE *stream, const char *format, ...)
{
	va_list arg;
	int ret;

	va_start(arg, format);
	ret = __vfprintf(stream, format, arg);
	va_end(arg);

	return ret;
}

int __vfprintf(FILE *stream, const char *format, va_list arg)
{
	const char *fpos;
	int ret;

	for (fpos = format; *fpos != '\0'; fpos++)
	{
		if (*fpos == '%')
		{
			fpos++;

			switch (*fpos)
			{
				case 's':
					ret = fputs(va_arg(arg, char*), stream);
					break;
				case 'd':
					ret = fprintf(stream, "%ld", va_arg(arg, int));
					break;
				case 'c':
					ret = putc(va_arg(arg, int), stream);
					break;
				case 'f':
					ret = fprintf(stream, "%f", va_arg(arg, double));
					break;
				case '%':
					ret = putc('%', stream);
					break;
				case '\0':
					fpos--;
					break;
				default:
					ret = vfprintf(stream, concat("%", fpos), arg);
					break;
			}
		}

		else
		{
			ret = putc(*fpos, stream);
		}
	}

	return ret;
}
