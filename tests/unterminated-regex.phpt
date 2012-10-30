--TEST--
Last error is unterminated regular expression
--FILE--
<?php

$js = <<<JS
var str = /test regular expression
JS;

echo jsmin($js);

if (jsmin_last_error() == JSMIN_ERROR_UNTERMINATED_REGEX) {
	echo jsmin_last_error_msg();
}

--EXPECTF--
Unterminated set in Regular Expression literal
