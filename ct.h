/*
 * ct.h -- very very simple c test framework
 *
 * The way of using if is much alike gtest.
 *
 * You must define USING_C_TEST macro in the c file which include the
 * main function. I think very hard of avoid using this USING_C_TEST
 * macro, But I failed. if I declare all the function in this file to
 * static, It can compile well(after modifying the __ct_test function),
 * and run well, But when you look inside the finally binary(using nm
 * utility), you will see lots of duplicate symbol, that is not the
 * way I want Any one who knows avoid using the macro, please feel free
 * to tell me(buf1024@gmail.com). thanks a lot
 *
 *  Luo Guochun 2013-12-31
 */

#ifndef __CT_H__
#define __CT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////
// data structure from sys/queue.h
/*
 * Singly-linked List definitions.
 */
#define __CT_SLIST_HEAD(name, type)                                  \
struct name {                               \
    struct type *slh_first; /* first element */         \
}

#define __CT_SLIST_HEAD_INITIALIZER(head)                    \
    { NULL }

#define __CT_SLIST_ENTRY(type)                       \
struct {                                \
    struct type *sle_next;  /* next element */          \
}

/*
 * Singly-linked List access methods.
 */
#define __CT_SLIST_FIRST(head)   ((head)->slh_first)
#define __CT_SLIST_END(head)     NULL
#define __CT_SLIST_EMPTY(head)   (__CT_SLIST_FIRST(head) == __CT_SLIST_END(head))
#define __CT_SLIST_NEXT(elm, field)  ((elm)->field.sle_next)

#define __CT_SLIST_FOREACH(var, head, field)                 \
    for((var) = __CT_SLIST_FIRST(head);                  \
        (var) != __CT_SLIST_END(head);                   \
        (var) = __CT_SLIST_NEXT(var, field))

/*
 * Singly-linked List functions.
 */
#define __CT_SLIST_INIT(head) {                      \
    __CT_SLIST_FIRST(head) = __CT_SLIST_END(head);                \
}

#define __CT_SLIST_INSERT_AFTER(slistelm, elm, field) do {           \
    (elm)->field.sle_next = (slistelm)->field.sle_next;     \
    (slistelm)->field.sle_next = (elm);             \
} while (0)

#define __CT_SLIST_INSERT_HEAD(head, elm, field) do {            \
    (elm)->field.sle_next = (head)->slh_first;          \
    (head)->slh_first = (elm);                  \
} while (0)

#define __CT_SLIST_REMOVE_HEAD(head, field) do {             \
    (head)->slh_first = (head)->slh_first->field.sle_next;      \
} while (0)

#define __CT_SLIST_REMOVE_AFTER(slistelm, elm, field) do {             \
    (slistelm)->field.sle_next = (elm)->field.sle_next;      \
    (elm)->field.sle_next = NULL; \
} while (0)

#define __CT_SLIST_ENTRY(type)                       \
struct {                                \
    struct type *sle_next;  /* next element */          \
}
//////////////////////////////////////////////////////////////////////
#define __ABORT_IF_NO_MONEY(m) do{                                   \
        if((m) == NULL) {                                            \
            printf("no memory!\n");                                  \
            abort();                                                 \
        }                                                            \
    }while(0);

#define __gen_symbol_2(tf, tc, no)                                   \
        void __##tf##__##tc##__ ## no()

#define __gen_symbol(tf, tc)                                         \
    __gen_symbol_2(tf, tc, __LINE__)

#define __discard_func_warning(func) (void)(func)

typedef struct __test_case __test_case_t;
typedef struct __test_fixture __test_fixture_t;
typedef struct __test_filter __test_filter_t;
typedef struct __test_case_entry __test_case_entry_t;
typedef struct __test_fixture_entry __test_fixture_entry_t;
typedef struct __test_filter_entry __test_filter_entry_t;

