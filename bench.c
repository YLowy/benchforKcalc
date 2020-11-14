#include "../MathEx/expression.h"
#include "../MathEx/expression.c"
#include "../kcalc-fixed/fixed-point.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h> /* for pow */

int status = 0;
int fp;
static double eval (fixedp ipt){
    if (ipt.data == NAN_INT)
        puts("NAN_INT");
    else if (ipt.data == INF_INT)
        puts("INF_INT");
    else {
        double evalresult = (int) ipt.inte + ((double) ipt.frac / UINT32_MAX);
    return evalresult;
    }
}
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
    fixedp kresult = {0};
    kresult.data = write(fp,s,strlen(s));
    float kout = eval(kresult);
    //---
    if(kout == result)
        printf("BENCH [PASS] %s  MathEx: %.5f kcalc-fixed: %.5f \n ", s, result, kout);
    else 
        printf("BENCH [X] %s  MathEx: %.5f kcalc-fixed: %.5f \n ", s, result, kout);

}

int main()
{
    fp = open("/dev/calc",02);
    if(fp < 0){ 
        exit(1);
    }
    printf("---Start Bench---\n");
    
    test_benchmark("10+1.1");
    test_benchmark("10*1.1");
    test_benchmark("10/1.1");
    test_benchmark("10-1.1");
    test_benchmark("1.1*1.1");

    printf("---End Bench---\n");
    close(fp);
    return 0;
}
