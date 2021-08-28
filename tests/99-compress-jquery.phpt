--TEST--
Test jsmin with jquery
--SKIPIF--
<?php
if (!file_exists(__DIR__ . '/data/jquery-2.1.3.js')) die('skip no jquery found');
?>
--FILE--
<?php
$js = file_get_contents(__DIR__ . '/data/jquery-2.1.3.js');
$out = jsmin($js);
echo jsmin_last_error();
--EXPECT--
0