typedef __CT_SLIST_HEAD(__test_caseq, __test_case_entry) __test_caseq_t;
typedef __CT_SLIST_HEAD(__test_fixtureq, __test_fixture_entry) __test_fixtureq_t;
typedef __CT_SLIST_HEAD(__test_filterq, __test_filter_entry) __test_filterq_t;

typedef struct __test __test_t;

struct __test_case
{
    char name[256];
    char tf[128];
    char tc[128];
    void (*test)();

    __test_fixture_t* fixture;
};

struct __test_fixture
{
    char name[256];

    void (*setup_testcase)();
    void (*teardown_testcase)();
    void (*setup)();
    void (*teardown)();

    int size;
    __test_caseq_t* caseq;
};

struct __test_case_entry
{
    __CT_SLIST_ENTRY(__test_case_entry) entry;
    __test_case_t* test;
};

struct __test_fixture_entry
{
    __CT_SLIST_ENTRY(__test_fixture_entry) entry;
    __test_fixture_t* test;
};

struct __test_filter
{
    int flag:1; // 0 positive, 1 negative
    char filter[256];
};
struct __test_filter_entry
{
    __CT_SLIST_ENTRY(__test_filter_entry) entry;
    __test_filter_t* filter;
};

struct __test
{
    int mode; // run 0 help 1 list_tests 2 filter 3 unknown 4

    void (*setup_env)();
    void (*teardown_env)();

    int r_size;
    int f_size;
    __test_fixtureq_t* run;
    __test_fixtureq_t* failed;
    __test_filterq_t* filter;
};

//////////////////////////////////////////////////////////////////////
// functions
char* __ct_hex_dump(void*, int, char*);
int __ct_match(const char*, const char*);
void __ct_set_env(void (*)(), void (*)());
__test_fixture_t* __ct_get_fixture(const char*, int); // 0 run, 1 failed
void __ct_register_test(__test_case_t*);
void __ct_register_fixture(__test_fixture_t*);
void __ct_add_failed_test(const char*);
void __ct_help();
void __ct_list_tests();
void __ct_init_test(int, char**);
int __ct_match_filter(const char*);
int __ct_run();
int __ct_run_test();
__test_t* __ct_test();
int __ct_get_fixture_count();
int __ct_get_test_count();
int __ct_is_test_failed(const char* tf, const char* tc);

#define __ct_get_hex(ch) ((0x0F & (ch)) >= 10?'A' + (ch) - 10:'0' + (ch))

#ifdef USING_C_TEST
char* __ct_hex_dump(void* src, int size, char* dst)
{
    if(src)
    {
        memset(dst, 0, size*2 + 2);
        const char* tmp = (const char*)src;
        int tmp_size = 0;
        int dst_idx = 0;
        while(tmp_size < size){
            if(tmp_size == 0)
            {
                memcpy(dst + dst_idx, "0x", 2);
                dst_idx += 2;
            }
            char ch = *(tmp + tmp_size);
            dst[dst_idx++] = __ct_get_hex((char)((0xF0 & ch) >> 4));
            dst[dst_idx++] = __ct_get_hex((char)(0x0F & ch));
            tmp_size++;
        }
        return dst;
    }
    return NULL;
}

/*
 * Routine to see if a text string is matched by a wildcard pattern.
 * Returns 1 if the text is matched, or 0 if it is not matched
 * or if the pattern is invalid.
 *  *        matches zero or more characters
 *  ?        matches a single character
 *  [abc]    matches 'a', 'b' or 'c'
 *  \c       quotes character c
 *  Adapted from code written by Ingo Wilken.
 */
