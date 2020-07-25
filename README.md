# GuardDetour
C++11 header-only wrapper for MinHook and funchook libraries  
Support **Windows**(MinHook/FuncHook) and **Linux**(FuncHook) arch **x86** and **x86_64**  
Support *exception mode*
# Example
Simple usage:
```cpp
#include <iostream>
#include "GuardDetour.h"
int orig_func()
{
    std::cout << "ORIG FUNCTON" << std::endl;
    return 0;
}
GuardDetour::GuardDetour<decltype(&orig_func)>* detour;
int hook_func()
{
    std::cout << "HOOKED FUNCTION" << std::endl;
    detour->call_original();
    return 1;
}
int main()
{
    detour = new GuardDetour::GuardDetour<decltype(&orig_func)>(&orig_func, &hook_func);
    detour->hook();
    orig_func();
    delete detour;
}

```
Use return value and change parameters:
```cpp
#include <iostream>
#include "GuardDetour.h"
#include "jni.h"
int orig_func(std::string str)
{
    std::cout << "str: " << str << std::endl;
    return 0;
}
GuardDetour::GuardDetour<decltype(&orig_func)>* detour;
int orig_hook(std::string str)
{
    int ret = detour->call_original(str + "_hooked");
    std::cout << "Orig funcion return " << ret << std::endl;
    return 1;
}
int main()
{
    detour = new GuardDetour::GuardDetour<decltype(&orig_func)>(&orig_func, &orig_hook);
    // Direct function call, non-hooked
    int ret =  orig_func("directcall");
    std::cout << "Function return " << ret << std::endl;
    detour->hook();
    // Direct function call, hooked
    ret =  orig_func("directcall");
    std::cout << "Function return " << ret << std::endl;
    // Original function call, hooked
    ret =  detour->call_original("originalcall");
    std::cout << "Function return " << ret << std::endl;
    delete detour;
}
```
Output:
```
str: directcall
Function return 0
str: directcall_hooked
Orig funcion return 0
Function return 1
str: originalcall
Function return 0
```
# Methods
Constructor `GuardDetour(typeOriginal original, typeHook hook)`  
original - original function  
hook - four hook  
Method `void hook()`  
Enable hook  
Method `void disable()`  
Disable hook  
Method `void* _original()`  
Return raw ptr to original function
# Defined
Define *USE_MINHOOK* to switch to MinHook library(default FuncHook)  
Define *GUARD_EXCEPTION_MODE* to throw exception in error
