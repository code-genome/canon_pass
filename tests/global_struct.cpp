#include <iostream>

// Define the inner struct
struct InnerStruct {
    int inner_value;
};

// Define the outer struct using the inner struct
struct OuterStruct {
    struct InnerStruct inner;
};

// Declare global variable B using the outer struct
struct OuterStruct globalB = {
    .inner = {
        .inner_value = 10
    }
};

// Declare global variable A that is dependent on global variable B
struct OuterStruct *globalA = &globalB;

__attribute__((noinline))
int main() {
    printf("Global Variable A: %p\n", globalA);
    return 0;
}

__attribute__((noinline))
int main3(struct OuterStruct *ptr = nullptr) {
    printf("Global Variable A: %p\n", globalA);
    return 0;
}

__attribute__((noinline))
int main2() {
    main3();
    return 0;
}





