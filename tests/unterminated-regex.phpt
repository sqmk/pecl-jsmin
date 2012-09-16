--TEST--
Warning message on unterminated regular expression
--FILE--
<?php

$js = <<<JS
var str = /test regular expression
JS;

echo jsmin($js);

--EXPECTF--
Warning: jsmin() unterminated Regular Expression literal in %s on line %d

var str=/test regular expression
