#ifndef GUARD_DETOUR_H
#define GUARD_DETOUR_H
#include <function>
#ifdef USE_MINHOOK
#include "MinHook.h"
#define GuardDetourStatus MH_STATUS
#else
#include <funchook.h>
#define GuardDetourStatus int
#endif
#include "GuardDetourHandler.h"
namespace GuardDetour
{
template<typename Fn>
class GuardDetour : public GuardDetourHandle<Fn>
{
public:
	using typeOriginal = typename GuardDetourHandle<Fn>::typeOriginal;
	using typeHook = typename GuardDetourHandle<Fn>::typeHook;
private:
public:
	GuardDetour(typeOriginal original, typeHook hook)
		: GuardDetourHandle<Fn>()
	{
#ifdef USE_MINHOOK
#ifndef GUARD_DETOUR_NO_INITIALIZE_MINHOOK
		static bool isMHInitilizated = false;
		if (!isMHInitilizated)
		{
			MH_Initialize();
			isMHInitilizated = true;
		}
#endif
#endif
#ifdef USE_MINHOOK
		GuardDetourStatus status = MH_CreateHook(reinterpret_cast<LPVOID>(original), reinterpret_cast<LPVOID>(hook), reinterpret_cast<LPVOID*>(&this->originalFunction));
#else
        this->_hook_data = funchook_create();
        this->originalFunction = original;
        GuardDetourStatus status = funchook_prepare(this->_hook_data, reinterpret_cast<void**>(&this->originalFunction), reinterpret_cast<void*>(hook));
#endif
#ifdef GUARD_DETOUR_EXCEPTION_MODE
		if (status != MH_OK)
			throw MinHookException(status);
#endif
	}
	void* _original()
	{
		return reinterpret_cast<void*>(this->original);
	}
};
}
#undef GuardDetourStatus
#endif
