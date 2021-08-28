--TEST--
Missing argument throws warning
--SKIPIF--
<?php
if (PHP_VERSION_ID >= 80000) die('skip Only for PHP < 8.0');
?>
--FILE--
<?php

echo jsmin();

--EXPECTF--
Warning: jsmin() expects at least 1 parameter, 0 given in %s on line %d
