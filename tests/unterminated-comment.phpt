--TEST--
Last error is unterminated comment
--FILE--
<?php

$js = <<<JS
alert('test');
/* Deliberate unterminated comment

alert('test');
JS;

echo jsmin($js);

if (jsmin_last_error() == JSMIN_ERROR_UNTERMINATED_COMMENT) {
	echo jsmin_last_error_msg();
}

--EXPECTF--
Unterminated comment
