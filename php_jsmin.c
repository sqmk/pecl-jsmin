/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Michael Squires <sqmk@php.net>                               |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "jsmin.h"
#include "php_jsmin.h"

ZEND_DECLARE_MODULE_GLOBALS(jsmin)

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_jsmin, 0, 0, 1)
	ZEND_ARG_INFO(0, javascript)
	ZEND_ARG_INFO(1, return_code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_jsmin_last_error, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_jsmin_last_error_msg, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ jsmin_functions[] */
const zend_function_entry jsmin_functions[] = {
	PHP_FE(jsmin, arginfo_jsmin)
	PHP_FE(jsmin_last_error, arginfo_jsmin_last_error)
	PHP_FE(jsmin_last_error_msg, arginfo_jsmin_last_error_msg)
	PHP_FE_END
};
/* }}} */

/* {{{ MINIT */
static PHP_MINIT_FUNCTION(jsmin)
{
	REGISTER_LONG_CONSTANT("JSMIN_ERROR_NONE",                 PHP_JSMIN_ERROR_NONE,                 CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("JSMIN_ERROR_UNTERMINATED_COMMENT", PHP_JSMIN_ERROR_UNTERMINATED_COMMENT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("JSMIN_ERROR_UNTERMINATED_STRING",  PHP_JSMIN_ERROR_UNTERMINATED_STRING,  CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("JSMIN_ERROR_UNTERMINATED_REGEX",   PHP_JSMIN_ERROR_UNTERMINATED_REGEX,   CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_GINIT_FUNCTION
*/
static PHP_GINIT_FUNCTION(jsmin)
{
	jsmin_globals->error_code = 0;
}
/* }}} */

/* {{{ jsmin_module_entry
 */
zend_module_entry jsmin_module_entry = {
	STANDARD_MODULE_HEADER,
	"jsmin",
	jsmin_functions,
	PHP_MINIT(jsmin),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(jsmin),
	PHP_JSMIN_VERSION,
	PHP_MODULE_GLOBALS(jsmin),
	PHP_GINIT(jsmin),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_JSMIN
ZEND_GET_MODULE(jsmin)
#endif

/* {{{ proto string jsmin(string javascript)
   Minifies JavaScript */
PHP_FUNCTION(jsmin)
{
	char *javascript;
	int javascript_len;
	jsmin_obj *jmo;

	zval *ret_code = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z/", &javascript, &javascript_len, &ret_code) == FAILURE) {
		RETURN_FALSE;
	}

	jmo = jsmin(javascript TSRMLS_CC);
	if (ret_code) {
		zval_dtor(ret_code);
		ZVAL_LONG(ret_code, jmo->errorCode);
	}
	JSMIN_G(error_code) = jmo->errorCode;

	if (jmo->errorCode) {
		ZVAL_BOOL(return_value, 0);
	} else {
		ZVAL_STRINGL(return_value, jmo->buffer.c, jmo->buffer.len);
	}
	free_jsmin_obj(jmo TSRMLS_CC);
}
/* }}} */

/* {{{ proto int jsmin_last_error()
   Returns error code of last call to jsmin() */
PHP_FUNCTION(jsmin_last_error)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(JSMIN_G(error_code));
}
/* }}} */

/* {{{ proto string jsmin_last_error_msg()
   Returns error message of last call to jsmin() */
PHP_FUNCTION(jsmin_last_error_msg)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}


	switch (JSMIN_G(error_code)) {
		case PHP_JSMIN_ERROR_UNTERMINATED_COMMENT:
			RETURN_STRING("Unterminated comment");

		case PHP_JSMIN_ERROR_UNTERMINATED_STRING:
			RETURN_STRING("Unterminated string literal");

		case PHP_JSMIN_ERROR_UNTERMINATED_REGEX:
			RETURN_STRING("Unterminated set in Regular Expression literal");

		default:
			RETURN_STRING("No error");
	}
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(jsmin)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "jsmin support", "enabled");
	php_info_print_table_row(2, "jsmin version", PHP_JSMIN_VERSION);
	php_info_print_table_row(2, "jsmin notes", "Port of Douglas Crockford's JSMin");
	php_info_print_table_end();
}
/* }}} */
