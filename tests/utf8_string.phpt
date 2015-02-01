--TEST--
Strings containing extended characters
--FILE--
<?php
$js = <<<JS
var test = "février"
JS;
echo jsmin($js);
--EXPECT--
var test="février"
