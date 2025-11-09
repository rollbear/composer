#!/bin/bash

#
# composer C++23 library for functional composition
#
# Copyright (C) Björn Fahller
#
#  Use, modification and distribution is subject to the
#  Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#
# Project home: https://github.com/rollbear/composer
#

git ls-files '*.[ch]pp'|xargs clang-format-21 --dry-run -Werror
exit $?