int __ct_match(const char * text, const char * pattern)
{
    const char * retryPat;
    const char * retryText;
    int ch;
    int found;

    retryPat = NULL;
    retryText = NULL;

    while (*text || *pattern) {
        ch = *pattern++;

        switch (ch) {
        case '*':
            retryPat = pattern;
            retryText = text;
            break;

        case '[':
            found = 0;

            while ((ch = *pattern++) != ']') {
                if (ch == '\\')
                    ch = *pattern++;

                if (ch == '\0')
                    return 0;

                if (*text == ch)
                    found = 1;
            }

            if (!found) {
                pattern = retryPat;
                text = ++retryText;
            }

            if (*text++ == '\0')
                return 0;

            break;

        case '?':
            if (*text++ == '\0')
                return 0;

            break;

        case '\\':
            ch = *pattern++;

            if (ch == '\0')
                return 0;

            if (*text == ch) {
                if (*text)
                    text++;
                break;
            }

            if (*text) {
                pattern = retryPat;
                text = ++retryText;
                break;
            }

            return 0;

        default:
            if (*text == ch) {
                if (*text)
                    text++;
                break;
            }

            if (*text) {
                pattern = retryPat;
                text = ++retryText;
                break;
            }

            return 0;
        }

        if (pattern == NULL )
            return 0;
    }

    return 1;
}

__test_t* __ct_test()
{
    static __test_t* t = NULL;
    if(t == NULL) {

        t = (__test_t*)malloc(sizeof(*t));
        __ABORT_IF_NO_MONEY(t);
        memset(t, 0, sizeof(*t));
        t->run = (__test_fixtureq_t*)malloc(sizeof(*t->run));
        __ABORT_IF_NO_MONEY(t->run);
        t->failed = (__test_fixtureq_t*)malloc(sizeof(*t->failed));
        __ABORT_IF_NO_MONEY(t->failed);
        t->filter = (__test_filterq_t*)malloc(sizeof(*t->filter));
        __ABORT_IF_NO_MONEY(t->filter);
    }
    return t;
}

void __ct_set_env(void (*up)(), void (*down)())
{
    __test_t* t = __ct_test();
    t->setup_env = up;
    t->teardown_env = down;
}

__test_fixture_t* __ct_get_fixture(const char* tf, int type) // 0 run, 1 failed
{

    __test_t* t = __ct_test();

    __test_fixtureq_t* q = t->run;
    if(type == 1) {
        q = t->failed;
    }

    __test_fixture_entry_t* ent = NULL;
    __CT_SLIST_FOREACH(ent, q, entry){
        if(strcmp(ent->test->name, tf) == 0) {
            return ent->test;
        }
    }
    ent = (__test_fixture_entry_t*)malloc(sizeof(*ent));
    __ABORT_IF_NO_MONEY(ent);
    memset(ent, 0, sizeof(*ent));

    __test_fixture_t* f = (__test_fixture_t*)malloc(sizeof(*f));
    __ABORT_IF_NO_MONEY(f);
    memset(f, 0, sizeof(*f));

    f->caseq = (__test_caseq_t*)malloc(sizeof(*f->caseq));
    __ABORT_IF_NO_MONEY(f->caseq);
    memset(f->caseq, 0, sizeof(*f->caseq));

    strcpy(f->name, tf);

    ent->test = f;

    if(__CT_SLIST_EMPTY(q)) {
        __CT_SLIST_INSERT_HEAD(q, ent, entry);
    }else{
        __test_fixture_entry_t* fent = NULL;
        __test_fixture_entry_t* prent = NULL;
        __test_fixture_t* finfo = NULL;
        __CT_SLIST_FOREACH(fent, q, entry){
            finfo = fent->test;
            if(strcmp(finfo->name, tf) > 0) {
                break;
            }
            prent = fent;
        }
        if(prent == NULL) {
            __CT_SLIST_INSERT_HEAD(q, ent, entry);
        }else{
            __CT_SLIST_INSERT_AFTER(prent, ent, entry);
        }
    }
    t->r_size++;
    return f;
}

