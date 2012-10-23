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
#include "jsmin.h"
#include "php_jsmin.h"

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_jsmin, 0, 0, 1)
	ZEND_ARG_INFO(0, javascript)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ jsmin_functions[] */
const zend_function_entry jsmin_functions[] = {
	PHP_FE(jsmin, arginfo_jsmin)
	PHP_FE_END
};
/* }}} */

/* {{{ jsmin_module_entry
 */
zend_module_entry jsmin_module_entry = {
	STANDARD_MODULE_HEADER,
	"jsmin",
	jsmin_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	PHP_MINFO(jsmin),
	PHP_JSMIN_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_JSMIN
ZEND_GET_MODULE(jsmin)
#endif

/* {{{ proto string jsmin(string javascript)
    */
PHP_FUNCTION(jsmin)
{
	char *javascript;
	int javascript_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &javascript, &javascript_len) == FAILURE) {
		return;
	}

	jsmin(javascript, return_value TSRMLS_CC);
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

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
