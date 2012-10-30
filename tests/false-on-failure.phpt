--TEST--
Expect boolean false on failure to minify
--FILE--
<?php

$js = <<<JS
alert('test');
/* Deliberate unterminated comment

alert('test');
JS;

if (jsmin($js) === false) {
	echo "is boolean false";
}

--EXPECTF--
is boolean false