void __ct_register_test(__test_case_t* test)
{
    __test_case_entry_t* ent = (__test_case_entry_t*)malloc(sizeof(*ent));
    __ABORT_IF_NO_MONEY(ent);
    memset(ent, 0, sizeof(*ent));

    __test_case_t* info = (__test_case_t*)malloc(sizeof(*info));
    __ABORT_IF_NO_MONEY(info);
    memset(info, 0, sizeof(*info));

    ent->test = info;
    memcpy(info->name, test->name, sizeof(test->name));
    memcpy(info->tf, test->tf, sizeof(test->tf));
    memcpy(info->tc, test->tc, sizeof(test->tc));
    info->test = test->test;

    __test_fixture_t* f = __ct_get_fixture(test->tf, 0);
    info->fixture = f;

    if(__CT_SLIST_EMPTY(f->caseq)) {
        __CT_SLIST_INSERT_HEAD(f->caseq, ent, entry);
    }else {
        __test_case_entry_t* fent = NULL;
        __test_case_entry_t* prent = NULL;
        __test_case_t* finfo = NULL;
        __CT_SLIST_FOREACH(fent, f->caseq, entry) {
            finfo = fent->test;
            if(strcmp(finfo->name, info->name) > 0) {
                break;
            }
            prent = fent;
        }
        if(prent == NULL) {
            __CT_SLIST_INSERT_HEAD(f->caseq, ent, entry);
        } else {
            __CT_SLIST_INSERT_AFTER(prent, ent, entry);
        }
    }
    f->size++;
}
void __ct_register_fixture(__test_fixture_t* test)
{
    __test_fixture_t* f = __ct_get_fixture(test->name, 0);

    memcpy(f->name, test->name, sizeof(test->name));
    f->setup = test->setup;
    f->teardown = test->teardown;
    f->setup_testcase = test->setup_testcase;
    f->teardown_testcase = test->teardown_testcase;
}

