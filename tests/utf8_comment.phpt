--TEST--
skip utf8 comment
--FILE--
<?php
$js = <<<JS
// févrie
var test = 1;
JS;
echo jsmin($js);
--EXPECT--
var test=1;
