--TEST--
Missing argument throws fatal error
--SKIPIF--
<?php
if (PHP_VERSION_ID < 80000) die('skip Only for PHP >= 8.0');
?>
--FILE--
<?php

echo jsmin();

--EXPECTF--
Fatal error: Uncaught ArgumentCountError: jsmin() expects at least 1 argument, 0 given in %s
Stack trace:
#0 %s(3): jsmin()
#1 {main}
  thrown in %s on line 3