void __ct_add_failed_test(const char* tn)
{
    const char* fix = "__test__";
    const char* mid = "__";
    const int fix_len = strlen(fix);
    const int mid_len = strlen(mid);

    const char* p = tn;
    const char* tmp = NULL;
    char tf[256] = {0};
    char tc[256] = {0};

    if((p = strstr(p, fix)) != NULL){
        p += fix_len;
        tmp = p;

        if((p = strstr(p, mid)) != NULL){
            memcpy(tf, tmp, p - tmp);
            p += mid_len;
            tmp = p;

            if((p = strstr(p, fix)) != NULL){
                memcpy(tc, tmp, p - tmp);
            }
        }
        __test_fixture_t* f = __ct_get_fixture(tf, 1);

        __test_case_entry_t* ent = (__test_case_entry_t*)malloc(sizeof(*ent));
        __ABORT_IF_NO_MONEY(ent);
        memset(ent, 0, sizeof(*ent));

        __test_case_t* info = (__test_case_t*)malloc(sizeof(*info));
        __ABORT_IF_NO_MONEY(info);
        memset(info, 0, sizeof(*info));

        ent->test = info;
        snprintf(info->name, sizeof(info->name) - 1, "%s.%s", tf, tc);
        strcpy(info->tf, tf);
        strcpy(info->tc, tc);

        __test_t* t = __ct_test();

        if(__CT_SLIST_EMPTY(f->caseq)) {
            __CT_SLIST_INSERT_HEAD(f->caseq, ent, entry);
            t->f_size++;
        }else {
            __test_case_entry_t* fent = NULL;
            __test_case_entry_t* prent = NULL;
            __test_case_t* finfo = NULL;
            int cmp = 0;
            __CT_SLIST_FOREACH(fent, f->caseq, entry) {
                finfo = fent->test;
                cmp = strcmp(finfo->name, info->name);
                if(cmp >= 0) {
                    break;
                }
                prent = fent;
            }
            if(cmp != 0) {
                if(prent == NULL) {
                    __CT_SLIST_INSERT_HEAD(f->caseq, ent, entry);
                } else {
                    __CT_SLIST_INSERT_AFTER(prent, ent, entry);
                }
                t->f_size++;
            }
        }
    }

}
int __ct_is_test_failed(const char* tf, const char* tc)
{
    __test_t* t = __ct_test();

    __test_fixture_entry_t* fent = NULL;
    __CT_SLIST_FOREACH(fent, t->failed, entry) {
        if(strcmp(fent->test->name, tf) == 0) {
            __test_case_entry_t* cent = NULL;
            __CT_SLIST_FOREACH(cent, fent->test->caseq, entry) {
                if(strcmp(cent->test->tc, tc) == 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void __ct_help()
{
    printf(
            "Very Very Simple C Test Framework Help Message\n\n"
            "--help\n"
            "   Print this help messages\n\n"
            "--list_tests\n"
            "   List the names of all tests instead of running them. The name of\n"
            "   TEST(Foo, Bar) is \"Foo.Bar\".\n\n"
            "--filter=POSTIVE_PATTERNS[-NEGATIVE_PATTERNS]\n"
            "   Run only the tests whose name matches one of the positive patterns but\n"
            "   none of the negative patterns. '?' matches any single character; '*'\n"
            "   matches any substring; ':' separates two patterns.\n\n");
}
void __ct_list_tests()
{
    __test_t* t = __ct_test();
    __test_fixtureq_t* q = t->run;
    if(__CT_SLIST_EMPTY(q)){
        printf("There is no test!!\n");
    }else{
        printf("Following is all the tests\n"
                "====================================\n\n");
        __test_fixture_entry_t* ent = NULL;
        __CT_SLIST_FOREACH(ent, q, entry) {
            printf("%s.\n", ent->test->name);
            __test_case_entry_t* cent = NULL;
            __CT_SLIST_FOREACH(cent, ent->test->caseq, entry){
                printf("  %s\n", cent->test->tc);
            }
        }
        printf("\n");
    }
}

void __ct_init_test(int argc, char** argv)
{
    __test_t* t = __ct_test();

    int i = 1;
    for(; i<argc; i++){
        char* p = argv[i];
        char* ft = NULL;
        if(strstr(p, "--help") != NULL){
            t->mode = 1;
        }else if(strstr(p, "--list_tests") != NULL){
            t->mode = 2;
        }else if((ft = strstr(p, "--filter")) != NULL){
            t->mode = 3;
            if((ft = strstr(ft, "=")) == NULL){
                t->mode = 4;
            }else{
                ft += 1;
                printf("XXXXXXXX!!!PLEASE NOTE!!!XXXXXXXX ==> --filter=%s\n", ft);
                do {
                    if(strlen(ft) == 0 || (strlen(ft) == 1 && *ft == '-')) {
                        t->mode = 4;
                        break;
                    } else {
                        __test_filter_entry_t* ent = (__test_filter_entry_t*)
                        malloc(sizeof(*ent));
                        __ABORT_IF_NO_MONEY(ent);
                        memset(ent, 0, sizeof(*ent));

                        __test_filter_t* fter = (__test_filter_t*)malloc(sizeof(*ft));
                        __ABORT_IF_NO_MONEY(fter);
                        memset(fter, 0, sizeof(*fter));

                        if(*ft == '-') {
                            fter->flag = 1;
                            ft += 1;
                        }
                        p = strstr(ft, ":");
                        if(p) {
                            memcpy(fter->filter, ft, p - ft);
                            ft = p + 1;
                        }else{
                            strcpy(fter->filter, ft);
                        }
                        ent->filter = fter;

                        __CT_SLIST_INSERT_HEAD(t->filter, ent, entry);

                    }
                }while(p);
            }
        }else{
            t->mode = 4;
        }
    }
}

int __ct_match_filter(const char* name)
{
    __test_t* t = __ct_test();

    int match = 0;

    __test_filter_entry_t* ent = NULL;
    __CT_SLIST_FOREACH(ent, t->filter, entry) {
        __test_filter_t* ft = ent->filter;
        match = __ct_match(name, ft->filter);
        if(ft->flag) {
            match = !match;
        }
        if(match) {
            return 1;
        }
    }

    return 0;
}

int __ct_run()
{
    __test_t* t = __ct_test();
    int filter = 0;
    if(!__CT_SLIST_EMPTY(t->filter)) {
        filter = 1;
    }
    int fix_count = __ct_get_fixture_count();
    int test_count = __ct_get_test_count();
    printf("[==========] Running %d tests from %d test cases\n",
            test_count, fix_count);

    clock_t used = 0;
    clock_t envb = clock();
    if(t->setup_env) {
        printf("[----------] Custom global test environment set-up\n");
        t->setup_env();
    }
    __test_fixture_entry_t* ent = NULL;
    __CT_SLIST_FOREACH(ent, t->run, entry) {
        __test_fixture_t* f = ent->test;
        printf("[----------] %d tests from %s\n", f->size, f->name);
        clock_t fixb = clock();
        if(f->setup_testcase){
            f->setup_testcase();
        }
        __test_case_entry_t* cent = NULL;
        __CT_SLIST_FOREACH(cent, f->caseq, entry) {
            __test_case_t* c = cent->test;
            int run = 1;
            if(filter){
                if(!__ct_match_filter(c->name)) {
                    run = 0;
                }
            }
            if(run) {
                printf("[ RUN      ] %s\n", c->name);
                clock_t testb = clock();
                if(f->setup) {
                    f->setup();
                }

                c->test();

                if(f->teardown){
                    f->teardown();
                }
                clock_t teste = clock();
                used = teste - testb;
                if(__ct_is_test_failed(c->tf, c->tc)) {
                    printf("[   FAILED ] %s (%ld ms)\n", c->name, used);
                }else{
                    printf("[       OK ] %s (%ld ms)\n", c->name, used);
                }
            }
        }
        if(f->teardown_testcase) {
            f->teardown_testcase();
        }
        clock_t fixe = clock();
        used = fixe - fixb;
        printf("[----------] %d tests from %s (%ld ms total)\n\n",
                f->size, f->name, used);
    }
    if(t->teardown_env) {
        printf("[----------] Custom global test environment tear-down\n");
        t->teardown_env();
    }
    clock_t enve = clock();
    used = enve - envb;
    printf("[==========] %d tests from %d test case ran (%ld ms total)\n",
            test_count, fix_count, used);
    if (test_count > 0) {
        int failed_count = t->f_size;
        printf("[  PASSED  ] %d tests\n", test_count - failed_count);
        if (failed_count > 0) {
            printf("[  FAILED  ] %d tests\n", failed_count);
            __test_fixture_entry_t* fent = NULL;
            __CT_SLIST_FOREACH(fent, t->failed, entry) {
                __test_case_entry_t* cent = NULL;
                __CT_SLIST_FOREACH(cent, fent->test->caseq, entry) {
                    printf("[  FAILED  ] %s\n", cent->test->name);
                }
            }

        }

    }
    return 0;
}

int __ct_run_test()
{
    __test_t* t = __ct_test();
    switch(t->mode) {
    case 1:
        __ct_help();
        break;
    case 2:
        __ct_list_tests();
        break;
    case 4:
        printf("Invalid Option! Please reference the following help message\n\n");
        __ct_help();
        break;
    default:
        return __ct_run();
    }
    return 0;
}
int __ct_get_fixture_count()
{
    __test_t* t = __ct_test();
    return t->r_size;
}
int __ct_get_test_count()
{
    int count = 0;
    __test_t* t = __ct_test();
    __test_fixture_entry_t* fent = NULL;
    __CT_SLIST_FOREACH(fent, t->run, entry) {
        count += fent->test->size;
    }
    return count;
}
#endif

//////////////////////////////////////////////////////////////////////
// MACROS
#define TEST(tf, tc)                                                 \
    void __test__##tf##__##tc##__test__();                           \
    __attribute__((constructor)) __gen_symbol(tf, tc){               \
        __test_case_t tf##tc = { #tf "." #tc, #tf, #tc,              \
                __test__##tf##__##tc##__test__, 0};                  \
        __ct_register_test(&(tf##tc));                               \
    }                                                                \
    void __test__##tf##__##tc##__test__()                            \

#define SETUP_TEST(tf, tfup, tfdown, tcup, tcdown)                   \
    __attribute__((constructor)) __gen_symbol(                       \
            tf##tfup__##tfdown__##tcup__##tcdown, __test__){         \
        __test_fixture_t tf##tfup__##tfdown__##tcup__##tcdown =      \
        { #tf, tfup, tfdown, tcup, tcdown, 0};                       \
        __ct_register_fixture(&(tf##tfup__##tfdown__##tcup__##tcdown));\
    }

#define SET_ENVIRONMENT(up, down)                                    \
    __attribute__((constructor)) __gen_symbol(                       \
            __test__env__##up##__##down##__test_env__, env){         \
            __ct_set_env(up, down);                                  \
    }

#define INIT_TEST(a, b) __ct_init_test(a, b)
#define RUN_ALL_TEST() __ct_run_test()

#define __FOMATTER(t, a) \
    const char* t = NULL; \
    t = strchr(#a, '.');\
    if(t==NULL) { \
        if(sizeof(a) == sizeof(long)) { \
            t = "%ld"; \
        }else{\
            t = "%d";\
        }\
    }else{\
        if(sizeof(a) == sizeof(double)) {\
            t = "%lf";\
        }else{\
            t = "%f";\
        }\
    }\

#define EXPECT_EQ(a, b) do{                                          \
    if((a) != (b)) {                                                 \
        __FOMATTER(__f1, (a));                                       \
        __FOMATTER(__f2, (b));                                       \
        char fmt[1024] = {0};                                        \
        snprintf(fmt, sizeof(fmt) - 1,                               \
                "[W]File: %s Line: %d EXPECT: %s ACTUAL: %s\n",      \
                __FILE__, __LINE__, __f1, __f2);                     \
        printf(fmt, (a), (b));                                       \
        __ct_add_failed_test(__func__);                              \
    }                                                                \
}while(0)

#define EXPECT_NEQ(a, b) do{                                         \
    if((a) == (b)){                                                  \
        __FOMATTER(__f1, (a));                                       \
        __FOMATTER(__f2, (b));                                       \
        char fmt[1024] = {0};                                        \
        snprintf(fmt, sizeof(fmt) - 1,                               \
                "[W]File: %s Line: %d EXPECT: %s != %s ACTUAL: %s == %s\n",\
                __FILE__, __LINE__, __f1, __f2, __f1, __f2);         \
        printf(fmt, (a), (b), (a), (b));                             \
        __ct_add_failed_test(__func__);                              \
    }                                                                \
}while(0)

#define EXPECT_STREQ(a, b) do{                                       \
    if(strncmp((a), (b), strlen((a))) != 0) {                        \
        printf("[W]File: %s Line: %d EXPECT: %s ACTUAL: %s\n",       \
                __FILE__, __LINE__, (a), (b));                       \
        __ct_add_failed_test(__func__);                              \
    }                                                                \
}while(0)

#define EXPECT_STRNEQ(a, b) do{                                      \
    if(strncmp((a), (b), strlen(a)) == 0) {                          \
        printf("[W]File: %s Line: %d EXPECT: %s != %s ACTUAL: %s == %s\n",\
                __FILE__, __LINE__, (a), (b), (a), (b));             \
            __ct_add_failed_test(__func__);                          \
    }                                                                \
}while(0)

#define EXPECT_STRCASEEQ(a, b) do{                                   \
    if(strncasecmp((a), (b), strlen((a))) != 0) {                    \
        printf("[W]File: %s Line: %d EXPECT: %s ACTUAL: %s\n",       \
                __FILE__, __LINE__, (a), (b));                       \
        __ct_add_failed_test(__func__);                              \
    }                                                                \
}while(0)

#define EXPECT_STRCASENEQ(a, b) do{                                  \
    if(strncasecmp((a), (b), strlen(a)) == 0) {                      \
        printf("[W]File: %s Line: %d EXPECT: %s != %s ACTUAL: %s == %s\n",\
                __FILE__, __LINE__, (a), (b), (a), (b));             \
            __ct_add_failed_test(__func__);                          \
    }                                                                \
}while(0)

#define EXPECT_BINEQ(a, b, s) do{                                    \
    if(memcmp((a), (b), (s)) != 0) {                                 \
        char hex[(s)*2 + 2 + 1] = {0}; /*new feature*/               \
        printf("[W]File: %s Line: %d EXPECT: %s == %s ACTUAL: %s != %s\n",\
                __FILE__, __LINE__,                                  \
                __ct_hex_dump((a), (s), hex), __ct_hex_dump((b), (s), hex),\
                __ct_hex_dump((a), (s), hex), __ct_hex_dump((b), (s), hex));\
        __ct_add_failed_test(__func__);                              \
    }                                                                \
}while(0)

#define EXPECT_BINNEQ(a, b, s) do{                                   \
    if(memcmp((a), (b), (s)) == 0) {                                 \
        char hex[(s)*2 + 2 + 1] = {0}; /*new feature*/               \
        printf("[W]File: %s Line: %d EXPECT: %s != %s ACTUAL: %s == %s\n",\
                __FILE__, __LINE__,                                  \
                __ct_hex_dump((a), (s), hex), __ct_hex_dump((b), (s), hex),\
                __ct_hex_dump((a), (s), hex), __ct_hex_dump((b), (s), hex));\
        __ct_add_failed_test(__func__);                              \
    }                                                                \
}while(0)

#define EXPECT_TRUE(a) do{                                           \
    if(!((int)(a))){                                                 \
        printf("[W]File: %s Line: %d EXPECT: TRUE ACTUAL: FALSE\n",  \
                __FILE__, __LINE__);                                 \
        __ct_add_failed_test(__func__);                              \
    }                                                                \
}while(0)


#define EXPECT_FALSE(a) do{                                          \
    if(!!((int)a)){                                                  \
        printf("[W]File: %s Line: %d EXPECT: FALSE ACTUAL: TRUE\n",  \
                __FILE__, __LINE__);                                 \
        __ct_add_failed_test(__func__);                              \
    }                                                                \
}while(0)


#define ASSERT_EQ(a, b) do{                                          \
    EXPECT_EQ(a, b);                                                 \
    return;                                                          \
}while(0)


