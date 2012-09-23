/* jsmin.c
   2012-07-02

Copyright (c) 2002 Douglas Crockford  (www.crockford.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

The Software shall be used for Good, not Evil.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include "jsmin.h"

typedef struct {
	char *javascript;
	smart_str *buffer;
	int theA;
	int theB;
	int theLookahead;
	int theX;
	int theY;
	int isFailed;
} jsmin_obj;

/* new_jsmin_obj -- sets up new struct for assisting in minification
*/

static jsmin_obj*
new_jsmin_obj(char *javascript TSRMLS_DC)
{
	jsmin_obj *jmo    = emalloc(sizeof(jsmin_obj));

	jmo->javascript   = javascript;
	jmo->buffer       = emalloc(sizeof(smart_str));
	jmo->theLookahead = 0;
	jmo->theX         = 0;
	jmo->theY         = 0;
	jmo->isFailed     = 0;

	return jmo;
}

/* free_jsmin_obj -- frees up memory on struct
*/

static void
free_jsmin_obj(jsmin_obj *jmo TSRMLS_DC)
{
	smart_str_free(jmo->buffer);
	efree(jmo->buffer);
	efree(jmo);
}

/* jsmin_error -- sets failure on struct and fires warning
*/

static void
jsmin_error(char *s, jsmin_obj *jmo)
{
	jmo->isFailed = 1;

	zend_error(E_WARNING, "jsmin() %s", s);
}

/* jsmin_isAlphanum -- return true if the character is a letter, digit, underscore,
		dollar sign, or non-ASCII character.
*/

static int
jsmin_isAlphanum(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
		(c >= 'A' && c <= 'Z') || c == '_' || c == '$' || c == '\\' ||
		c > 126);
}


/* jsmin_get -- return the next character from stdin. Watch out for lookahead. If
		the character is a control character, translate it to a space or
		linefeed.
*/

static int
jsmin_get(jsmin_obj *jmo)
{
	int c = jmo->theLookahead;
	jmo->theLookahead = 0;
	if (c == 0) {
		c = *jmo->javascript;
		++(jmo->javascript);
		jmo->theY = jmo->theX;
		jmo->theX = c;
	}
	if (c >= ' ' || c == '\n' || c == 0) {
		return c;
	}
	if (c == '\r') {
		return '\n';
	}
	return ' ';
}


/* jsmin_peek -- get the next character without getting it.
*/

static int
jsmin_peek(jsmin_obj *jmo)
{
	jmo->theLookahead = jsmin_get(jmo);
	return jmo->theLookahead;
}


/* jsmin_next -- get the next character, excluding comments. jsmin_peek() is used to see
		if a '/' is followed by a '/' or '*'.
*/

static int
jsmin_next(jsmin_obj *jmo)
{
	int c = jsmin_get(jmo);
	if (c == '/') {
		switch (jsmin_peek(jmo)) {
		case '/':
			for (;;) {
				c = jsmin_get(jmo);
				if (c <= '\n') {
					return c;
				}
			}
		case '*':
			jsmin_get(jmo);
			for (;;) {
				switch (jsmin_get(jmo)) {
				case '*':
					if (jsmin_peek(jmo) == '/') {
						jsmin_get(jmo);
						return ' ';
					}
					break;
				case 0:
					jsmin_error("unterminated comment", jmo);
					return;
				}
			}
		default:
			return c;
		}
	}
	return c;
}


/* jsmin_action -- do something! What you do is determined by the argument:
		1	Output A. Copy B to A. Get the next B.
		2	Copy B to A. Get the next B. (Delete A).
		3	Get the next B. (Delete B).
   jsmin_action treats a string as a single character. Wow!
   jsmin_action recognizes a regular expression if it is preceded by ( or , or =.
*/

