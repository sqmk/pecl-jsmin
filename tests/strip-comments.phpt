--TEST--
Strip extraneous spaces and comments
--FILE--
<?php

$js = <<<JS
/**
 * Test comments
 */

alert('test');
JS;

echo jsmin($js);

--EXPECT--

alert('test');