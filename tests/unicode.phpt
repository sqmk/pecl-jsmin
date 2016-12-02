--TEST--
Strings containing extended characters
--FILE--
<?php
$js = <<<JS
var test = "févrie"
JS;
echo jsmin($js);
--EXPECT--
var test="février"
