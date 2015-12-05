/* jsmin.c
   2013-03-29

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
#include "utf8.h"
#include "jsmin.h"

#define isutf(c) (((c)&0xC0)!=0x80)

/* new_jsmin_obj -- sets up new struct for assisting in minification
*/

static jsmin_obj*
new_jsmin_obj(char *javascript TSRMLS_DC)
{
	jsmin_obj *jmo	= ecalloc(1, sizeof(jsmin_obj));
	jmo->javascript = javascript;
	memset(&jmo->buffer, 0, sizeof(smart_string));
	jmo->theA		= '\n';
	jmo->errorCode	= 0;

	return jmo;
}

/* free_jsmin_obj -- frees up memory on struct
*/
void
free_jsmin_obj(jsmin_obj *jmo TSRMLS_DC)
{
	smart_string_free(&jmo->buffer);
	efree(jmo);
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
		int byte_len = 0;
		c = u8_nextchar(jmo->javascript, &byte_len);
		jmo->javascript += byte_len;
		jmo->byte_len = byte_len;
	}
	if (c >= ' ' || c == '\n' || c == 0) {
		return c;
	}
	if (c == '\r') {
		return '\n';
	}
	if (isutf(c)) {
		return c;
	}
	return ' ';
}


/** jsmin_peek -- get the next character without getting it.
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
					break;
				}
			}
			break;
		case '*':
			jsmin_get(jmo);
			while (c != ' ') {
				switch (jsmin_get(jmo)) {
				case '*':
					if (jsmin_peek(jmo) == '/') {
						jsmin_get(jmo);
						c = ' ';
					}
					break;
				case 0:
					jmo->errorCode = PHP_JSMIN_ERROR_UNTERMINATED_COMMENT;
					c = ' ';
				}
			}
		default:
			break;
		}
	}
	jmo->theY = jmo->theX;
	jmo->theX = c;
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
		smart_string_appendc(&jmo->buffer, jmo->theA);
		if (
			(jmo->theY == '\n' || jmo->theY == ' ') &&
			(jmo->theA == '+' || jmo->theA == '-' || jmo->theA == '*' || jmo->theA == '/') &&
			(jmo->theB == '+' || jmo->theB == '-' || jmo->theB == '*' || jmo->theB == '/')
		) {
			smart_string_appendc(&jmo->buffer, jmo->theY);
		}
	case 2:
		jmo->theA = jmo->theB;
		if (jmo->theA == '\'' || jmo->theA == '"' || jmo->theA == '`') {
			for (;;) {
				char wc_bytes[4];
				int bytes = u8_wc_toutf8(wc_bytes, jmo->theA);
				smart_string_appendl_ex(&jmo->buffer, wc_bytes, bytes, 0);
				jmo->theA = jsmin_get(jmo);
				if (jmo->theA == jmo->theB) {
					break;
				}
				if (jmo->theA == '\\') {
					smart_string_appendc(&jmo->buffer, jmo->theA);
					jmo->theA = jsmin_get(jmo);
				}
				if (jmo->theA == 0) {
					jmo->errorCode = PHP_JSMIN_ERROR_UNTERMINATED_STRING;
					return;
				}
			}
		}
	case 3:
		jmo->theB = jsmin_next(jmo);
		if (jmo->theB == '/' && (jmo->theA == '(' || jmo->theA == ',' || jmo->theA == '=' ||
							jmo->theA == ':' || jmo->theA == '[' || jmo->theA == '!' ||
							jmo->theA == '&' || jmo->theA == '|' || jmo->theA == '?' ||
							jmo->theA == '+' || jmo->theA == '-' || jmo->theA == '~' ||
							jmo->theA == '*' || jmo->theA == '/' || jmo->theA == '{' || 
							jmo->theA == '\n')) {
			smart_string_appendc(&jmo->buffer, jmo->theA);
			if (jmo->theA == '/' || jmo->theA == '*') {
				smart_string_appendc(&jmo->buffer, ' ');
			}
			smart_string_appendc(&jmo->buffer, jmo->theB);
			for (;;) {
				jmo->theA = jsmin_get(jmo);
				if (jmo->theA == '[') {
					for (;;) {
						smart_string_appendc(&jmo->buffer, jmo->theA);
						jmo->theA = jsmin_get(jmo);
						if (jmo->theA == ']') {
							break;
						}
						if (jmo->theA == '\\') {
							smart_string_appendc(&jmo->buffer, jmo->theA);
							jmo->theA = jsmin_get(jmo);
						}
						if (jmo->theA == 0) {
							jmo->errorCode = PHP_JSMIN_ERROR_UNTERMINATED_REGEX;
							return;
						}
					}
				} else if (jmo->theA == '/') {
					switch (jsmin_peek(jmo)) {
					case '/':
					case '*':
						jmo->errorCode = PHP_JSMIN_ERROR_UNTERMINATED_REGEX;
						return;
					}
					break;
				} else if (jmo->theA =='\\') {
					smart_string_appendc(&jmo->buffer, jmo->theA);
					jmo->theA = jsmin_get(jmo);
				}
				if (jmo->theA == 0) {
					jmo->errorCode = PHP_JSMIN_ERROR_UNTERMINATED_REGEX;
					return;
				}
				smart_string_appendc(&jmo->buffer, jmo->theA);
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

jsmin_obj*
jsmin(char *javascript TSRMLS_DC)
{
	jsmin_obj *jmo = new_jsmin_obj(javascript TSRMLS_CC);

	jsmin_action(3, jmo);
	while (jmo->theA != 0) {
		if (jmo->errorCode) {
			break;
		}
		switch (jmo->theA) {
		case ' ':
			jsmin_action(jsmin_isAlphanum(jmo->theB) ? 1 : 2, jmo);
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
				jsmin_action(jsmin_isAlphanum(jmo->theB) ? 1 : 2, jmo);
			}
			break;
		default:
			switch (jmo->theB) {
			case ' ':
				jsmin_action(jsmin_isAlphanum(jmo->theA) ? 1 : 3, jmo);
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
					jsmin_action(jsmin_isAlphanum(jmo->theA) ? 1 : 3, jmo);
				}
				break;
			default:
				jsmin_action(1, jmo);
				break;
			}
		}
	}

	return jmo;
}
