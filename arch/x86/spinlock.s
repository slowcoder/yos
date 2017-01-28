.text

/* int arch_tns(int *val, int set_to, int test_val) */
.global arch_tns
arch_tns:
        movl    4(%esp),%edx
        movl    8(%esp),%ecx
        movl    12(%esp),%eax

        lock
        cmpxchgl        %ecx,(%edx)

        ret
