/*
 * atest.c
 *
 *  Created on: 2013-12-31
 *      Author: Luo Guochun
 */

#include "ctest.h"

TEST(spc, t)
{
    printf("aaaaaaaaaaaaaaaaaaaaaaaffffffaaaaaaaaaatf.tc3\n");/*
    EXPECT_EQ(1, 2.0);
    EXPECT_NEQ(1, 1);
    EXPECT_STREQ("abc", "bcd");
    EXPECT_STRNEQ("abc", "abc");*/
}

void setup_testcase()
{
    printf("%s\n", __func__);
}

void teardown_testcase()
{
    printf("%s\n", __func__);
}

void setup()
{
    printf("%s\n", __func__);
}

void teardown()
{
    printf("%s\n", __func__);
}

SETUP_TEST(spc, setup_testcase, teardown_testcase, setup, teardown)
