# jsmin: PHP extension for JavaScript minification

Master: [![Build Status](https://secure.travis-ci.org/sqmk/pecl-jsmin.png)](http://travis-ci.org/sqmk/pecl-jsmin)

## Introduction

This extension adds Douglas Crockford's jsmin functionality to PHP.

http://www.crockford.com/javascript/jsmin.html

## Requirements

This extension currently works with all versions of PHP 5.3 and PHP 5.4. Support for older verisons of PHP is not provided.

## Installation

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

Using jsmin is simple.

### Function: jsmin

Use this function to minify JavaScript. It accepts a single string argument.

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

Example output is the following:

```text
var notes="jsmin is easy!";
```

### Function: jsmin_last_error

Returns error code of last call to jsmin().

### Function: jsmin_last_error_msg

Returns an error message (string) for the last call to jsmin().

### Constants

* JSMIN_ERROR_NONE - No errors.
* JSMIN_ERROR_UNTERMINATED_STRING - Unterminated string.
* JSMIN_ERROR_UNTERMINATED_COMMENT - Unterminated comment.
* JSMIN_ERROR_UNTERMINATED_REGEX - Unterminated regex.

# Credits

[Discovery Communications](http://discovery.com) developed a similar extension in-house for minifying bundled JavaScript.

I decided to take the most recent source from [Douglas Crockford's JSMin](/douglascrockford/JSMin) and port / manage the extension for PECL.
