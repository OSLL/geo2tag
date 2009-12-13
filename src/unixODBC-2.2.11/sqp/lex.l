/**********************************************************************
 *                           LEX FOR SQL
 **********************************************************************
 *
 *
 **********************************************************************/

/**********************************************************************
 *
 *                             C-CODE
 *
 **********************************************************************/

%{

#include "sqp.h"
#include "yac.h"
#include <string.h>

#undef YY_INPUT
#define YY_INPUT(b, r, ms) (r = my_yyinput(b, ms))

%}

/**********************************************************************
 *
 *                             LEX HEADER
 *
 **********************************************************************/

%e 1200

/**********************************************************************
 *
 *                             LEX BODY
 *
 **********************************************************************/
%%

 /***************************************
  * LITERALS KEYWORDS TOKENS
  ***************************************/

DELETE		{ return DELETE; }
FROM		{ return FROM; }
INSERT		{ return INSERT; }
INTO		{ return INTO; }
SELECT		{ return SELECT; }
SET		{ return SET; }
UPDATE		{ return UPDATE; }
VALUES		{ return VALUES; }
WHERE		{ return WHERE; }
CURRENT		{ return CURRENT; }
OF		{ return OF; }
AND		{ return AND; }
CREATE		{ return CREATE; }
TABLE		{ return TABLE; }
ORDER 		{ return ORDER; }
BY		{ return BY; }
DROP		{ return DROP; }
ASC		{ return ASC; }
DESC		{ return DESC; }
VARCHAR		{ return VARCHAR; }
CHAR		{ return CHAR; }
LONG 		{ return LONG; }
NUMERIC 	{ return NUMERIC; }
DECIMAL 	{ return DECIMAL; }
SMALLINT 	{ return SMALLINT; }
INTEGER 	{ return INTEGER; }
REAL 		{ return REAL; }
FLOAT 		{ return FLOAT; }
DOUBLE 		{ return DOUBLE; }
PRECISION 	{ return PRECISION; }
BIT 		{ return BIT; }
TINYINT 	{ return TINYINT; }
BIGINT 		{ return BIGINT; }
BINARY 		{ return BINARY; }
VARBINARY 	{ return VARBINARY; }
DATE 		{ return DATE; }
TIME 		{ return TIME; }
TIMESTAMP	{ return TIMESTAMP; }
NOT		{ return NOT; }
NULL		{ return NULLX; }
IS      { return IS; }
OR      { return OR; }
LIKE        { return LIKE; }
ESCAPE      { return ESCAPE; }

 /***************************************
  * COMPARISON
  ***************************************/

"="	|
"<>" 	|
"<"	|
">"	|
"<="	|
">="		{
			yylval.strval = (char*)strdup(yytext);
			return COMPARISON;
		}

 /***************************************
  * PUNCTUATION
  ***************************************/

[-+*/:(),.;?]	{
			yylval.strval = (char*)strdup(yytext);
			return yytext[0];
		}

 /***************************************
  * NAMES
  ***************************************/

[A-Za-z][A-Za-z0-9_]*	{
				yylval.strval = (char*)strdup(yytext);
				return NAME;
			}


 /***************************************
  * INTEGER
  ***************************************/

[0-9]+	|
[0-9]+"."[0-9]* |
"."[0-9]*	{
			yylval.intval = atoi(yytext);
			return INTNUM;
		}

 /***************************************
  * APPROX NUM
  ***************************************/

[0-9]+[eE][+-]?[0-9]+	|
[0-9]+"."[0-9]*[eE][+-]?[0-9]+ |
"."[0-9]*[eE][+-]?[0-9]+	{
					yylval.floatval = atol(yytext);
					return APPROXNUM;
				}


 /***************************************
  * STRINGS (single quotes)
  *
  * NOTE: I recommend appending a space on the end of your statement because I
  *       have found that STRINGs will not get picked up if the last char in the
  *       statement is the closing quote of said STRING. - PAH
  *
  ***************************************/

'[^'\n]*'	{
			char	*pszBuffer;
			int 	c;
			
			pszBuffer = (char*)strdup(yytext);	/* store here because we lose it when unput()			*/
			c = input();
			unput( c );				/* just peeking - checking for a double quote... embedded quote */

			if ( c != '\'' )
			{
				yylval.strval = (char*)strdup(pszBuffer);
				free( pszBuffer );
				return STRING;
			}
			else
			{
				free( pszBuffer );
				yymore();
			}				
		}
		
 /***************************************
  * STRINGS (unterminated)
  ***************************************/

'[^'\n]*$	{ 
			yyerror( "Unterminated string" ); 
		}

 /***************************************
  * NEW LINE (nLineNo used in error messages)
  ***************************************/

\n		g_nLineNo++;

 /***************************************
  * WHITE SPACE (ignored)
  ***************************************/

[ \t\r]+	;	/* white space */

 /***************************************
  * COMMENTS (ignored)
  ***************************************/

"--".*$		;	/* comment */

%%
/**********************************************************************
 *
 *                             C-CODE
 *
 **********************************************************************/

#include <string.h>

/**********************************************************************
 * my_yyinput
 *
 * Lexer will ask this function for input when it requires more.
 *
 **********************************************************************/
int my_yyinput(char *buf, int max_size)
{
	int n = ( max_size < g_pszSQLLimit - g_pszSQLCursor ? max_size : g_pszSQLLimit - g_pszSQLCursor );

        if ( n > 0 )
	{
		memcpy( buf, g_pszSQLCursor, n );
		g_pszSQLCursor += n;
	}
		
	return n;		
}

/**********************************************************************
 * yyerror
 *
 * This should be called just before failing. It formats a meaningfull
 * message and deposits it in a usefull place.
 *
 **********************************************************************/
void yyerror( char *s )
{
	sprintf( g_szError, "%s processing '%s' on line %d", s, yytext, g_nLineNo );
	
	YY_FLUSH_BUFFER;
}

/**********************************************************************
 * yywrap
 *
 * We are not doing any buffer switching but lets not use the Flex verion of
 * of this func anyway so we can avoid the link dependency.
 *
 **********************************************************************/
int yywrap()
{
        return 1;
}

