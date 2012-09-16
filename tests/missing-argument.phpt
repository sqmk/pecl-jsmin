--TEST--
Missing argument throws warning
--FILE--
<?php

echo jsmin();

--EXPECTF--
Warning: jsmin() expects exactly 1 parameter, 0 given in %s on line %d