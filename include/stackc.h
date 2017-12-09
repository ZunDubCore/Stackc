/**
 * @file stackc.h
 *
 * @brief Main header
 *
 * Contains global includes and all the functions declared
 * in each file of the project.
 */

#ifndef STACKC_H
#define STACKC_H

/********************************** Includes **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>   /* to get file size */

/***************************** Useful definitions *****************************/

#define TRUE    1       /**< Bool value @c TRUE */
#define FALSE   0       /**< Bool value @c FALSE */
typedef int     BOOL;   /**< @c BOOL type */

#ifndef NULL
#define NULL    0       /**< @c NULL for pointers */
#endif

/*********************************** GLOBAL ***********************************/

typedef struct ParserState ParserState;
typedef struct LexerState LexerState;

/********************************* stackc.c **********************************/

typedef struct Stackc
{
	ParserState *parser;
} Stackc;

/**
 * @brief Main function
 */
int main(int argc, char const *argv[]);

void stackcInit(Stackc *sc);

/**
 * @brief Write an error
 *
 * Print an error to @c stderr using @c fStringPrintf() and
 * @c vfStringPrintf() with @c "Error: " preceding, and exit with exit code
 * @c EXIT_FAILURE.
 * <br>
 * If @c errorMsg includes format specifiers (subsequences beginning with @c %),
 * the additional arguments following @c errorMsg are formatted and inserted
 * in the resulting string replacing their respective specifiers.
 *
 * After the @c errorMsg parameter, the function expects at least as many
 * additional arguments as specified by @c errorMsg.
 *
 * @param errorMsg
 *
 * C string that contains the text to be written.
 * <br>
 * It can optionally contain embedded @c format @c specifiers that are replaced
 * by the values specified in subsequent additional arguments
 * and formatted as requested.
 *
 * @param ...
 *
 * <tt>(additional arguments)</tt>
 * <br>
 * Depending on the @c errorMsg string, the function may expect
 * a sequence of additional arguments, each containing a value to be used
 * to replace a @c format @c specifier in the @c errorMsg string.
 * <br>
 * There should be at least as many of these arguments as the number of
 * values specified in the @c format @c specifiers.
 * <br>
 * Additional arguments are ignored by the function.
 *
 * @see
 * @c fStringPrintf()
 * @c vfStringPrintf()
 */
void error(const char *errorMsg, ...);

/********************************* parser.c **********************************/

typedef struct ParserState
{

} ParserState;

/**
 * @brief Parse the file
 *
 * @param sc
 * Pointer to @c Stackc @c struct
 *
 * @param fileName
 * Path to file
 */
void parseFile(Stackc *sc, const char *fileName);

void parserInit(Stackc *sc, ParserState *parser, const char *sourceText, const char *fileName);

/********************************** lexer.c ***********************************/

typedef struct LexerState
{
	const char *fileName;
	const char *sourceText;
	int sourceLength;
	const char *pos;
	const char *end;
	int line;
} LexerState;

/**
 * @brief @c Enum of @c Lexer tokens
 */
typedef enum LexerToken
{
	TokenHalt,
	TokenPush,
	TokenPop,
	TokenNumber,
	TokenEndOfLine,
	TokenEOF,
	TokenNone
} LexerToken;

typedef union Value
{
	int number;
	char *word;
}

typedef struct Token
{
	LexerToken token;
	Value *value;
} Token;

void lexerInit(Stackc *sc, LexerState *lexer, const char *sourceText, const char *fileName);

Token *lexerTokenize(Stackc *sc, ParseState *parser)

LexerToken lexerGetToken(Stackc *sc, LexerState *lexer);

LexerToken lexerGetWord(Stackc *sc, LexerState *lexer);

LexerToken lexerGetNumber(Stackc *sc, LexerState *lexer);

LexerToken lexerCheckReservedWord(Stackc *sc, const char *word);

/********************************** stack.c ***********************************/

/**
 * @brief @c stackElement type
 *
 * This is the type of the objects entered in the stack.
 * Edit it to change the type of things to be placed in
 * the stack.
 */