static void
jsmin_action(int d, jsmin_obj *jmo)
{
	switch (d) {
	case 1:
		smart_str_appendc(jmo->buffer, jmo->theA);
		if (jmo->theA == jmo->theB && (jmo->theA == '+' || jmo->theA == '-') && jmo->theY != jmo->theA) {
			smart_str_appendc(jmo->buffer, ' ');
		}
	case 2:
		jmo->theA = jmo->theB;
		if (jmo->theA == '\'' || jmo->theA == '"' || jmo->theA == '`') {
			for (;;) {
				smart_str_appendc(jmo->buffer, jmo->theA);
				jmo->theA = jsmin_get(jmo);
				if (jmo->theA == jmo->theB) {
					break;
				}
				if (jmo->theA == '\\') {
					smart_str_appendc(jmo->buffer, jmo->theA);
					jmo->theA = jsmin_get(jmo);
				}
				if (jmo->theA == 0) {
					jsmin_error("unterminated string literal", jmo);
					return;
				}
			}
		}
	case 3:
		jmo->theB = jsmin_next(jmo);
		if (jmo->theB == '/' && (jmo->theA == '(' || jmo->theA == ',' || jmo->theA == '=' ||
							jmo->theA == ':' || jmo->theA == '[' || jmo->theA == '!' ||
							jmo->theA == '&' || jmo->theA == '|' || jmo->theA == '?' ||
							jmo->theA == '{' || jmo->theA == '}' || jmo->theA == ';' ||
							jmo->theA == '\n')) {
			smart_str_appendc(jmo->buffer, jmo->theA);
			smart_str_appendc(jmo->buffer, jmo->theB);
			for (;;) {
				jmo->theA = jsmin_get(jmo);
				if (jmo->theA == '[') {
					for (;;) {
						smart_str_appendc(jmo->buffer, jmo->theA);
						jmo->theA = jsmin_get(jmo);
						if (jmo->theA == ']') {
							break;
						}
						if (jmo->theA == '\\') {
							smart_str_appendc(jmo->buffer, jmo->theA);
							jmo->theA = jsmin_get(jmo);
						}
						if (jmo->theA == 0) {
							jsmin_error("unterminated set in Regular Expression literal", jmo);
							return;
						}
					}
				} else if (jmo->theA == '/') {
					break;
				} else if (jmo->theA =='\\') {
					smart_str_appendc(jmo->buffer, jmo->theA);
					jmo->theA = jsmin_get(jmo);
				}
				if (jmo->theA == 0) {
					jsmin_error("unterminated Regular Expression literal", jmo);
					return;
				}
				smart_str_appendc(jmo->buffer, jmo->theA);
			}
			jmo->theB = jsmin_next(jmo);
		}
	}
}


/* jsmin -- Copy the input to buffer, deleting the characters which are
		insignificant to JavaScript. Comments will be removed. Tabs will be
		replaced with spaces. Carriage returns will be replaced with linefeeds.
		Most spaces and linefeeds will be removed.
*/

char *
jsmin(char* javascript TSRMLS_DC)
{
	char *minified;
	jsmin_obj *jmo = new_jsmin_obj(javascript TSRMLS_CC);

	jmo->theA = '\n';
	jsmin_action(3, jmo);
	while (jmo->theA != 0) {
		if (jmo->isFailed) {
			break;
		}
		switch (jmo->theA) {
		case ' ':
			if (jsmin_isAlphanum(jmo->theB)) {
				jsmin_action(1, jmo);
			} else {
				jsmin_action(2, jmo);
			}
			break;
		case '\n':
			switch (jmo->theB) {
			case '{':
			case '[':
			case '(':
			case '+':
			case '-':
			case '!':
			case '~':
				jsmin_action(1, jmo);
				break;
			case ' ':
				jsmin_action(3, jmo);
				break;
			default:
				if (jsmin_isAlphanum(jmo->theB)) {
					jsmin_action(1, jmo);
				} else {
					jsmin_action(2, jmo);
				}
			}
			break;
		default:
			switch (jmo->theB) {
			case ' ':
				if (jsmin_isAlphanum(jmo->theA)) {
					jsmin_action(1, jmo);
					break;
				}
				jsmin_action(3, jmo);
				break;
			case '\n':
				switch (jmo->theA) {
				case '}':
				case ']':
				case ')':
				case '+':
				case '-':
				case '"':
				case '\'':
				case '`':
					jsmin_action(1, jmo);
					break;
				default:
					if (jsmin_isAlphanum(jmo->theA)) {
						jsmin_action(1, jmo);
					} else {
						jsmin_action(3, jmo);
					}
				}
				break;
			default:
				jsmin_action(1, jmo);
				break;
			}
		}
	}

	minified = estrndup(jmo->buffer->c, jmo->buffer->len);

	free_jsmin_obj(jmo TSRMLS_CC);

	return minified;
}
