#!/usr/bin/env sh

# Get and extract PHP
wget http://uk3.php.net/get/php-5.4.7.tar.bz2/from/this/mirror -O php-5.4.7.tar.bz2
tar -xf php-5.4.7.tar.bz2
cd php-5.4.7

# Clone into ext directory
git clone https://github.com/sqmk/pecl-jsmin.git ext/jsmin

# Build PHP
./buildconf --force
./configure --disable-all --enable-debug --enable-jsmin
make

# Run tests
TEST_PHP_EXECUTABLE=sapi/cli/php sapi/cli/php run-tests.php ext/jsmin  