typedef int stackElement;

#define EMPTY -1        /**< @c top value when the stack is empty */

/**
 * @brief @c Stack type
 *
 * This is the type for a stack, i.e., it is a type that
 * holds the information necessary to keep track of a stack.
 * It has a pointer @c stack to a dynamically-allocated
 * array (used to hold the contents of the stack), an integer
 * @c size that holds the size of this array (i.e., the
 * maximum number of things that can be held in the stack),
 * and another integer @c top, which stores the array index of
 * the element at the top of the stack.
 */
typedef struct Stack
{
	stackElement *stack;    /**< Pointer to dynamically-allocated stack array */
	int size;               /**< Size of the array */
	int top;                /**< Index of the element at the top of the stack */
} Stack;

/**
 * @brief Initialize a new @c Stack
 *
 * A new stack variable is initialized. The initialized
 * stack is made empty. @c size is used to determine the
 * maximum number of elements that can be held in the
 * stack.
 *
 * @param size
 * Size of @c Stack.
 *
 * @return
 * Pointer to the created @c Stack.
 *
 * Usage:
 * @code
 * Stack *stack = stackInit(size);
 * @endcode
 *
 * @see
 * @c stackDestroy()
 */
Stack *stackInit(int size);

/**
 * @brief Destroy a @c Stack
 *
 * This function frees all memory associated with the stack.
 * The @c Stack variable may not be used again unless
 * @c stackInit(&stack, size) is first called on the stack.
 *
 * @param stack
 * Pointer to @c Stack
 *
 * Usage:
 * @code
 * stackDestroy(&stack);
 * @endcode
 *
 * @see
 * @c stackInit()
 */
void stackDestroy(Stack *stack);

/**
 * @brief Push to @c Stack
 *
 * This is the fundamental stack operation that adds an element to
 * the top of the stack. A call to @c stackPush() on a full stack
 * is an error. Make use of @c stackIsFull() to avoid this error.
 *
 * @param stack
 * Pointer to @c Stack
 *
 * @param element
 * @c stackElement to add
 *
 * Usage:
 * @code
 * stackPush(&stack, element);
 * @endcode
 *
 * @see
 * @c stackPop()
 * @c stackIsFull()
 */
void stackPush(Stack *stack, stackElement element);

/**
 * @brief Pop from @c Stack
 *
 * This is the fundamental stack operations that removes an element
 * from the top of the stack. A call to stackPop on an empty stack
 * is an error. Make use of @c stackIsEmpty() to avoid this error.
 *
 * @param stack
 * Pointer to @c Stack
 *
 * @return
 * @c stackElement popped from the @c Stack
 *
 * Usage:
 * @code
 * stackElement element = StackPop(&stack);
 * @endcode
 *
 *
 * @see
 * @c stackPush
 * @c stackIsEmpty()
 */
stackElement stackPop(Stack *stack);

/**
 * @brief Check if @c Stack is empty
 *
 * @param stack
 * Pointer to @c Stack
 *
 * @return
 * @c TRUE if it is empty, @c FALSE otherwise
 *
 * @see
 * @c stackIsFull()
 */
BOOL stackIsEmpty(Stack *stack);

/**
 * @brief Check if @c Stack is full
 *
 * @param stack
 * Pointer to @c Stack
 *
 * @return
 * @c TRUE if it is full, @c FALSE otherwise
 *
 * @see
 * @c stackIsEmpty()
 */
BOOL stackIsFull(Stack *stack);

/************************************ io.c ************************************/

/**
 * @brief Read the file
 *
 * @param sc
 * Pointer to @c Stackc @c struct
 *
 * @param fileName
 * Path to file
 *
 * @return
 * C string with file contents
 */
char *readFile(Stackc *sc, const char *fileName);

char *concat(const char *s1, const char *s2);

int __printf(const char *format, ...);

int __fprintf(FILE *stream, const char *format, ...);

int __vfprintf(FILE *stream, const char *format, va_list arg);

#endif /* STACKC_H */
