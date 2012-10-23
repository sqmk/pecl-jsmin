#!/usr/bin/env sh

if [ $PHP_OLDER_AUTOCONF ]
then
    sudo apt-get install autoconf2.59 
    export PHP_AUTOCONF=`which autoconf-2.59`
fi

# PHP Settings
PHP_BASE="php-$PHP_VERSION"
PHP_PACKAGE="php-$PHP_VERSION.tar.bz2"
PHP_URL="http://us1.php.net/get/$PHP_PACKAGE/from/www.php.net/mirror"

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
