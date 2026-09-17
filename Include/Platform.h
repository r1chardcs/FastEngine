//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_PLATFORM_H
#define FASTENGINE_PLATFORM_H

#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <thread>
#include <exception>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <list>

template <typename T>
class _view_ptr {
    T* data;
public:
    _view_ptr(T* raw);

    operator T*() const;
    T* operator ->() const;
};

template<typename T>
_view_ptr<T>::_view_ptr(T *raw) {
    data = raw;
}

template<typename T>
_view_ptr<T>::operator T*() const {
    return data;
}

template<typename T>
T * _view_ptr<T>::operator->() const {
    return data;
}

template <typename T>
struct _err_object {
    T res;
    const char* err = nullptr;
};

struct _abort_exception : std::exception {
    const char * what() const noexcept override;
};

inline const char * _abort_exception::what() const noexcept { return "Called 'abort'"; }

struct _not_null {};

typedef void* POINTER;
typedef void* HANDLE;
typedef int INT, IHANDLE;
typedef unsigned int UINT, UIHANDLE;
typedef short SHORT, STATUS;
typedef char CHAR;
typedef float FLOAT;
typedef long LONG;
#if !defined(__OBJC__) && !defined(__OBJC_BOOL) && !defined(__objc_INCLUDE_GNU) && !defined(_NO_BOOL_TYPEDEF)
typedef int BOOL;
#endif
typedef double DOUBLE;
typedef const char* LITERAL;

typedef std::string STRING;
typedef std::thread THREAD;
typedef std::mutex MUTEX;
typedef _abort_exception ABORT;
typedef std::exception EXCEPTION;
typedef unsigned char BYTE;
typedef _not_null NOT;

template <typename T>
using VECTOR = std::vector<T>;

template <typename T>
using QUEUE = std::queue<T>;

template <typename T>
using Err = _err_object<T>;

template <typename SIGNATURE>
using FUNC = std::function<SIGNATURE>;

#ifdef CALLBACK
#undef CALLBACK
#endif

template <typename SIGNATURE>
using CALLBACK = std::function<SIGNATURE>;

template <typename T>
using LIST = std::list<T>;

template <typename TKey, typename TValue>
using HASH_MAP = std::unordered_map<TKey, TValue>;

template <typename T>
using NIL = std::optional<T>;

template <typename T>
using SELF_PTR = std::unique_ptr<T>;
#define MakeSelfPtr std::make_unique

template <typename T>
using VIEW_PTR = _view_ptr<T>;

template <typename T>
using GLOBAL_PTR = std::shared_ptr<T>;
#define MakeGlobalPtr std::make_shared

template <typename T>
using WEAK_PTR = std::weak_ptr<T>;

#ifdef nil
    #undef nil
#endif

#define nil std::nullopt

#ifdef VOID
    #undef VOID
#endif
#ifdef INLINE
    #undef INLINE
#endif

#define INLINE inline
#define VIRTUAL virtual
#define VOID void
#define MUTEX_LOCK std::lock_guard
#define MOVE std::move
#define MOVE_PLEASE
#define CALL_ABORT throw ABORT()
#define CREATEISCLASSID public: constexpr static int GetClassID() {return __COUNTER__ + __LINE__; }
using ClassID = INT;

INLINE ClassID _NextClassID() {
    static ClassID counter = 0;
    return counter++;
}

template <typename T>
ClassID StaticClassIDOf() {
    static const ClassID id = _NextClassID();
    return id;
}

#define CREATE_ABSTRACT_CLASS_ID() \
public: \
    VIRTUAL ~ThisClass() = default; \
    VIRTUAL ClassID GetClassID() const = 0

#define CREATE_CLASS_ID(ClassName) \
public: \
    static ClassID StaticClassID() { return StaticClassIDOf<ClassName>(); } \
    ClassID GetClassID() const override { return StaticClassID(); }

#define DEPRECTED_API
#define COMMENT(x)

#endif //FASTENGINE_PLATFORM_H
