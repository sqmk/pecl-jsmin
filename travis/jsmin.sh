#!/usr/bin/env sh

# PHP Settings
PHP_BASE="php-$PHP_VERSION"
PHP_PACKAGE="php-$PHP_VERSION.tar.gz"
PHP_URL="http://us1.php.net/get/$PHP_PACKAGE/from/this/mirror"

# Move out of project 
cd ../

# Get and extract PHP
wget $PHP_URL -O $PHP_PACKAGE
tar -xf $PHP_PACKAGE 
cd $PHP_BASE 

# Clone into ext directory
mv ../pecl-jsmin ext/jsmin

# Build PHP
./buildconf --force
./configure --disable-all --enable-debug --enable-jsmin
make

# Run tests
export REPORT_EXIT_STATUS=1
export TEST_PHP_EXECUTABLE=sapi/cli/php
sapi/cli/php run-tests.php ext/jsmin  
