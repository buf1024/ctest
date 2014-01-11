/*
 * main.c
 *
 *  Created on: 2013-12-28
 *      Author: Luo Guochun
 */
#define C_TEST_APP

#include "ctest.h"


TEST(tf2, tc)
{
    printf("tf2.atc\n");
}
TEST(tf3, atc)
{
    printf("tf3.atc\n");
}
TEST(tf2, 1tc)
{
    printf("tf2.1tc\n");
}
TEST(tf2, ctc)
{
    printf("tf2.ctc\n");
}
TEST(tf2, btc)
{
    printf("tf2.btc\n");
}
TEST(tf2, etc)
{
    printf("tf2.etc\n");
}
TEST(tf, tc)
{
    printf("tf.tc\n");
    EXPECT_EQ(1, 2.0);
    EXPECT_NEQ(1, 1);
    EXPECT_STREQ("abc", "bcd");
    EXPECT_STRNEQ("abc", "abc");
}
TEST(tf, tc1)
{
    printf("tf.tc1\n");
    EXPECT_EQ(1, 2.0);
    EXPECT_NEQ(1, 1);
    EXPECT_STREQ("abc", "bcd");
    EXPECT_STRNEQ("abc", "abc");
}
TEST(tf, tc2)
{
    printf("tf.tc2\n");
    EXPECT_EQ(1, 2.0);
    EXPECT_NEQ(1, 1);
    EXPECT_STREQ("abc", "bcd");
    EXPECT_STRNEQ("abc", "abc");
}

TEST(tf, tc3)
{
    printf("tf.tc3\n");
    EXPECT_EQ(1, 2.0);
    EXPECT_NEQ(1, 1);
    EXPECT_STREQ("abc", "bcd");
    EXPECT_STRNEQ("abc", "abc");
}

void setup_env()
{
    printf("%s\n", __func__);
}

void teardown_env()
{
    printf("%s\n", __func__);
}
SET_ENVIRONMENT(setup_env, teardown_env);

int main(int argc, char **argv)
{
    INIT_TEST(argc, argv);
    RUN_ALL_TEST();


    return 0;
}
