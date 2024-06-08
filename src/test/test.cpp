#include "test.h"

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
static void test_i24_negative();

static void try_i24(Int24 i, i32 expected);

static void test_i24()
{
    test_i24_positive();
    test_i24_negative();
}

static void test_i24_positive()
{
    Int24 i;

    i = {{0, 0, 0}};
    try_i24(i, 0);

    i = {{0x01, 0, 0}};
    try_i24(i, 1);

    i = {{0x10, 0, 0}};
    try_i24(i, 16);

    i = {{0xFF, 0xFF, 0x7F}};  // int max
    try_i24(i, (1 << 23) - 1);
}

static void test_i24_negative()
{
    Int24 i;

    i = {{0xFF, 0xFF, 0xFF}};
    try_i24(i, -1);
}

static void try_i24(Int24 i, i32 expected)
{
    if (i.to_i32() != expected)
    {
        std::cout << "Error, expected " << expected << " was " << i.to_i32()
                  << std::endl;
        std::exit(1);
    }
    std::cout << "Ok for " << expected << std::endl; 
}
