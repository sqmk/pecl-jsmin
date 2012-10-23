#!/usr/bin/env sh

# Create copy of project
cp -R . project-copy

# Get and extract PHP
wget http://uk3.php.net/get/php-5.4.7.tar.bz2/from/this/mirror -O php-5.4.7.tar.bz2
tar -xf php-5.4.7.tar.bz2
cd php-5.4.7

# Clone into ext directory
mv ../project-copy ext/jsmin

# Build PHP
./buildconf --force
./configure --disable-all --enable-debug --enable-silent-rules --enable-jsmin
make -s

# Run tests
TEST_PHP_EXECUTABLE=sapi/cli/php sapi/cli/php run-tests.php ext/jsmin  
