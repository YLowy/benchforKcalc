#include "../MathEx/expression.h"
#include "../MathEx/expression.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h> /* for pow */

int status = 0;
int fp;

static float user_func_next(struct expr_func *f, vec_expr_t args, void *c)
{
    (void)f, (void)c;
    float a = expr_eval(&vec_nth(&args, 0));
    return a + 1;
}

static struct expr_func user_funcs[] = {
    { "next", user_func_next, NULL, 0 },
    { NULL, NULL, NULL, 0 },
};

static void test_benchmark(const char *s)
{
    struct timeval t;
    float result ;
    struct expr_var_list vars = { 0 };
    struct expr *e = expr_create(s, strlen(s), &vars, user_funcs);
    if (e == NULL) {
        printf("FAIL: %s can't be compiled\n", s);
        status = 1;
        return;
    }
    long N = 1000000L;
    for (long i = 0; i < N; i++) {
        result = expr_eval(e);
    }
    expr_destroy(e, &vars);


    //---
    ssize_t value = write(fp,s,strlen(s));
    printf("value = %ld\n",value);
    //---



    printf("BENCH MathEx: %s = %.5f \n", s, result);
}

int main()
{
    fp = open("/dev/calc",02);
    if(fp < 0){ 
        printf("fp =%d\n",fp);
        exit(1);
    }
    printf("---Start Bench---\n");
    
    test_benchmark("5.6*5.8");

    printf("---End Bench---\n");
    close(fp);
    return 0;
}
