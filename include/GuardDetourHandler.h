#ifndef GUARD_DETOUR_HANDLER_H
#define GUARD_DETOUR_HANDLER_H
namespace GuardDetour
{
#ifdef GUARD_DETOUR_EXCEPTION_MODE
class MinHookException
{
public:
	GuardDetourStatus status;
	MinHookException(GuardDetourStatus status) : status(status) {};
};
#endif
template<typename OriginalFunctionType, typename HookFunctionType>
class GuardDetourBase
{
protected:
	OriginalFunctionType originalFunction;
	OriginalFunctionType original;
	HookFunctionType hookFunction;
#ifndef USE_MINHOOK
    funchook_t* _hook_data;
#endif
public:
	using typeOriginal = OriginalFunctionType;
	using typeHook = HookFunctionType;
	GuardDetourBase() {}
	void hook()
	{
#ifdef USE_MINHOOK
		GuardDetourStatus status = MH_EnableHook(reinterpret_cast<LPVOID>(original));
#else
        GuardDetourStatus status = funchook_install(_hook_data, 0);
#endif
#ifdef GUARD_DETOUR_EXCEPTION_MODE
		if (status != MH_OK)
			throw MinHookException(status);
#endif
	}
	void disable()
	{
#ifdef USE_MINHOOK
		GuardDetourStatus status = MH_DisableHook(reinterpret_cast<LPVOID>(original));
#else
        GuardDetourStatus status = funchook_uninstall(_hook_data, 0);
#endif
#ifdef GUARD_DETOUR_EXCEPTION_MODE
		if (status != MH_OK)
			throw MinHookException(status);
#endif
	}
	void* _original()
	{
		return reinterpret_cast<void*>(original);
	}
	~GuardDetourBase()
	{
		disable();
#ifdef USE_MINHOOK
		GuardDetourStatus status = MH_RemoveHook(reinterpret_cast<LPVOID>(original));
#else
        GuardDetourStatus status = funchook_destroy(_hook_data);
#endif
	}
};
template<typename Fn>
struct GuardDetourHandle;
#if defined(X32) || defined(__WIN32)
template<typename R, typename... Args>
class GuardDetourHandle<R (*)(Args...)> : public GuardDetourBase< R(__cdecl *)(Args...) , R(__cdecl*)(Args...)>
{
public:
	using typeOriginal = typename GuardDetourBase<R(__cdecl*)(Args...), R(__cdecl*)(Args...)>::typeOriginal;
	using typeHook = typename GuardDetourBase<R(__cdecl*)(Args...), R(__cdecl *)(Args...)>::typeHook;
	inline R call_original(Args&&... args)
	{
		return this->originalFunction(args...);
	}
};
template<typename R, typename... Args>
class GuardDetourHandle<R(__stdcall *)(Args...)> : public GuardDetourBase< R(__stdcall *)(Args...), R(__stdcall*)(Args...)>
{
public:
	using typeOriginal = typename GuardDetourBase<R(__stdcall*)(Args...), R(__stdcall*)(Args...)>::typeOriginal;
	using typeHook = typename GuardDetourBase<R(__stdcall*)(Args...), R(__stdcall*)(Args...)>::typeHook;
	inline R call_original(Args&&... args)
	{
		return this->originalFunction(args...);
	}
};
template<typename R, typename... Args>
class GuardDetourHandle<R(__thiscall *)(Args...)> : public GuardDetourBase< R(__thiscall*)(Args...), R(__thiscall*)(Args...)>
{
public:
	using typeOriginal = typename GuardDetourBase<R(__thiscall*)(Args...), R(__thiscall*)(Args...)>::typeOriginal;
	using typeHook = typename GuardDetourBase<R(__thiscall*)(Args...), R(__thiscall*)(Args...)>::typeHook;
	inline R call_original(Args&&... args)
	{
		return this->originalFunction(args...);
	}
};
#else
template<typename R, typename... Args>
class GuardDetourHandle<R (*)(Args...)> : public GuardDetourBase< R(*)(Args...) , R(*)(Args...)>
{
public:
	using typeOriginal = typename GuardDetourBase<R(*)(Args...), R(*)(Args...)>::typeOriginal;
	using typeHook = typename GuardDetourBase<R(*)(Args...), R(*)(Args...)>::typeHook;
	inline R call_original(Args&&... args)
	{
		return this->originalFunction(args...);
	}
};
#endif
}
#endif
