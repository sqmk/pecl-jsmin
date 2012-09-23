--TEST--
Call jsmin multiple times
--FILE--
<?php

$js1 = <<<JS
var test = 1 + 1; /* Testing */ alert('test');
JS;

$js2 = <<<JS
test();
JS;

echo jsmin($js1);
echo jsmin($js2);

--EXPECT--
var test=1+1;alert('test');
test();