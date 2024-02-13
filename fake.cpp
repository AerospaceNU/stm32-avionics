#define PARENS ()

#define EXPAND(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                         \
  macro(a1)                                                     \
  __VA_OPT__(, FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define ⁣ ,
#define ​(b) fun(b);
#define ‌(b) return fun(b);
#define MAKE_AUTO(arg) auto arg
#define fun(a, ...) a(__VA_ARGS__)
#define define(x, value) auto x = value;
#define vals(a) a

#define GET_ARGS(...) __VA_ARGS__
#define λ(args, body) [](FOR_EACH(MAKE_AUTO, GET_ARGS args)) {return body;};

int main() {
    ​(define ⁣ real_main ⁣ ​(λ ⁣ (a ⁣ b ⁣ c) ⁣ (a + b)))
    
    real_main(123.45, (int)3, 2);
    
    ‌(real_main ⁣ 3 ⁣ 4 ⁣ 5)
}
