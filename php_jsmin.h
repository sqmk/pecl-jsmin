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

#ifndef PHP_JSMIN_H
#define PHP_JSMIN_H

#define PHP_JSMIN_VERSION "1.0.0"

#ifdef PHP_WIN32
#define PHP_JSMIN_API __declspec(dllexport)
#else
#define PHP_JSMIN_API
#endif

extern zend_module_entry jsmin_module_entry;
#define phpext_jsmin_ptr &jsmin_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(jsmin)
	int error_code;
ZEND_END_MODULE_GLOBALS(jsmin)

#ifdef ZTS
#define JSMIN_G(v) TSRMG(jsmin_globals_id, zend_jsmin_globals *, v)
#else
#define JSMIN_G(v) (jsmin_globals.v)
#endif

PHP_MINFO_FUNCTION(jsmin);
PHP_FUNCTION(jsmin);
PHP_FUNCTION(jsmin_last_error);
PHP_FUNCTION(jsmin_last_error_msg);

#endif  /* PHP_JSMIN_H */
