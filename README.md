This extension adds Douglas Crockford's jsmin functionality to PHP.

http://www.crockford.com/javascript/jsmin.html

## Installing

To install, download the source and build as follows:

```
cd /path/to/source
phpize
./configure
make install clean
```

Then, move the built module to your extensions directory, and enable the extension in php.ini by adding:

```
extension="jsmin.so"
```

## Usage

Using jsmin is simple. Here is an example:

```php
<?php

$javascript = <<<JS
/**
 * My JavaScript Library
 */

var notes = "jsmin is easy!";
JS;

echo jsmin($javascript);
```

And example output is the following:

```php
var notes="jsmin is easy!";
```

### Credits

[Discovery Communications](http://discovery.com) developed a similar extension in-house for minifying bundled JavaScript.

I decided to take the most recent source from [Douglas Crockford's JSMin](/douglascrockford/JSMin) and port / manage the extension for PECL.
