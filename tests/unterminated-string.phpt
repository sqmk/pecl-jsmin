--TEST--
Warning message on unterminated string
--FILE--
<?php

$js = <<<JS
var str = "Test
JS;

echo jsmin($js);

--EXPECTF--
Warning: jsmin() unterminated string literal in %s on line %d
