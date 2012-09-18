--TEST--
Return value is type string on empty arg
--FILE--
<?php

// Try null
echo gettype(jsmin(""));

--EXPECT--
string