#define ASSERT_NEQ(a, b) do{                                         \
    EXPECT_NEQ(a, b);                                                \
    return;                                                          \
}while(0)

#define ASSERT_STREQ(a, b) do{                                       \
    EXPECT_STREQ(a, b);                                              \
    return;                                                          \
}while(0)

#define ASSERT_STRNEQ(a, b) do{                                      \
    EXPECT_STRNEQ(a, b);                                             \
    return;                                                          \
}while(0)

#define ASSERT_STRCASEEQ(a, b) do{                                   \
    EXPECT_STREQ(a, b);                                              \
    return;                                                          \
}while(0)

#define ASSERT_STRCASENEQ(a, b) do{                                  \
    EXPECT_STRNEQ(a, b);                                             \
    return;                                                          \
}while(0)


#define ASSERT_BINEQ(a, b, s) do{                                    \
    EXPECT_BINEQ(a, b, s);                                           \
    return;                                                          \
}while(0)

#define ASSERT_BINNEQ(a, b, s) do{                                   \
    EXPECT_BINNEQ(a, b, s);                                          \
    return;                                                          \
}while(0)


#define ASSERT_TRUE(a) do{                                           \
    EXPECT_TRUE(a);                                                  \
    return;                                                          \
}while(0)

#define ASSERT_FALSE(a) do{                                          \
    EXPECT_FALSE(a);                                                 \
    return;                                                          \
}while(0)


#endif /* __CT_H__ */
