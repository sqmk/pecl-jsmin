/* json.h */

#ifndef JSMIN_H
#define JSMIN_H

#include "php.h"
#include "ext/standard/php_smart_str.h"

extern void* jsmin(char *javascript, zval *return_value TSRMLS_DC);
#endif
