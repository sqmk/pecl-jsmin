--TEST--
Last error is unterminated string
--FILE--
<?php

$js = <<<JS
var str = "Test
JS;

echo jsmin($js);

if (jsmin_last_error() == JSMIN_ERROR_UNTERMINATED_STRING) {
	echo jsmin_last_error_msg();
}

--EXPECTF--
Unterminated string literal
