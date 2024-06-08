#include "test.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

#include "types.h"

static void test_i24();

void run_tests_and_exit()
{
    std::cout << "Running tests\n";
    test_i24();
    std::exit(EXIT_SUCCESS);
}

static void test_i24_positive();

static void test_i24()
{
    test_i24_positive();
}

static void test_i24_positive()
{
    Int24 i;

    i = {{0, 0, 0}};
    assert(i.to_i32() == 0);

    i = {{0x01, 0, 0}};
    assert(i.to_i32() == 1);

    i = {{0x10, 0, 0}};
    assert(i.to_i32() == 16);
}
