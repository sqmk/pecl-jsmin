/* jsmin.h */

#ifndef JSMIN_H
#define JSMIN_H

#include "php.h"
#include "ext/standard/php_smart_string.h"

typedef struct {
	char *javascript;
	int   byte_len;
	smart_string buffer;
	int theA;
	int theB;
	int theLookahead;
	int theX;
	int theY;
	int errorCode;
} jsmin_obj;

enum error_codes {
  PHP_JSMIN_ERROR_NONE = 0,
  PHP_JSMIN_ERROR_UNTERMINATED_COMMENT,
  PHP_JSMIN_ERROR_UNTERMINATED_STRING,
  PHP_JSMIN_ERROR_UNTERMINATED_REGEX
};

extern jsmin_obj* jsmin(char *javascript TSRMLS_DC);
extern void free_jsmin_obj(jsmin_obj *jmo TSRMLS_DC);

#endif
