#!/usr/bin/env sh

# Move out of project 
cd ../

# Get and extract PHP
wget http://uk3.php.net/get/php-5.4.7.tar.bz2/from/this/mirror -O php-5.4.7.tar.bz2
tar -xf php-5.4.7.tar.bz2
cd php-5.4.7

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
