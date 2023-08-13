// xstring internal header

// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// (<string_view> without emitting non-C++17 warnings)

#pragma once
#ifndef _XSTRING_
#define _XSTRING_
#include <yvals_core.h>
#if _STL_COMPILER_PREPROCESSOR
#include <iosfwd>
#include <xmemory>

#if _HAS_CXX17
#include <xpolymorphic_allocator.h>
#endif // _HAS_CXX17

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

#ifdef __clang__
#define _HAS_MEMCPY_MEMMOVE_INTRINSICS 1
#else // ^^^ use __builtin_memcpy and __builtin_memmove ^^^ / vvv use workaround vvv
#define _HAS_MEMCPY_MEMMOVE_INTRINSICS 0 // TRANSITION, DevCom-1046483 (MSVC) and VSO-1129974 (EDG)
#endif // ^^^ use workaround ^^^

_STD_BEGIN
// STRUCT TEMPLATE _Char_traits (FROM <string>)
template <class _Elem, class _Int_type>
struct _Char_traits { // properties of a string or stream element
    using char_type = _Elem;
    using int_type = _Int_type;
    using pos_type = streampos;
    using off_type = streamoff;
    using state_type = _Mbstatet;
#if _HAS_CXX20
    using comparison_category = strong_ordering;
#endif // _HAS_CXX20

    // For copy/move, we can uniformly call memcpy/memmove (or their builtin versions) for all element types.

    static _CONSTEXPR20 _Elem* copy(_Out_writes_all_(_Count) _Elem* const _First1,
        _In_reads_(_Count) const _Elem* const _First2, const size_t _Count) noexcept /* strengthened */ {
        // copy [_First2, _First2 + _Count) to [_First1, ...)
#if _HAS_MEMCPY_MEMMOVE_INTRINSICS
        __builtin_memcpy(_First1, _First2, _Count * sizeof(_Elem));
#else // ^^^ _HAS_MEMCPY_MEMMOVE_INTRINSICS ^^^ / vvv !_HAS_MEMCPY_MEMMOVE_INTRINSICS vvv
#ifdef __cpp_lib_is_constant_evaluated
        if (_STD is_constant_evaluated()) {
            // pre: [_First1, _First1 + _Count) and [_First2, _First2 + _Count) do not overlap; see LWG-3085
            for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
                _First1[_Idx] = _First2[_Idx];
            }

            return _First1;
        }
#endif // __cpp_lib_is_constant_evaluated

        _CSTD memcpy(_First1, _First2, _Count * sizeof(_Elem));
#endif // ^^^ !_HAS_MEMCPY_MEMMOVE_INTRINSICS ^^^

        return _First1;
    }

    _Pre_satisfies_(_Dest_size >= _Count) static _CONSTEXPR20 _Elem* _Copy_s(_Out_writes_all_(_Dest_size)
        _Elem* const _First1,
        const size_t _Dest_size, _In_reads_(_Count) const _Elem* const _First2, const size_t _Count) noexcept {
        // copy [_First2, _First2 + _Count) to [_First1, _First1 + _Dest_size)
        _STL_VERIFY(_Count <= _Dest_size, "invalid argument");
        return copy(_First1, _First2, _Count);
    }

    static _CONSTEXPR20 _Elem* move(_Out_writes_all_(_Count) _Elem* const _First1,
        _In_reads_(_Count) const _Elem* const _First2, const size_t _Count) noexcept /* strengthened */ {
        // copy [_First2, _First2 + _Count) to [_First1, ...), allowing overlap
#if _HAS_MEMCPY_MEMMOVE_INTRINSICS
        __builtin_memmove(_First1, _First2, _Count * sizeof(_Elem));
#else // ^^^ _HAS_MEMCPY_MEMMOVE_INTRINSICS ^^^ / vvv !_HAS_MEMCPY_MEMMOVE_INTRINSICS vvv
#ifdef __cpp_lib_is_constant_evaluated
        if (_STD is_constant_evaluated()) {
            // dest: [_First1, _First1 + _Count)
            // src: [_First2, _First2 + _Count)
            // We need to handle overlapping ranges.
            // If _First1 is in the src range, we need a backward loop.
            // Otherwise, the forward loop works (even if the back of dest overlaps the front of src).

            if (_First1 == _First2) {
                return _First1; // Self-assignment; either loop would work, but returning immediately is faster.
            }

            // Usually, we would compare pointers with less-than, even though they could belong to different arrays.
            // However, we're not allowed to do that during constant evaluation, so we need a linear scan for equality.
            bool _Loop_forward = true;

            for (const _Elem* _Src = _First2; _Src != _First2 + _Count; ++_Src) {
                if (_First1 == _Src) {
                    _Loop_forward = false;
                    break;
                }
            }

            if (_Loop_forward) {
                for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
                    _First1[_Idx] = _First2[_Idx];
                }
            }
            else {
                for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
                    _First1[_Count - 1 - _Idx] = _First2[_Count - 1 - _Idx];
                }
            }

            return _First1;
        }
#endif // __cpp_lib_is_constant_evaluated

        _CSTD memmove(_First1, _First2, _Count * sizeof(_Elem));
#endif // ^^^ !_HAS_MEMCPY_MEMMOVE_INTRINSICS ^^^

        return _First1;
    }

    // For compare/length/find/assign, we can't uniformly call CRT functions (or their builtin versions).
    // 8-bit: memcmp/strlen/memchr/memset; 16-bit: wmemcmp/wcslen/wmemchr/wmemset; 32-bit: nonexistent

    _NODISCARD static _CONSTEXPR17 int compare(_In_reads_(_Count) const _Elem* _First1,
        _In_reads_(_Count) const _Elem* _First2, size_t _Count) noexcept /* strengthened */ {
        // compare [_First1, _First1 + _Count) with [_First2, ...)
        for (; 0 < _Count; --_Count, ++_First1, ++_First2) {
            if (*_First1 != *_First2) {
                return *_First1 < *_First2 ? -1 : +1;
            }
        }

        return 0;
    }

    _NODISCARD static _CONSTEXPR17 size_t length(_In_z_ const _Elem* _First) noexcept /* strengthened */ {
        // find length of null-terminated sequence
        size_t _Count = 0;
        while (*_First != _Elem()) {
            ++_Count;
            ++_First;
        }

        return _Count;
    }

    _NODISCARD static _CONSTEXPR17 const _Elem* find(
        _In_reads_(_Count) const _Elem* _First, size_t _Count, const _Elem& _Ch) noexcept /* strengthened */ {
        // look for _Ch in [_First, _First + _Count)
        for (; 0 < _Count; --_Count, ++_First) {
            if (*_First == _Ch) {
                return _First;
            }
        }

        return nullptr;
    }

    static _CONSTEXPR20 _Elem* assign(
        _Out_writes_all_(_Count) _Elem* const _First, size_t _Count, const _Elem _Ch) noexcept /* strengthened */ {
        // assign _Count * _Ch to [_First, ...)
#ifdef __cpp_lib_constexpr_string
        if (_STD is_constant_evaluated()) {
            for (_Elem* _Next = _First; _Count > 0; --_Count, ++_Next) {
                _STD construct_at(_Next, _Ch);
            }
        }
        else
#endif // __cpp_lib_constexpr_string
        {
            for (_Elem* _Next = _First; _Count > 0; --_Count, ++_Next) {
                *_Next = _Ch;
            }
        }

        return _First;
    }

    static _CONSTEXPR17 void assign(_Elem& _Left, const _Elem& _Right) noexcept {
#ifdef __cpp_lib_constexpr_string
        if (_STD is_constant_evaluated()) {
            _STD construct_at(_STD addressof(_Left), _Right);
        }
        else
#endif // __cpp_lib_constexpr_string
        {
            _Left = _Right;
        }
    }

    _NODISCARD static constexpr bool eq(const _Elem& _Left, const _Elem& _Right) noexcept {
        return _Left == _Right;
    }

    _NODISCARD static constexpr bool lt(const _Elem& _Left, const _Elem& _Right) noexcept {
        return _Left < _Right;
    }

    _NODISCARD static constexpr _Elem to_char_type(const int_type& _Meta) noexcept {
        return static_cast<_Elem>(_Meta);
    }

    _NODISCARD static constexpr int_type to_int_type(const _Elem& _Ch) noexcept {
        return static_cast<int_type>(_Ch);
    }

    _NODISCARD static constexpr bool eq_int_type(const int_type& _Left, const int_type& _Right) noexcept {
        return _Left == _Right;
    }

    _NODISCARD static constexpr int_type not_eof(const int_type& _Meta) noexcept {
        return _Meta != eof() ? _Meta : !eof();
    }

    _NODISCARD static constexpr int_type eof() noexcept {
        return static_cast<int_type>(EOF);
    }
};

// STRUCT TEMPLATE _WChar_traits
template <class _Elem>
struct _WChar_traits : private _Char_traits<_Elem, unsigned short> {
    // char_traits for the char16_t-likes: char16_t, wchar_t, unsigned short
private:
    using _Primary_char_traits = _Char_traits<_Elem, unsigned short>;

public:
    using char_type = _Elem;
    using int_type = unsigned short;
    using pos_type = streampos;
    using off_type = streamoff;
    using state_type = mbstate_t;
#if _HAS_CXX20
    using comparison_category = strong_ordering;
#endif // _HAS_CXX20

    using _Primary_char_traits::_Copy_s;
    using _Primary_char_traits::copy;
    using _Primary_char_traits::move;

    _NODISCARD static _CONSTEXPR17 int compare(_In_reads_(_Count) const _Elem* const _First1,
        _In_reads_(_Count) const _Elem* const _First2, const size_t _Count) noexcept /* strengthened */ {
        // compare [_First1, _First1 + _Count) with [_First2, ...)
#if _HAS_CXX17
        if constexpr (is_same_v<_Elem, wchar_t>) {
            return __builtin_wmemcmp(_First1, _First2, _Count);
        }
        else {
            return _Primary_char_traits::compare(_First1, _First2, _Count);
        }
#else // _HAS_CXX17
        return _CSTD wmemcmp(
            reinterpret_cast<const wchar_t*>(_First1), reinterpret_cast<const wchar_t*>(_First2), _Count);
#endif // _HAS_CXX17
    }

    _NODISCARD static _CONSTEXPR17 size_t length(_In_z_ const _Elem* _First) noexcept /* strengthened */ {
        // find length of null-terminated sequence
#if _HAS_CXX17
        if constexpr (is_same_v<_Elem, wchar_t>) {
            return __builtin_wcslen(_First);
        }
        else {
            return _Primary_char_traits::length(_First);
        }
#else // _HAS_CXX17
        return _CSTD wcslen(reinterpret_cast<const wchar_t*>(_First));
#endif // _HAS_CXX17
    }

    _NODISCARD static _CONSTEXPR17 const _Elem* find(
        _In_reads_(_Count) const _Elem* _First, const size_t _Count, const _Elem& _Ch) noexcept /* strengthened */ {
        // look for _Ch in [_First, _First + _Count)
#if _HAS_CXX17
        if constexpr (is_same_v<_Elem, wchar_t>) {
            return __builtin_wmemchr(_First, _Ch, _Count);
        }
        else {
            return _Primary_char_traits::find(_First, _Count, _Ch);
        }
#else // _HAS_CXX17
        return reinterpret_cast<const _Elem*>(_CSTD wmemchr(reinterpret_cast<const wchar_t*>(_First), _Ch, _Count));
#endif // _HAS_CXX17
    }

    static _CONSTEXPR20 _Elem* assign(
        _Out_writes_all_(_Count) _Elem* const _First, size_t _Count, const _Elem _Ch) noexcept /* strengthened */ {
        // assign _Count * _Ch to [_First, ...)
#ifdef __cpp_lib_is_constant_evaluated
        if (_STD is_constant_evaluated()) {
            return _Primary_char_traits::assign(_First, _Count, _Ch);
        }
#endif // __cpp_lib_is_constant_evaluated

        return reinterpret_cast<_Elem*>(_CSTD wmemset(reinterpret_cast<wchar_t*>(_First), _Ch, _Count));
    }

    static _CONSTEXPR17 void assign(_Elem& _Left, const _Elem& _Right) noexcept {
        _Left = _Right;
    }

    _NODISCARD static constexpr bool eq(const _Elem& _Left, const _Elem& _Right) noexcept {
        return _Left == _Right;
    }

    _NODISCARD static constexpr bool lt(const _Elem& _Left, const _Elem& _Right) noexcept {
        return _Left < _Right;
    }

    _NODISCARD static constexpr _Elem to_char_type(const int_type& _Meta) noexcept {
        return _Meta;
    }

    _NODISCARD static constexpr int_type to_int_type(const _Elem& _Ch) noexcept {
        return _Ch;
    }

    _NODISCARD static constexpr bool eq_int_type(const int_type& _Left, const int_type& _Right) noexcept {
        return _Left == _Right;
    }

    _NODISCARD static constexpr int_type not_eof(const int_type& _Meta) noexcept {
        return _Meta != eof() ? _Meta : static_cast<int_type>(!eof());
    }

    _NODISCARD static constexpr int_type eof() noexcept {
        return WEOF;
    }
};

// STRUCT TEMPLATE char_traits
template <class _Elem>
struct char_traits : _Char_traits<_Elem, long> {}; // properties of a string or stream unknown element

// STRUCT char_traits<char16_t>
template <>
struct char_traits<char16_t> : _WChar_traits<char16_t> {};

using u16streampos = streampos;

// STRUCT char_traits<char32_t>
template <>
struct char_traits<char32_t> : _Char_traits<char32_t, unsigned int> {};

using u32streampos = streampos;

// STRUCT char_traits<wchar_t>
template <>
struct char_traits<wchar_t> : _WChar_traits<wchar_t> {};

#ifdef _NATIVE_WCHAR_T_DEFINED
// STRUCT char_traits<unsigned short>
template <>
struct char_traits<unsigned short> : _WChar_traits<unsigned short> {};
#endif // _NATIVE_WCHAR_T_DEFINED

#if defined(__cpp_char8_t) && !defined(__EDG__) && !defined(__clang__)
#define _HAS_U8_INTRINSICS 1
#else // ^^^ Use intrinsics for char8_t / don't use said intrinsics vvv
#define _HAS_U8_INTRINSICS 0
#endif // Detect u8 intrinsics

// STRUCT TEMPLATE _Narrow_char_traits
template <class _Elem, class _Int_type>
struct _Narrow_char_traits : private _Char_traits<_Elem, _Int_type> {
    // Implement char_traits for narrow character types char and char8_t
private:
    using _Primary_char_traits = _Char_traits<_Elem, _Int_type>;

public:
    using char_type = _Elem;
    using int_type = _Int_type;
    using pos_type = streampos;
    using off_type = streamoff;
    using state_type = mbstate_t;
#if _HAS_CXX20
    using comparison_category = strong_ordering;
#endif // _HAS_CXX20

    using _Primary_char_traits::_Copy_s;
    using _Primary_char_traits::copy;
    using _Primary_char_traits::move;

    _NODISCARD static _CONSTEXPR17 int compare(_In_reads_(_Count) const _Elem* const _First1,
        _In_reads_(_Count) const _Elem* const _First2, const size_t _Count) noexcept /* strengthened */ {
        // compare [_First1, _First1 + _Count) with [_First2, ...)
#if _HAS_CXX17
        return __builtin_memcmp(_First1, _First2, _Count);
#else // _HAS_CXX17
        return _CSTD memcmp(_First1, _First2, _Count);
#endif // _HAS_CXX17
    }

    _NODISCARD static _CONSTEXPR17 size_t length(_In_z_ const _Elem* const _First) noexcept /* strengthened */ {
        // find length of null-terminated string
#if _HAS_CXX17
#ifdef __cpp_char8_t
        if constexpr (is_same_v<_Elem, char8_t>) {
#if _HAS_U8_INTRINSICS
            return __builtin_u8strlen(_First);
#else // ^^^ use u8 intrinsics / no u8 intrinsics vvv
            return _Primary_char_traits::length(_First);
#endif // _HAS_U8_INTRINSICS
        }
        else
#endif // __cpp_char8_t
        {
            return __builtin_strlen(_First);
        }
#else // _HAS_CXX17
        return _CSTD strlen(reinterpret_cast<const char*>(_First));
#endif // _HAS_CXX17
    }

    _NODISCARD static _CONSTEXPR17 const _Elem* find(_In_reads_(_Count) const _Elem* const _First, const size_t _Count,
        const _Elem& _Ch) noexcept /* strengthened */ {
        // look for _Ch in [_First, _First + _Count)
#if _HAS_CXX17
#ifdef __cpp_char8_t
        if constexpr (is_same_v<_Elem, char8_t>) {
#if _HAS_U8_INTRINSICS
            return __builtin_u8memchr(_First, _Ch, _Count);
#else // ^^^ use u8 intrinsics / no u8 intrinsics vvv
            return _Primary_char_traits::find(_First, _Count, _Ch);
#endif // _HAS_U8_INTRINSICS
        }
        else
#endif // __cpp_char8_t
        {
            return __builtin_char_memchr(_First, _Ch, _Count);
        }
#else // _HAS_CXX17
        return static_cast<const _Elem*>(_CSTD memchr(_First, _Ch, _Count));
#endif // _HAS_CXX17
    }

    static _CONSTEXPR20 _Elem* assign(
        _Out_writes_all_(_Count) _Elem* const _First, size_t _Count, const _Elem _Ch) noexcept /* strengthened */ {
        // assign _Count * _Ch to [_First, ...)
#ifdef __cpp_lib_is_constant_evaluated
        if (_STD is_constant_evaluated()) {
            return _Primary_char_traits::assign(_First, _Count, _Ch);
        }
#endif // __cpp_lib_is_constant_evaluated

        return static_cast<_Elem*>(_CSTD memset(_First, _Ch, _Count));
    }

    static _CONSTEXPR17 void assign(_Elem& _Left, const _Elem& _Right) noexcept {
        _Left = _Right;
    }

    _NODISCARD static constexpr bool eq(const _Elem& _Left, const _Elem& _Right) noexcept {
        return _Left == _Right;
    }

    _NODISCARD static constexpr bool lt(const _Elem& _Left, const _Elem& _Right) noexcept {
        return static_cast<unsigned char>(_Left) < static_cast<unsigned char>(_Right);
    }

    _NODISCARD static constexpr _Elem to_char_type(const int_type& _Meta) noexcept {
        return static_cast<_Elem>(_Meta);
    }

    _NODISCARD static constexpr int_type to_int_type(const _Elem& _Ch) noexcept {
        return static_cast<unsigned char>(_Ch);
    }

    _NODISCARD static constexpr bool eq_int_type(const int_type& _Left, const int_type& _Right) noexcept {
        return _Left == _Right;
    }

    _NODISCARD static constexpr int_type not_eof(const int_type& _Meta) noexcept {
        return _Meta != eof() ? _Meta : !eof();
    }

    _NODISCARD static constexpr int_type eof() noexcept {
        return static_cast<int_type>(EOF);
    }
};

#undef _HAS_U8_INTRINSICS
#undef _HAS_MEMCPY_MEMMOVE_INTRINSICS

// STRUCT char_traits<char> (FROM <string>)
template <>
struct char_traits<char> : _Narrow_char_traits<char, int> {}; // properties of a string or stream char element

#ifdef __cpp_char8_t
// STRUCT char_traits<char8_t>
template <>
struct char_traits<char8_t> : _Narrow_char_traits<char8_t, unsigned int> {};

#ifdef __cpp_lib_char8_t
using u8streampos = streampos;
#endif // __cpp_lib_char8_t
#endif // __cpp_char8_t

template <class _Elem, class _Traits, class _SizeT>
basic_ostream<_Elem, _Traits>& _Insert_string(
    basic_ostream<_Elem, _Traits>& _Ostr, const _Elem* const _Data, const _SizeT _Size) {
    // insert a character-type sequence into _Ostr as if through a basic_string copy
    using _Ostr_t = basic_ostream<_Elem, _Traits>;
    typename _Ostr_t::iostate _State = _Ostr_t::goodbit;

    _SizeT _Pad;
    if (_Ostr.width() <= 0 || static_cast<_SizeT>(_Ostr.width()) <= _Size) {
        _Pad = 0;
    }
    else {
        _Pad = static_cast<_SizeT>(_Ostr.width()) - _Size;
    }

    const typename _Ostr_t::sentry _Ok(_Ostr);

    if (!_Ok) {
        _State |= _Ostr_t::badbit;
    }
    else { // state okay, insert characters
        _TRY_IO_BEGIN
            if ((_Ostr.flags() & _Ostr_t::adjustfield) != _Ostr_t::left) {
                for (; 0 < _Pad; --_Pad) { // pad on left
                    if (_Traits::eq_int_type(_Traits::eof(), _Ostr.rdbuf()->sputc(_Ostr.fill()))) {
                        _State |= _Ostr_t::badbit; // insertion failed, quit
                        break;
                    }
                }
            }

        if (_State == _Ostr_t::goodbit
            && _Ostr.rdbuf()->sputn(_Data, static_cast<streamsize>(_Size)) != static_cast<streamsize>(_Size)) {
            _State |= _Ostr_t::badbit;
        }
        else {
            for (; 0 < _Pad; --_Pad) { // pad on right
                if (_Traits::eq_int_type(_Traits::eof(), _Ostr.rdbuf()->sputc(_Ostr.fill()))) {
                    _State |= _Ostr_t::badbit; // insertion failed, quit
                    break;
                }
            }
        }

        _Ostr.width(0);
        _CATCH_IO_(_Ostr_t, _Ostr)
    }

    _Ostr.setstate(_State);
    return _Ostr;
}

// STRUCT TEMPLATE _Char_traits_eq
template <class _Traits>
struct _Char_traits_eq {
    using _Elem = typename _Traits::char_type;

    bool operator()(_Elem _Left, _Elem _Right) const {
        return _Traits::eq(_Left, _Right);
    }
};

// STRUCT TEMPLATE _Char_traits_lt
template <class _Traits>
struct _Char_traits_lt {
    using _Elem = typename _Traits::char_type;

    bool operator()(_Elem _Left, _Elem _Right) const {
        return _Traits::lt(_Left, _Right);
    }
};

// library-provided char_traits::eq behaves like equal_to<_Elem>
// TRANSITION: This should not be activated for user-defined specializations of char_traits
template <class _Elem>
_INLINE_VAR constexpr bool _Pred_is_consistent_with_memcmp<_Elem, _Elem, _Char_traits_eq<char_traits<_Elem>>> = true;

template <class _Traits>
using _Traits_ch_t = typename _Traits::char_type;

template <class _Traits>
using _Traits_ptr_t = const typename _Traits::char_type*;

template <class _Traits>
constexpr bool _Traits_equal(_In_reads_(_Left_size) const _Traits_ptr_t<_Traits> _Left, const size_t _Left_size,
    _In_reads_(_Right_size) const _Traits_ptr_t<_Traits> _Right, const size_t _Right_size) noexcept {
    // compare [_Left, _Left + _Left_size) to [_Right, _Right + _Right_size) for equality using _Traits
    return _Left_size == _Right_size && _Traits::compare(_Left, _Right, _Left_size) == 0;
}

template <class _Traits>
constexpr int _Traits_compare(_In_reads_(_Left_size) const _Traits_ptr_t<_Traits> _Left, const size_t _Left_size,
    _In_reads_(_Right_size) const _Traits_ptr_t<_Traits> _Right, const size_t _Right_size) noexcept {
    // compare [_Left, _Left + _Left_size) to [_Right, _Right + _Right_size) using _Traits
    const int _Ans = _Traits::compare(_Left, _Right, (_STD min)(_Left_size, _Right_size));

    if (_Ans != 0) {
        return _Ans;
    }

    if (_Left_size < _Right_size) {
        return -1;
    }

    if (_Left_size > _Right_size) {
        return 1;
    }

    return 0;
}

template <class _Traits>
constexpr size_t _Traits_find(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack, const size_t _Hay_size,
    const size_t _Start_at, _In_reads_(_Needle_size) const _Traits_ptr_t<_Traits> _Needle,
    const size_t _Needle_size) noexcept {
    // search [_Haystack, _Haystack + _Hay_size) for [_Needle, _Needle + _Needle_size), at/after _Start_at
    if (_Needle_size > _Hay_size || _Start_at > _Hay_size - _Needle_size) {
        // xpos cannot exist, report failure
        // N4659 24.3.2.7.2 [string.find]/1 says:
        // 1. _Start_at <= xpos
        // 2. xpos + _Needle_size <= _Hay_size;
        // therefore:
        // 3. _Needle_size <= _Hay_size (by 2) (checked above)
        // 4. _Start_at + _Needle_size <= _Hay_size (substitute 1 into 2)
        // 5. _Start_at <= _Hay_size - _Needle_size (4, move _Needle_size to other side) (also checked above)
        return static_cast<size_t>(-1);
    }

    if (_Needle_size == 0) { // empty string always matches if xpos is possible
        return _Start_at;
    }

    const auto _Possible_matches_end = _Haystack + (_Hay_size - _Needle_size) + 1;
    for (auto _Match_try = _Haystack + _Start_at;; ++_Match_try) {
        _Match_try = _Traits::find(_Match_try, static_cast<size_t>(_Possible_matches_end - _Match_try), *_Needle);
        if (!_Match_try) { // didn't find first character; report failure
            return static_cast<size_t>(-1);
        }

        if (_Traits::compare(_Match_try, _Needle, _Needle_size) == 0) { // found match
            return static_cast<size_t>(_Match_try - _Haystack);
        }
    }
}

template <class _Traits>
constexpr size_t _Traits_find_ch(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack, const size_t _Hay_size,
    const size_t _Start_at, const _Traits_ch_t<_Traits> _Ch) noexcept {
    // search [_Haystack, _Haystack + _Hay_size) for _Ch, at/after _Start_at
    if (_Start_at < _Hay_size) {
        const auto _Found_at = _Traits::find(_Haystack + _Start_at, _Hay_size - _Start_at, _Ch);
        if (_Found_at) {
            return static_cast<size_t>(_Found_at - _Haystack);
        }
    }

    return static_cast<size_t>(-1); // (npos) no match
}

template <class _Traits>
constexpr size_t _Traits_rfind(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack, const size_t _Hay_size,
    const size_t _Start_at, _In_reads_(_Needle_size) const _Traits_ptr_t<_Traits> _Needle,
    const size_t _Needle_size) noexcept {
    // search [_Haystack, _Haystack + _Hay_size) for [_Needle, _Needle + _Needle_size) beginning before _Start_at
    if (_Needle_size == 0) {
        return (_STD min)(_Start_at, _Hay_size); // empty string always matches
    }

    if (_Needle_size <= _Hay_size) { // room for match, look for it
        for (auto _Match_try = _Haystack + (_STD min)(_Start_at, _Hay_size - _Needle_size);; --_Match_try) {
            if (_Traits::eq(*_Match_try, *_Needle) && _Traits::compare(_Match_try, _Needle, _Needle_size) == 0) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }

            if (_Match_try == _Haystack) {
                break; // at beginning, no more chance for match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Traits>
constexpr size_t _Traits_rfind_ch(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack, const size_t _Hay_size,
    const size_t _Start_at, const _Traits_ch_t<_Traits> _Ch) noexcept {
    // search [_Haystack, _Haystack + _Hay_size) for _Ch before _Start_at
    if (_Hay_size != 0) { // room for match, look for it
        for (auto _Match_try = _Haystack + (_STD min)(_Start_at, _Hay_size - 1);; --_Match_try) {
            if (_Traits::eq(*_Match_try, _Ch)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }

            if (_Match_try == _Haystack) {
                break; // at beginning, no more chance for match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Elem, bool = _Is_character<_Elem>::value>
class _String_bitmap { // _String_bitmap for character types
public:
    constexpr bool _Mark(const _Elem* _First, const _Elem* const _Last) {
        // mark this bitmap such that the characters in [_First, _Last) are intended to match
        // returns whether all inputs can be placed in the bitmap
        for (; _First != _Last; ++_First) {
            _Matches[static_cast<unsigned char>(*_First)] = true;
        }

        return true;
    }

    constexpr bool _Match(const _Elem _Ch) const { // test if _Ch is in the bitmap
        return _Matches[static_cast<unsigned char>(_Ch)];
    }

private:
    bool _Matches[256] = {};
};

template <class _Elem>
class _String_bitmap<_Elem, false> { // _String_bitmap for wchar_t/unsigned short/char16_t/char32_t/etc. types
public:
    static_assert(is_unsigned_v<_Elem>,
        "Standard char_traits is only provided for char, wchar_t, char16_t, and char32_t. See N5687 [char.traits]. "
        "Visual C++ accepts other unsigned integral types as an extension.");

    constexpr bool _Mark(const _Elem* _First, const _Elem* const _Last) {
        // mark this bitmap such that the characters in [_First, _Last) are intended to match
        // returns whether all inputs can be placed in the bitmap
        for (; _First != _Last; ++_First) {
            const auto _Ch = *_First;
            if (_Ch >= 256U) {
                return false;
            }

            _Matches[static_cast<unsigned char>(_Ch)] = true;
        }

        return true;
    }

    constexpr bool _Match(const _Elem _Ch) const { // test if _Ch is in the bitmap
        return _Ch < 256U && _Matches[_Ch];
    }

private:
    bool _Matches[256] = {};
};

template <class _Traits>
constexpr size_t _Traits_find_first_of(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack,
    const size_t _Hay_size, const size_t _Start_at, _In_reads_(_Needle_size) const _Traits_ptr_t<_Traits> _Needle,
    const size_t _Needle_size, false_type) noexcept {
    // in [_Haystack, _Haystack + _Hay_size), look for one of [_Needle, _Needle + _Needle_size), at/after _Start_at
    // general algorithm
    if (_Needle_size != 0 && _Start_at < _Hay_size) { // room for match, look for it
        const auto _End = _Haystack + _Hay_size;
        for (auto _Match_try = _Haystack + _Start_at; _Match_try < _End; ++_Match_try) {
            if (_Traits::find(_Needle, _Needle_size, *_Match_try)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Traits>
constexpr size_t _Traits_find_first_of(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack,
    const size_t _Hay_size, const size_t _Start_at, _In_reads_(_Needle_size) const _Traits_ptr_t<_Traits> _Needle,
    const size_t _Needle_size, true_type) noexcept {
    // in [_Haystack, _Haystack + _Hay_size), look for one of [_Needle, _Needle + _Needle_size), at/after _Start_at
    // special case for std::char_traits
    if (_Needle_size != 0 && _Start_at < _Hay_size) { // room for match, look for it
        _String_bitmap<typename _Traits::char_type> _Matches;
        if (!_Matches._Mark(_Needle, _Needle + _Needle_size)) { // couldn't put one of the characters into the bitmap,
                                                                // fall back to the serial algorithm
            return _Traits_find_first_of<_Traits>(_Haystack, _Hay_size, _Start_at, _Needle, _Needle_size, false_type{});
        }

        const auto _End = _Haystack + _Hay_size;
        for (auto _Match_try = _Haystack + _Start_at; _Match_try < _End; ++_Match_try) {
            if (_Matches._Match(*_Match_try)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Traits>
constexpr size_t _Traits_find_last_of(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack,
    const size_t _Hay_size, const size_t _Start_at, _In_reads_(_Needle_size) const _Traits_ptr_t<_Traits> _Needle,
    const size_t _Needle_size, false_type) noexcept {
    // in [_Haystack, _Haystack + _Hay_size), look for last of [_Needle, _Needle + _Needle_size), before _Start_at
    // general algorithm
    if (_Needle_size != 0 && _Hay_size != 0) { // worth searching, do it
        for (auto _Match_try = _Haystack + (_STD min)(_Start_at, _Hay_size - 1);; --_Match_try) {
            if (_Traits::find(_Needle, _Needle_size, *_Match_try)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }

            if (_Match_try == _Haystack) {
                break; // at beginning, no more chance for match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Traits>
constexpr size_t _Traits_find_last_of(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack,
    const size_t _Hay_size, const size_t _Start_at, _In_reads_(_Needle_size) const _Traits_ptr_t<_Traits> _Needle,
    const size_t _Needle_size, true_type) noexcept {
    // in [_Haystack, _Haystack + _Hay_size), look for last of [_Needle, _Needle + _Needle_size), before _Start_at
    // special case for std::char_traits
    if (_Needle_size != 0 && _Hay_size != 0) { // worth searching, do it
        _String_bitmap<typename _Traits::char_type> _Matches;
        if (!_Matches._Mark(_Needle, _Needle + _Needle_size)) { // couldn't put one of the characters into the bitmap,
                                                                // fall back to the serial algorithm
            return _Traits_find_last_of<_Traits>(_Haystack, _Hay_size, _Start_at, _Needle, _Needle_size, false_type{});
        }

        for (auto _Match_try = _Haystack + (_STD min)(_Start_at, _Hay_size - 1);; --_Match_try) {
            if (_Matches._Match(*_Match_try)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }

            if (_Match_try == _Haystack) {
                break; // at beginning, no more chance for match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Traits>
constexpr size_t _Traits_find_first_not_of(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack,
    const size_t _Hay_size, const size_t _Start_at, _In_reads_(_Needle_size) const _Traits_ptr_t<_Traits> _Needle,
    const size_t _Needle_size, false_type) noexcept {
    // in [_Haystack, _Haystack + _Hay_size), look for none of [_Needle, _Needle + _Needle_size), at/after _Start_at
    // general algorithm
    if (_Start_at < _Hay_size) { // room for match, look for it
        const auto _End = _Haystack + _Hay_size;
        for (auto _Match_try = _Haystack + _Start_at; _Match_try < _End; ++_Match_try) {
            if (!_Traits::find(_Needle, _Needle_size, *_Match_try)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Traits>
constexpr size_t _Traits_find_first_not_of(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack,
    const size_t _Hay_size, const size_t _Start_at, _In_reads_(_Needle_size) const _Traits_ptr_t<_Traits> _Needle,
    const size_t _Needle_size, true_type) noexcept {
    // in [_Haystack, _Haystack + _Hay_size), look for none of [_Needle, _Needle + _Needle_size), at/after _Start_at
    // special case for std::char_traits
    if (_Start_at < _Hay_size) { // room for match, look for it
        _String_bitmap<typename _Traits::char_type> _Matches;
        if (!_Matches._Mark(_Needle, _Needle + _Needle_size)) { // couldn't put one of the characters into the bitmap,
                                                                // fall back to the serial algorithm
            return _Traits_find_first_not_of<_Traits>(
                _Haystack, _Hay_size, _Start_at, _Needle, _Needle_size, false_type{});
        }

        const auto _End = _Haystack + _Hay_size;
        for (auto _Match_try = _Haystack + _Start_at; _Match_try < _End; ++_Match_try) {
            if (!_Matches._Match(*_Match_try)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Traits>
constexpr size_t _Traits_find_not_ch(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack,
    const size_t _Hay_size, const size_t _Start_at, const _Traits_ch_t<_Traits> _Ch) noexcept {
    // search [_Haystack, _Haystack + _Hay_size) for any value other than _Ch, at/after _Start_at
    if (_Start_at < _Hay_size) { // room for match, look for it
        const auto _End = _Haystack + _Hay_size;
        for (auto _Match_try = _Haystack + _Start_at; _Match_try < _End; ++_Match_try) {
            if (!_Traits::eq(*_Match_try, _Ch)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Traits>
constexpr size_t _Traits_find_last_not_of(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack,
    const size_t _Hay_size, const size_t _Start_at, _In_reads_(_Needle_size) const _Traits_ptr_t<_Traits> _Needle,
    const size_t _Needle_size, false_type) noexcept {
    // in [_Haystack, _Haystack + _Hay_size), look for none of [_Needle, _Needle + _Needle_size), before _Start_at
    // general algorithm
    if (_Hay_size != 0) { // worth searching, do it
        for (auto _Match_try = _Haystack + (_STD min)(_Start_at, _Hay_size - 1);; --_Match_try) {
            if (!_Traits::find(_Needle, _Needle_size, *_Match_try)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }

            if (_Match_try == _Haystack) {
                break; // at beginning, no more chance for match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Traits>
constexpr size_t _Traits_find_last_not_of(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack,
    const size_t _Hay_size, const size_t _Start_at, _In_reads_(_Needle_size) const _Traits_ptr_t<_Traits> _Needle,
    const size_t _Needle_size, true_type) noexcept {
    // in [_Haystack, _Haystack + _Hay_size), look for none of [_Needle, _Needle + _Needle_size), before _Start_at
    // special case for std::char_traits
    if (_Hay_size != 0) { // worth searching, do it
        _String_bitmap<typename _Traits::char_type> _Matches;
        if (!_Matches._Mark(_Needle, _Needle + _Needle_size)) { // couldn't put one of the characters into the bitmap,
                                                                // fall back to the serial algorithm
            return _Traits_find_last_not_of<_Traits>(
                _Haystack, _Hay_size, _Start_at, _Needle, _Needle_size, false_type{});
        }

        for (auto _Match_try = _Haystack + (_STD min)(_Start_at, _Hay_size - 1);; --_Match_try) {
            if (!_Matches._Match(*_Match_try)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }

            if (_Match_try == _Haystack) {
                break; // at beginning, no more chance for match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}

template <class _Traits>
constexpr size_t _Traits_rfind_not_ch(_In_reads_(_Hay_size) const _Traits_ptr_t<_Traits> _Haystack,
    const size_t _Hay_size, const size_t _Start_at, const _Traits_ch_t<_Traits> _Ch) noexcept {
    // search [_Haystack, _Haystack + _Hay_size) for any value other than _Ch before _Start_at
    if (_Hay_size != 0) { // room for match, look for it
        for (auto _Match_try = _Haystack + (_STD min)(_Start_at, _Hay_size - 1);; --_Match_try) {
            if (!_Traits::eq(*_Match_try, _Ch)) {
                return static_cast<size_t>(_Match_try - _Haystack); // found a match
            }

            if (_Match_try == _Haystack) {
                break; // at beginning, no more chance for match
            }
        }
    }

    return static_cast<size_t>(-1); // no match
}


#if _HAS_CXX17
// CLASS TEMPLATE _String_view_iterator
template <class _Elem, class _Traits = char_traits<_Elem>>
class basic_string_view;

template <class _Traits>
class _String_view_iterator {
public:
#ifdef __cpp_lib_concepts
    using iterator_concept = contiguous_iterator_tag;
#endif // __cpp_lib_concepts
    using iterator_category = random_access_iterator_tag;
    using value_type = typename _Traits::char_type;
    using difference_type = ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    constexpr _String_view_iterator() noexcept = default;

private:
    friend basic_string_view<value_type, _Traits>;

#if _ITERATOR_DEBUG_LEVEL >= 1
    constexpr _String_view_iterator(const pointer _Data, const size_t _Size, const size_t _Off) noexcept
        : _Mydata(_Data), _Mysize(_Size), _Myoff(_Off) {}
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
    constexpr explicit _String_view_iterator(const pointer _Ptr) noexcept : _Myptr(_Ptr) {}
#endif // _ITERATOR_DEBUG_LEVEL

public:
    _NODISCARD constexpr reference operator*() const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Mydata, "cannot dereference value-initialized string_view iterator");
        _STL_VERIFY(_Myoff < _Mysize, "cannot dereference end string_view iterator");
        return _Mydata[_Myoff];
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        return *_Myptr;
#endif // _ITERATOR_DEBUG_LEVEL
    }

    _NODISCARD constexpr pointer operator->() const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Mydata, "cannot dereference value-initialized string_view iterator");
        _STL_VERIFY(_Myoff < _Mysize, "cannot dereference end string_view iterator");
        return _Mydata + _Myoff;
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        return _Myptr;
#endif // _ITERATOR_DEBUG_LEVEL
    }

    constexpr _String_view_iterator& operator++() noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Mydata, "cannot increment value-initialized string_view iterator");
        _STL_VERIFY(_Myoff < _Mysize, "cannot increment string_view iterator past end");
        ++_Myoff;
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        ++_Myptr;
#endif // _ITERATOR_DEBUG_LEVEL
        return *this;
    }

    constexpr _String_view_iterator operator++(int) noexcept {
        _String_view_iterator _Tmp{ *this };
        ++* this;
        return _Tmp;
    }

    constexpr _String_view_iterator& operator--() noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Mydata, "cannot decrement value-initialized string_view iterator");
        _STL_VERIFY(_Myoff != 0, "cannot decrement string_view iterator before begin");
        --_Myoff;
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        --_Myptr;
#endif // _ITERATOR_DEBUG_LEVEL
        return *this;
    }

    constexpr _String_view_iterator operator--(int) noexcept {
        _String_view_iterator _Tmp{ *this };
        --* this;
        return _Tmp;
    }

    constexpr void _Verify_offset(const difference_type _Off) const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        if (_Off != 0) {
            _STL_VERIFY(_Mydata, "cannot seek value-initialized string_view iterator");
        }

        if (_Off < 0) {
            _STL_VERIFY(
                _Myoff >= size_t{ 0 } - static_cast<size_t>(_Off), "cannot seek string_view iterator before begin");
        }

        if (_Off > 0) {
            _STL_VERIFY(_Mysize - _Myoff >= static_cast<size_t>(_Off), "cannot seek string_view iterator after end");
        }
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        (void)_Off;
#endif // _ITERATOR_DEBUG_LEVEL >= 1
    }

    constexpr _String_view_iterator& operator+=(const difference_type _Off) noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _Verify_offset(_Off);
        _Myoff += static_cast<size_t>(_Off);
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        _Myptr += _Off;
#endif // _ITERATOR_DEBUG_LEVEL

        return *this;
    }

    _NODISCARD constexpr _String_view_iterator operator+(const difference_type _Off) const noexcept {
        _String_view_iterator _Tmp{ *this };
        _Tmp += _Off;
        return _Tmp;
    }

    constexpr _String_view_iterator& operator-=(const difference_type _Off) noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        if (_Off != 0) {
            _STL_VERIFY(_Mydata, "cannot seek value-initialized string_view iterator");
        }

        if (_Off > 0) {
            _STL_VERIFY(_Myoff >= static_cast<size_t>(_Off), "cannot seek string_view iterator before begin");
        }

        if (_Off < 0) {
            _STL_VERIFY(_Mysize - _Myoff >= size_t{ 0 } - static_cast<size_t>(_Off),
                "cannot seek string_view iterator after end");
        }

        _Myoff -= static_cast<size_t>(_Off);
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        _Myptr -= _Off;
#endif // _ITERATOR_DEBUG_LEVEL

        return *this;
    }

    _NODISCARD constexpr _String_view_iterator operator-(const difference_type _Off) const noexcept {
        _String_view_iterator _Tmp{ *this };
        _Tmp -= _Off;
        return _Tmp;
    }

    _NODISCARD constexpr difference_type operator-(const _String_view_iterator& _Right) const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Mydata == _Right._Mydata && _Mysize == _Right._Mysize,
            "cannot subtract incompatible string_view iterators");
        return static_cast<difference_type>(_Myoff - _Right._Myoff);
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        return _Myptr - _Right._Myptr;
#endif // _ITERATOR_DEBUG_LEVEL
    }

    _NODISCARD constexpr reference operator[](const difference_type _Off) const noexcept {
        return *(*this + _Off);
    }

    _NODISCARD constexpr bool operator==(const _String_view_iterator& _Right) const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Mydata == _Right._Mydata && _Mysize == _Right._Mysize,
            "cannot compare incompatible string_view iterators for equality");
        return _Myoff == _Right._Myoff;
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        return _Myptr == _Right._Myptr;
#endif // _ITERATOR_DEBUG_LEVEL
    }

#if _HAS_CXX20
    _NODISCARD constexpr strong_ordering operator<=>(const _String_view_iterator& _Right) const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Mydata == _Right._Mydata && _Mysize == _Right._Mysize,
            "cannot compare incompatible string_view iterators");
        return _Myoff <=> _Right._Myoff;
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        return _Myptr <=> _Right._Myptr;
#endif // _ITERATOR_DEBUG_LEVEL
    }
#else // ^^^ _HAS_CXX20 ^^^ / vvv !_HAS_CXX20 vvv
    _NODISCARD constexpr bool operator!=(const _String_view_iterator& _Right) const noexcept {
        return !(*this == _Right);
    }

    _NODISCARD constexpr bool operator<(const _String_view_iterator& _Right) const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Mydata == _Right._Mydata && _Mysize == _Right._Mysize,
            "cannot compare incompatible string_view iterators");
        return _Myoff < _Right._Myoff;
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        return _Myptr < _Right._Myptr;
#endif // _ITERATOR_DEBUG_LEVEL
    }

    _NODISCARD constexpr bool operator>(const _String_view_iterator& _Right) const noexcept {
        return _Right < *this;
    }

    _NODISCARD constexpr bool operator<=(const _String_view_iterator& _Right) const noexcept {
        return !(_Right < *this);
    }

    _NODISCARD constexpr bool operator>=(const _String_view_iterator& _Right) const noexcept {
        return !(*this < _Right);
    }
#endif // !_HAS_CXX20

#if _ITERATOR_DEBUG_LEVEL >= 1
    friend constexpr void _Verify_range(const _String_view_iterator& _First, const _String_view_iterator& _Last) {
        _STL_VERIFY(_First._Mydata == _Last._Mydata && _First._Mysize == _Last._Mysize,
            "string_view iterators in range are from different views");
        _STL_VERIFY(_First._Myoff <= _Last._Myoff, "string_view iterator range transposed");
    }
#endif // _ITERATOR_DEBUG_LEVEL >= 1

    using _Prevent_inheriting_unwrap = _String_view_iterator;

    _NODISCARD constexpr pointer _Unwrapped() const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        return _Mydata + _Myoff;
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        return _Myptr;
#endif // _ITERATOR_DEBUG_LEVEL
    }

    static constexpr bool _Unwrap_when_unverified = _ITERATOR_DEBUG_LEVEL == 0;

    constexpr void _Seek_to(pointer _It) noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _Myoff = static_cast<size_t>(_It - _Mydata);
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        _Myptr = _It;
#endif // _ITERATOR_DEBUG_LEVEL
    }

private:
#if _ITERATOR_DEBUG_LEVEL >= 1
    pointer _Mydata = nullptr;
    size_t _Mysize = 0;
    size_t _Myoff = 0;
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
    pointer _Myptr = nullptr;
#endif // _ITERATOR_DEBUG_LEVEL
};

template <class _Traits>
_NODISCARD constexpr _String_view_iterator<_Traits> operator+(
    const typename _String_view_iterator<_Traits>::difference_type _Off,
    _String_view_iterator<_Traits> _Right) noexcept {
    _Right += _Off;
    return _Right;
}

#if _HAS_CXX20
template <class _Traits>
struct pointer_traits<_String_view_iterator<_Traits>> {
    using pointer = _String_view_iterator<_Traits>;
    using element_type = const typename pointer::value_type;
    using difference_type = typename pointer::difference_type;

    _NODISCARD static constexpr element_type* to_address(const pointer& _Iter) noexcept {
        return _Iter._Unwrapped();
    }
};
#endif // _HAS_CXX20


// CLASS TEMPLATE basic_string_view
template <class _Elem, class _Traits>
class basic_string_view { // wrapper for any kind of contiguous character buffer
public:
    static_assert(is_same_v<_Elem, typename _Traits::char_type>,
        "Bad char_traits for basic_string_view; "
        "N4659 24.4.2 [string.view.template]/1 \"the type traits::char_type shall name the same type as charT.\"");

    static_assert(!is_array_v<_Elem>&& is_trivial_v<_Elem>&& is_standard_layout_v<_Elem>,
        "The character type of basic_string_view must be a non-array trivial standard-layout type. See N4861 "
        "[strings.general]/1.");

    using traits_type = _Traits;
    using value_type = _Elem;
    using pointer = _Elem*;
    using const_pointer = const _Elem*;
    using reference = _Elem&;
    using const_reference = const _Elem&;
    using const_iterator = _String_view_iterator<_Traits>;
    using iterator = const_iterator;
    using const_reverse_iterator = _STD reverse_iterator<const_iterator>;
    using reverse_iterator = const_reverse_iterator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    static constexpr auto npos{ static_cast<size_type>(-1) };

    constexpr basic_string_view() noexcept : _Mydata(), _Mysize(0) {}

    constexpr basic_string_view(const basic_string_view&) noexcept = default;
    constexpr basic_string_view& operator=(const basic_string_view&) noexcept = default;

    /* implicit */ constexpr basic_string_view(_In_z_ const const_pointer _Ntcts) noexcept // strengthened
        : _Mydata(_Ntcts), _Mysize(_Traits::length(_Ntcts)) {}

    constexpr basic_string_view(
        _In_reads_(_Count) const const_pointer _Cts, const size_type _Count) noexcept // strengthened
        : _Mydata(_Cts), _Mysize(_Count) {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Count == 0 || _Cts, "non-zero size null string_view");
#endif // _CONTAINER_DEBUG_LEVEL > 0
    }

#ifdef __cpp_lib_concepts
    // clang-format off
    template <contiguous_iterator _It, sized_sentinel_for<_It> _Se>
    requires (is_same_v<iter_value_t<_It>, _Elem> && !is_convertible_v<_Se, size_type>)
        constexpr basic_string_view(_It _First, _Se _Last) noexcept(noexcept(_Last - _First)) // strengthened
        : _Mydata(_STD to_address(_First)), _Mysize(static_cast<size_type>(_Last - _First)) {}
    // clang-format on
#endif // __cpp_lib_concepts

    _NODISCARD constexpr const_iterator begin() const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        return const_iterator(_Mydata, _Mysize, 0);
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        return const_iterator(_Mydata);
#endif // _ITERATOR_DEBUG_LEVEL
    }

    _NODISCARD constexpr const_iterator end() const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        return const_iterator(_Mydata, _Mysize, _Mysize);
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        return const_iterator(_Mydata + _Mysize);
#endif // _ITERATOR_DEBUG_LEVEL
    }

    _NODISCARD constexpr const_iterator cbegin() const noexcept {
        return begin();
    }

    _NODISCARD constexpr const_iterator cend() const noexcept {
        return end();
    }

    _NODISCARD constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator{ end() };
    }

    _NODISCARD constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator{ begin() };
    }

    _NODISCARD constexpr const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    _NODISCARD constexpr const_reverse_iterator crend() const noexcept {
        return rend();
    }

    constexpr const_pointer _Unchecked_begin() const noexcept {
        return _Mydata;
    }

    constexpr const_pointer _Unchecked_end() const noexcept {
        return _Mydata + _Mysize;
    }

    _NODISCARD constexpr size_type size() const noexcept {
        return _Mysize;
    }

    _NODISCARD constexpr size_type length() const noexcept {
        return _Mysize;
    }

    _NODISCARD constexpr bool empty() const noexcept {
        return _Mysize == 0;
    }

    _NODISCARD constexpr const_pointer data() const noexcept {
        return _Mydata;
    }

    _NODISCARD constexpr size_type max_size() const noexcept {
        // bound to PTRDIFF_MAX to make end() - begin() well defined (also makes room for npos)
        // bound to static_cast<size_t>(-1) / sizeof(_Elem) by address space limits
        return (_STD min)(static_cast<size_t>(PTRDIFF_MAX), static_cast<size_t>(-1) / sizeof(_Elem));
    }

    _NODISCARD constexpr const_reference operator[](const size_type _Off) const noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Off < _Mysize, "string_view subscript out of range");
#endif // _CONTAINER_DEBUG_LEVEL > 0
        return _Mydata[_Off];
    }

    _NODISCARD constexpr const_reference at(const size_type _Off) const {
        // get the character at _Off or throw if that is out of range
        _Check_offset_exclusive(_Off);
        return _Mydata[_Off];
    }

    _NODISCARD constexpr const_reference front() const noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Mysize != 0, "cannot call front on empty string_view");
#endif // _CONTAINER_DEBUG_LEVEL > 0
        return _Mydata[0];
    }

    _NODISCARD constexpr const_reference back() const noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Mysize != 0, "cannot call back on empty string_view");
#endif // _CONTAINER_DEBUG_LEVEL > 0
        return _Mydata[_Mysize - 1];
    }

    constexpr void remove_prefix(const size_type _Count) noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Mysize >= _Count, "cannot remove prefix longer than total size");
#endif // _CONTAINER_DEBUG_LEVEL > 0
        _Mydata += _Count;
        _Mysize -= _Count;
    }

    constexpr void remove_suffix(const size_type _Count) noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Mysize >= _Count, "cannot remove suffix longer than total size");
#endif // _CONTAINER_DEBUG_LEVEL > 0
        _Mysize -= _Count;
    }

    constexpr void swap(basic_string_view& _Other) noexcept {
        const basic_string_view _Tmp{ _Other }; // note: std::swap is not constexpr before C++20
        _Other = *this;
        *this = _Tmp;
    }

    _CONSTEXPR20 size_type copy(
        _Out_writes_(_Count) _Elem* const _Ptr, size_type _Count, const size_type _Off = 0) const {
        // copy [_Off, _Off + Count) to [_Ptr, _Ptr + _Count)
        _Check_offset(_Off);
        _Count = _Clamp_suffix_size(_Off, _Count);
        _Traits::copy(_Ptr, _Mydata + _Off, _Count);
        return _Count;
    }

    _Pre_satisfies_(_Dest_size >= _Count) _CONSTEXPR20 size_type
        _Copy_s(_Out_writes_all_(_Dest_size) _Elem* const _Dest, const size_type _Dest_size, size_type _Count,
            const size_type _Off = 0) const {
        // copy [_Off, _Off + _Count) to [_Dest, _Dest + _Count)
        _Check_offset(_Off);
        _Count = _Clamp_suffix_size(_Off, _Count);
        _Traits::_Copy_s(_Dest, _Dest_size, _Mydata + _Off, _Count);
        return _Count;
    }

    _NODISCARD constexpr basic_string_view substr(const size_type _Off = 0, size_type _Count = npos) const {
        // return a new basic_string_view moved forward by _Off and trimmed to _Count elements
        _Check_offset(_Off);
        _Count = _Clamp_suffix_size(_Off, _Count);
        return basic_string_view(_Mydata + _Off, _Count);
    }

    constexpr bool _Equal(const basic_string_view _Right) const noexcept {
        return _Traits_equal<_Traits>(_Mydata, _Mysize, _Right._Mydata, _Right._Mysize);
    }

    _NODISCARD constexpr int compare(const basic_string_view _Right) const noexcept {
        return _Traits_compare<_Traits>(_Mydata, _Mysize, _Right._Mydata, _Right._Mysize);
    }

    _NODISCARD constexpr int compare(const size_type _Off, const size_type _Nx, const basic_string_view _Right) const {
        // compare [_Off, _Off + _Nx) with _Right
        return substr(_Off, _Nx).compare(_Right);
    }

    _NODISCARD constexpr int compare(const size_type _Off, const size_type _Nx, const basic_string_view _Right,
        const size_type _Roff, const size_type _Count) const {
        // compare [_Off, _Off + _Nx) with _Right [_Roff, _Roff + _Count)
        return substr(_Off, _Nx).compare(_Right.substr(_Roff, _Count));
    }

    _NODISCARD constexpr int compare(_In_z_ const _Elem* const _Ptr) const { // compare [0, _Mysize) with [_Ptr, <null>)
        return compare(basic_string_view(_Ptr));
    }

    _NODISCARD constexpr int compare(const size_type _Off, const size_type _Nx, _In_z_ const _Elem* const _Ptr) const {
        // compare [_Off, _Off + _Nx) with [_Ptr, <null>)
        return substr(_Off, _Nx).compare(basic_string_view(_Ptr));
    }

    _NODISCARD constexpr int compare(const size_type _Off, const size_type _Nx,
        _In_reads_(_Count) const _Elem* const _Ptr, const size_type _Count) const {
        // compare [_Off, _Off + _Nx) with [_Ptr, _Ptr + _Count)
        return substr(_Off, _Nx).compare(basic_string_view(_Ptr, _Count));
    }

#if _HAS_CXX20
    _NODISCARD constexpr bool starts_with(const basic_string_view _Right) const noexcept {
        const auto _Rightsize = _Right._Mysize;
        if (_Mysize < _Rightsize) {
            return false;
        }
        return _Traits::compare(_Mydata, _Right._Mydata, _Rightsize) == 0;
    }

    _NODISCARD constexpr bool starts_with(const _Elem _Right) const noexcept {
        return !empty() && _Traits::eq(front(), _Right);
    }

    _NODISCARD constexpr bool starts_with(const _Elem* const _Right) const noexcept /* strengthened */ {
        return starts_with(basic_string_view(_Right));
    }

    _NODISCARD constexpr bool ends_with(const basic_string_view _Right) const noexcept {
        const auto _Rightsize = _Right._Mysize;
        if (_Mysize < _Rightsize) {
            return false;
        }
        return _Traits::compare(_Mydata + (_Mysize - _Rightsize), _Right._Mydata, _Rightsize) == 0;
    }

    _NODISCARD constexpr bool ends_with(const _Elem _Right) const noexcept {
        return !empty() && _Traits::eq(back(), _Right);
    }

    _NODISCARD constexpr bool ends_with(const _Elem* const _Right) const noexcept /* strengthened */ {
        return ends_with(basic_string_view(_Right));
    }
#endif // _HAS_CXX20

    _NODISCARD constexpr size_type find(const basic_string_view _Right, const size_type _Off = 0) const noexcept {
        // look for _Right beginning at or after _Off
        return _Traits_find<_Traits>(_Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize);
    }

    _NODISCARD constexpr size_type find(const _Elem _Ch, const size_type _Off = 0) const noexcept {
        // look for _Ch at or after _Off
        return _Traits_find_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type find(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept /* strengthened */ {
        // look for [_Ptr, _Ptr + _Count) beginning at or after _Off
        return _Traits_find<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Count);
    }

    _NODISCARD constexpr size_type find(_In_z_ const _Elem* const _Ptr, const size_type _Off = 0) const noexcept
        /* strengthened */ {
        // look for [_Ptr, <null>) beginning at or after _Off
        return _Traits_find<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr));
    }

    _NODISCARD constexpr size_type rfind(const basic_string_view _Right, const size_type _Off = npos) const noexcept {
        // look for _Right beginning before _Off
        return _Traits_rfind<_Traits>(_Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize);
    }

    _NODISCARD constexpr size_type rfind(const _Elem _Ch, const size_type _Off = npos) const noexcept {
        // look for _Ch before _Off
        return _Traits_rfind_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type rfind(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept /* strengthened */ {
        // look for [_Ptr, _Ptr + _Count) beginning before _Off
        return _Traits_rfind<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Count);
    }

    _NODISCARD constexpr size_type rfind(_In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const noexcept
        /* strengthened */ {
        // look for [_Ptr, <null>) beginning before _Off
        return _Traits_rfind<_Traits>(_Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr));
    }

    _NODISCARD constexpr size_type find_first_of(const basic_string_view _Right,
        const size_type _Off = 0) const noexcept { // look for one of _Right at or after _Off
        return _Traits_find_first_of<_Traits>(
            _Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_first_of(const _Elem _Ch, const size_type _Off = 0) const noexcept {
        // look for _Ch at or after _Off
        return _Traits_find_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type find_first_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept /* strengthened */ {
        // look for one of [_Ptr, _Ptr + _Count) at or after _Off
        return _Traits_find_first_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_first_of(
        _In_z_ const _Elem* const _Ptr, const size_type _Off = 0) const noexcept /* strengthened */ {
        // look for one of [_Ptr, <null>) at or after _Off
        return _Traits_find_first_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_of(const basic_string_view _Right,
        const size_type _Off = npos) const noexcept { // look for one of _Right before _Off
        return _Traits_find_last_of<_Traits>(
            _Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_of(const _Elem _Ch, const size_type _Off = npos) const noexcept {
        // look for _Ch before _Off
        return _Traits_rfind_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type find_last_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept /* strengthened */ {
        // look for one of [_Ptr, _Ptr + _Count) before _Off
        return _Traits_find_last_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_of(
        _In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const noexcept /* strengthened */ {
        // look for one of [_Ptr, <null>) before _Off
        return _Traits_find_last_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_first_not_of(const basic_string_view _Right,
        const size_type _Off = 0) const noexcept { // look for none of _Right at or after _Off
        return _Traits_find_first_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_first_not_of(const _Elem _Ch, const size_type _Off = 0) const noexcept {
        // look for any value other than _Ch at or after _Off
        return _Traits_find_not_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type find_first_not_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept /* strengthened */ {
        // look for none of [_Ptr, _Ptr + _Count) at or after _Off
        return _Traits_find_first_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_first_not_of(
        _In_z_ const _Elem* const _Ptr, const size_type _Off = 0) const noexcept /* strengthened */ {
        // look for none of [_Ptr, <null>) at or after _Off
        return _Traits_find_first_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_not_of(const basic_string_view _Right,
        const size_type _Off = npos) const noexcept { // look for none of _Right before _Off
        return _Traits_find_last_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Right._Mydata, _Right._Mysize, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_not_of(const _Elem _Ch, const size_type _Off = npos) const noexcept {
        // look for any value other than _Ch before _Off
        return _Traits_rfind_not_ch<_Traits>(_Mydata, _Mysize, _Off, _Ch);
    }

    _NODISCARD constexpr size_type find_last_not_of(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept /* strengthened */ {
        // look for none of [_Ptr, _Ptr + _Count) before _Off
        return _Traits_find_last_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr size_type find_last_not_of(
        _In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const noexcept /* strengthened */ {
        // look for none of [_Ptr, <null>) before _Off
        return _Traits_find_last_not_of<_Traits>(
            _Mydata, _Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{});
    }

    _NODISCARD constexpr bool _Starts_with(const basic_string_view _View) const noexcept {
        return _Mysize >= _View._Mysize && _Traits::compare(_Mydata, _View._Mydata, _View._Mysize) == 0;
    }

private:
    constexpr void _Check_offset(const size_type _Off) const { // checks whether _Off is in the bounds of [0, size()]
        if (_Mysize < _Off) {
            _Xran();
        }
    }

    constexpr void _Check_offset_exclusive(const size_type _Off) const {
        // checks whether _Off is in the bounds of [0, size())
        if (_Mysize <= _Off) {
            _Xran();
        }
    }

    constexpr size_type _Clamp_suffix_size(const size_type _Off, const size_type _Size) const noexcept {
        // trims _Size to the longest it can be assuming a string at/after _Off
        return (_STD min)(_Size, _Mysize - _Off);
    }

    [[noreturn]] static void _Xran() {
        _Xout_of_range("invalid string_view position");
    }

    const_pointer _Mydata;
    size_type _Mysize;
};

#ifdef __cpp_lib_concepts
template <contiguous_iterator _It, sized_sentinel_for<_It> _Se>
basic_string_view(_It, _Se)->basic_string_view<iter_value_t<_It>>;

namespace ranges {
    template <class _Elem, class _Traits>
    inline constexpr bool enable_view<basic_string_view<_Elem, _Traits>> = true;
    template <class _Elem, class _Traits>
    inline constexpr bool enable_borrowed_range<basic_string_view<_Elem, _Traits>> = true;
} // namespace ranges
#endif // __cpp_lib_concepts

// FUNCTION TEMPLATES operator== FOR basic_string_view
template <class _Elem, class _Traits>
_NODISCARD constexpr bool operator==(
    const basic_string_view<_Elem, _Traits> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return _Lhs._Equal(_Rhs);
}

#if !_HAS_CXX20
template <class _Elem, class _Traits, int = 1> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator==(
    const _Identity_t<basic_string_view<_Elem, _Traits>> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return _Lhs._Equal(_Rhs);
}
#endif // !_HAS_CXX20

template <class _Elem, class _Traits, int = 2> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator==(
    const basic_string_view<_Elem, _Traits> _Lhs, const _Identity_t<basic_string_view<_Elem, _Traits>> _Rhs) noexcept {
    return _Lhs._Equal(_Rhs);
}

#if !_HAS_CXX20
// FUNCTION TEMPLATES operator!= FOR basic_string_view
template <class _Elem, class _Traits>
_NODISCARD constexpr bool operator!=(
    const basic_string_view<_Elem, _Traits> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return !_Lhs._Equal(_Rhs);
}

template <class _Elem, class _Traits, int = 1> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator!=(
    const _Identity_t<basic_string_view<_Elem, _Traits>> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return !_Lhs._Equal(_Rhs);
}

template <class _Elem, class _Traits, int = 2> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator!=(
    const basic_string_view<_Elem, _Traits> _Lhs, const _Identity_t<basic_string_view<_Elem, _Traits>> _Rhs) noexcept {
    return !_Lhs._Equal(_Rhs);
}


// FUNCTION TEMPLATES operator< FOR basic_string_view
template <class _Elem, class _Traits>
_NODISCARD constexpr bool operator<(
    const basic_string_view<_Elem, _Traits> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) < 0;
}

template <class _Elem, class _Traits, int = 1> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator<(
    const _Identity_t<basic_string_view<_Elem, _Traits>> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) < 0;
}

template <class _Elem, class _Traits, int = 2> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator<(
    const basic_string_view<_Elem, _Traits> _Lhs, const _Identity_t<basic_string_view<_Elem, _Traits>> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) < 0;
}


// FUNCTION TEMPLATES operator> FOR basic_string_view
template <class _Elem, class _Traits>
_NODISCARD constexpr bool operator>(
    const basic_string_view<_Elem, _Traits> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) > 0;
}

template <class _Elem, class _Traits, int = 1> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator>(
    const _Identity_t<basic_string_view<_Elem, _Traits>> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) > 0;
}

template <class _Elem, class _Traits, int = 2> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator>(
    const basic_string_view<_Elem, _Traits> _Lhs, const _Identity_t<basic_string_view<_Elem, _Traits>> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) > 0;
}


// FUNCTION TEMPLATES operator<= FOR basic_string_view
template <class _Elem, class _Traits>
_NODISCARD constexpr bool operator<=(
    const basic_string_view<_Elem, _Traits> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) <= 0;
}

template <class _Elem, class _Traits, int = 1> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator<=(
    const _Identity_t<basic_string_view<_Elem, _Traits>> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) <= 0;
}

template <class _Elem, class _Traits, int = 2> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator<=(
    const basic_string_view<_Elem, _Traits> _Lhs, const _Identity_t<basic_string_view<_Elem, _Traits>> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) <= 0;
}


// FUNCTION TEMPLATES operator>= FOR basic_string_view
template <class _Elem, class _Traits>
_NODISCARD constexpr bool operator>=(
    const basic_string_view<_Elem, _Traits> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) >= 0;
}

template <class _Elem, class _Traits, int = 1> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator>=(
    const _Identity_t<basic_string_view<_Elem, _Traits>> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) >= 0;
}

template <class _Elem, class _Traits, int = 2> // TRANSITION, VSO-409326
_NODISCARD constexpr bool operator>=(
    const basic_string_view<_Elem, _Traits> _Lhs, const _Identity_t<basic_string_view<_Elem, _Traits>> _Rhs) noexcept {
    return _Lhs.compare(_Rhs) >= 0;
}
#endif // !_HAS_CXX20

#if _HAS_CXX20
template <class _Traits, class = void>
struct _Get_comparison_category {
    using type = weak_ordering;
};

template <class _Traits>
struct _Get_comparison_category<_Traits, void_t<typename _Traits::comparison_category>> {
    using type = typename _Traits::comparison_category;

    static_assert(_Is_any_of_v<type, partial_ordering, weak_ordering, strong_ordering>,
        "N4878 [string.view.comparison]/4: Mandates: R denotes a comparison category type.");
};

template <class _Traits>
using _Get_comparison_category_t = typename _Get_comparison_category<_Traits>::type;

template <class _Elem, class _Traits>
_NODISCARD constexpr _Get_comparison_category_t<_Traits> operator<=>(
    const basic_string_view<_Elem, _Traits> _Lhs, const basic_string_view<_Elem, _Traits> _Rhs) noexcept {
    return static_cast<_Get_comparison_category_t<_Traits>>(_Lhs.compare(_Rhs) <=> 0);
}

template <class _Elem, class _Traits, int = 2> // TRANSITION, VSO-409326
_NODISCARD constexpr _Get_comparison_category_t<_Traits> operator<=>(
    const basic_string_view<_Elem, _Traits> _Lhs, const _Identity_t<basic_string_view<_Elem, _Traits>> _Rhs) noexcept {
    return static_cast<_Get_comparison_category_t<_Traits>>(_Lhs.compare(_Rhs) <=> 0);
}
#endif // _HAS_CXX20

// TYPEDEFS FOR basic_string_view
using string_view = basic_string_view<char>;
#ifdef __cpp_lib_char8_t
using u8string_view = basic_string_view<char8_t>;
#endif // __cpp_lib_char8_t
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;
using wstring_view = basic_string_view<wchar_t>;


// STRUCT TEMPLATE SPECIALIZATION hash FOR basic_string_view
template <class _Elem, class _Traits>
struct hash<basic_string_view<_Elem, _Traits>> {
    _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef basic_string_view<_Elem, _Traits> _ARGUMENT_TYPE_NAME;
    _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef size_t _RESULT_TYPE_NAME;

    _NODISCARD size_t operator()(const basic_string_view<_Elem, _Traits> _Keyval) const noexcept {
        return _Hash_array_representation(_Keyval.data(), _Keyval.size());
    }
};

template <class _Elem, class _Traits>
basic_ostream<_Elem, _Traits>& operator<<(
    basic_ostream<_Elem, _Traits>& _Ostr, const basic_string_view<_Elem, _Traits> _Str) {
    return _Insert_string(_Ostr, _Str.data(), _Str.size());
}


// basic_string_view LITERALS
inline namespace literals {
    inline namespace string_view_literals {
        _NODISCARD constexpr string_view operator"" sv(const char* _Str, size_t _Len) noexcept {
            return string_view(_Str, _Len);
        }

        _NODISCARD constexpr wstring_view operator"" sv(const wchar_t* _Str, size_t _Len) noexcept {
            return wstring_view(_Str, _Len);
        }

#ifdef __cpp_char8_t
        _NODISCARD constexpr basic_string_view<char8_t> operator"" sv(const char8_t* _Str, size_t _Len) noexcept {
            return basic_string_view<char8_t>(_Str, _Len);
        }
#endif // __cpp_char8_t

        _NODISCARD constexpr u16string_view operator"" sv(const char16_t* _Str, size_t _Len) noexcept {
            return u16string_view(_Str, _Len);
        }

        _NODISCARD constexpr u32string_view operator"" sv(const char32_t* _Str, size_t _Len) noexcept {
            return u32string_view(_Str, _Len);
        }
    } // namespace string_view_literals
} // namespace literals
#endif // _HAS_CXX17

// CLASS TEMPLATE _String_const_iterator
template <class _Mystr>
class _String_const_iterator : public _Iterator_base {
public:
#ifdef __cpp_lib_concepts
    using iterator_concept = contiguous_iterator_tag;
#endif // __cpp_lib_concepts
    using iterator_category = random_access_iterator_tag;
    using value_type = typename _Mystr::value_type;
    using difference_type = typename _Mystr::difference_type;
    using pointer = typename _Mystr::const_pointer;
    using reference = const value_type&;

    _CONSTEXPR20_CONTAINER _String_const_iterator() noexcept : _Ptr() {}

    _CONSTEXPR20_CONTAINER _String_const_iterator(pointer _Parg, const _Container_base* _Pstring) noexcept
        : _Ptr(_Parg) {
        this->_Adopt(_Pstring);
    }

    // TRANSITION, DevCom-1331017
    _CONSTEXPR20_CONTAINER _String_const_iterator& operator=(const _String_const_iterator&) noexcept = default;

    _NODISCARD _CONSTEXPR20_CONTAINER reference operator*() const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Ptr, "cannot dereference value-initialized string iterator");
        const auto _Mycont = static_cast<const _Mystr*>(this->_Getcont());
        _STL_VERIFY(_Mycont, "cannot dereference string iterator because the iterator was"
            " invalidated (e.g. reallocation occurred, or the string was destroyed)");
        const auto _Contptr = _Mycont->_Myptr();
        const auto _Rawptr = _Unfancy(_Ptr);
        _STL_VERIFY(_Contptr <= _Rawptr && _Rawptr < _Contptr + _Mycont->_Mysize,
            "cannot dereference string iterator because it is out of range (e.g. an end iterator)");
#endif // _ITERATOR_DEBUG_LEVEL >= 1

        _Analysis_assume_(_Ptr);
        return *_Ptr;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(**this);
    }

    _CONSTEXPR20_CONTAINER _String_const_iterator& operator++() noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Ptr, "cannot increment value-initialized string iterator");
        const auto _Mycont = static_cast<const _Mystr*>(this->_Getcont());
        _STL_VERIFY(_Mycont, "cannot increment string iterator because the iterator was"
            " invalidated (e.g. reallocation occurred, or the string was destroyed)");
        _STL_VERIFY(_Unfancy(_Ptr) < _Mycont->_Myptr() + _Mycont->_Mysize, "cannot increment string iterator past end");
#endif // _ITERATOR_DEBUG_LEVEL >= 1

        ++_Ptr;
        return *this;
    }

    _CONSTEXPR20_CONTAINER _String_const_iterator operator++(int) noexcept {
        _String_const_iterator _Tmp = *this;
        ++* this;
        return _Tmp;
    }

    _CONSTEXPR20_CONTAINER _String_const_iterator& operator--() noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Ptr, "cannot decrement value-initialized string iterator");
        const auto _Mycont = static_cast<const _Mystr*>(this->_Getcont());
        _STL_VERIFY(_Mycont, "cannot decrement string iterator because the iterator was"
            " invalidated (e.g. reallocation occurred, or the string was destroyed)");
        _STL_VERIFY(_Mycont->_Myptr() < _Unfancy(_Ptr), "cannot decrement string iterator before begin");
#endif // _ITERATOR_DEBUG_LEVEL >= 1

        --_Ptr;
        return *this;
    }

    _CONSTEXPR20_CONTAINER _String_const_iterator operator--(int) noexcept {
        _String_const_iterator _Tmp = *this;
        --* this;
        return _Tmp;
    }

    _CONSTEXPR20_CONTAINER void _Verify_offset(const difference_type _Off) const noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        if (_Off == 0) {
            return;
        }

        _STL_ASSERT(_Ptr, "cannot seek value-initialized string iterator");
        const auto _Mycont = static_cast<const _Mystr*>(this->_Getcont());
        _STL_ASSERT(_Mycont, "cannot seek string iterator because the iterator was"
            " invalidated (e.g. reallocation occurred, or the string was destroyed)");
        const auto _Contptr = _Mycont->_Myptr();
        const auto _Rawptr = _Unfancy(_Ptr);

        if (_Off < 0) {
            _STL_VERIFY(_Contptr - _Rawptr <= _Off, "cannot seek string iterator before begin");
        }

        if (_Off > 0) {
            using _Size_type = typename _Mystr::size_type;
            const auto _Left = _Mycont->_Mysize - static_cast<_Size_type>(_Rawptr - _Contptr);
            _STL_VERIFY(static_cast<_Size_type>(_Off) <= _Left, "cannot seek string iterator after end");
        }
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        (void)_Off;
#endif // _ITERATOR_DEBUG_LEVEL >= 1
    }

    _CONSTEXPR20_CONTAINER _String_const_iterator& operator+=(const difference_type _Off) noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        _Verify_offset(_Off);
#endif // _ITERATOR_DEBUG_LEVEL >= 1
        _Ptr += _Off;
        return *this;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _String_const_iterator operator+(const difference_type _Off) const noexcept {
        _String_const_iterator _Tmp = *this;
        _Tmp += _Off; // TRANSITION, LLVM-49342
        return _Tmp;
    }

    _CONSTEXPR20_CONTAINER _String_const_iterator& operator-=(const difference_type _Off) noexcept {
        return *this += -_Off;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _String_const_iterator operator-(const difference_type _Off) const noexcept {
        _String_const_iterator _Tmp = *this;
        _Tmp -= _Off; // TRANSITION, LLVM-49342
        return _Tmp;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER difference_type operator-(const _String_const_iterator& _Right) const noexcept {
        _Compat(_Right);
        return _Ptr - _Right._Ptr;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER reference operator[](const difference_type _Off) const noexcept {
        return *(*this + _Off);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER bool operator==(const _String_const_iterator& _Right) const noexcept {
        _Compat(_Right);
        return _Ptr == _Right._Ptr;
    }

#if _HAS_CXX20
    _NODISCARD _CONSTEXPR20_CONTAINER strong_ordering operator<=>(const _String_const_iterator& _Right) const noexcept {
        _Compat(_Right);
        return _Unfancy(_Ptr) <=> _Unfancy(_Right._Ptr);
    }
#else // ^^^ _HAS_CXX20 ^^^ / vvv !_HAS_CXX20 vvv
    _NODISCARD bool operator!=(const _String_const_iterator& _Right) const noexcept {
        return !(*this == _Right);
    }

    _NODISCARD bool operator<(const _String_const_iterator& _Right) const noexcept {
        _Compat(_Right);
        return _Ptr < _Right._Ptr;
    }

    _NODISCARD bool operator>(const _String_const_iterator& _Right) const noexcept {
        return _Right < *this;
    }

    _NODISCARD bool operator<=(const _String_const_iterator& _Right) const noexcept {
        return !(_Right < *this);
    }

    _NODISCARD bool operator>=(const _String_const_iterator& _Right) const noexcept {
        return !(*this < _Right);
    }
#endif // !_HAS_CXX20

    _CONSTEXPR20_CONTAINER void _Compat(const _String_const_iterator& _Right) const noexcept {
        // test for compatible iterator pair
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(this->_Getcont() == _Right._Getcont(), "string iterators incompatible (e.g."
            " point to different string instances)");
#else // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 ^^^ // vvv _ITERATOR_DEBUG_LEVEL == 0 vvv
        (void)_Right;
#endif // _ITERATOR_DEBUG_LEVEL
    }

#if _ITERATOR_DEBUG_LEVEL >= 1
    friend _CONSTEXPR20_CONTAINER void _Verify_range(
        const _String_const_iterator& _First, const _String_const_iterator& _Last) noexcept {
        _STL_VERIFY(_First._Getcont() == _Last._Getcont(), "string iterators in range are from different containers");
        _STL_VERIFY(_First._Ptr <= _Last._Ptr, "string iterator range transposed");
    }
#endif // _ITERATOR_DEBUG_LEVEL >= 1

    using _Prevent_inheriting_unwrap = _String_const_iterator;

    _NODISCARD _CONSTEXPR20_CONTAINER const value_type* _Unwrapped() const noexcept {
        return _Unfancy(_Ptr);
    }

    _CONSTEXPR20_CONTAINER void _Seek_to(const value_type* _It) noexcept {
        _Ptr = _Refancy<pointer>(const_cast<value_type*>(_It));
    }

    pointer _Ptr; // pointer to element in string
};

template <class _Mystr>
_NODISCARD _CONSTEXPR20_CONTAINER _String_const_iterator<_Mystr> operator+(
    typename _String_const_iterator<_Mystr>::difference_type _Off, _String_const_iterator<_Mystr> _Next) noexcept {
    return _Next += _Off;
}

#if _HAS_CXX20
template <class _Mystr>
struct pointer_traits<_String_const_iterator<_Mystr>> {
    using pointer = _String_const_iterator<_Mystr>;
    using element_type = const typename pointer::value_type;
    using difference_type = typename pointer::difference_type;

    _NODISCARD static constexpr element_type* to_address(const pointer _Iter) noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        const auto _Mycont = static_cast<const _Mystr*>(_Iter._Getcont());
        if (!_Mycont) {
            _STL_VERIFY(!_Iter._Ptr, "cannot convert string iterator to pointer because the iterator was invalidated "
                "(e.g. reallocation occurred, or the string was destroyed)");
        }
#endif // _ITERATOR_DEBUG_LEVEL >= 1

        const auto _Rawptr = _STD to_address(_Iter._Ptr);

#if _ITERATOR_DEBUG_LEVEL >= 1
        if (_Mycont) {
            const auto _Contptr = _Mycont->_Myptr();
            _STL_VERIFY(_Contptr <= _Rawptr && _Rawptr <= _Contptr + _Mycont->_Mysize,
                "cannot convert string iterator to pointer because it is out of range");
        }
#endif // _ITERATOR_DEBUG_LEVEL >= 1

        return _Rawptr;
    }
};
#endif // _HAS_CXX20

// CLASS TEMPLATE _String_iterator
template <class _Mystr>
class _String_iterator : public _String_const_iterator<_Mystr> {
public:
    using _Mybase = _String_const_iterator<_Mystr>;

#ifdef __cpp_lib_concepts
    using iterator_concept = contiguous_iterator_tag;
#endif // __cpp_lib_concepts
    using iterator_category = random_access_iterator_tag;
    using value_type = typename _Mystr::value_type;
    using difference_type = typename _Mystr::difference_type;
    using pointer = typename _Mystr::pointer;
    using reference = value_type&;

    using _Mybase::_Mybase;

    // TRANSITION, DevCom-1331017
    _CONSTEXPR20_CONTAINER _String_iterator& operator=(const _String_iterator&) noexcept = default;

    _NODISCARD _CONSTEXPR20_CONTAINER reference operator*() const noexcept {
        return const_cast<reference>(_Mybase::operator*());
    }

    _NODISCARD _CONSTEXPR20_CONTAINER pointer operator->() const noexcept {
        return pointer_traits<pointer>::pointer_to(**this);
    }

    _CONSTEXPR20_CONTAINER _String_iterator& operator++() noexcept {
        _Mybase::operator++();
        return *this;
    }

    _CONSTEXPR20_CONTAINER _String_iterator operator++(int) noexcept {
        _String_iterator _Tmp = *this;
        _Mybase::operator++();
        return _Tmp;
    }

    _CONSTEXPR20_CONTAINER _String_iterator& operator--() noexcept {
        _Mybase::operator--();
        return *this;
    }

    _CONSTEXPR20_CONTAINER _String_iterator operator--(int) noexcept {
        _String_iterator _Tmp = *this;
        _Mybase::operator--();
        return _Tmp;
    }

    _CONSTEXPR20_CONTAINER _String_iterator& operator+=(const difference_type _Off) noexcept {
        _Mybase::operator+=(_Off);
        return *this;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _String_iterator operator+(const difference_type _Off) const noexcept {
        _String_iterator _Tmp = *this;
        _Tmp += _Off; // TRANSITION, LLVM-49342
        return _Tmp;
    }

    _CONSTEXPR20_CONTAINER _String_iterator& operator-=(const difference_type _Off) noexcept {
        _Mybase::operator-=(_Off);
        return *this;
    }

    using _Mybase::operator-;

    _NODISCARD _CONSTEXPR20_CONTAINER _String_iterator operator-(const difference_type _Off) const noexcept {
        _String_iterator _Tmp = *this;
        _Tmp -= _Off; // TRANSITION, LLVM-49342
        return _Tmp;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER reference operator[](const difference_type _Off) const noexcept {
        return const_cast<reference>(_Mybase::operator[](_Off));
    }

    using _Prevent_inheriting_unwrap = _String_iterator;

    _NODISCARD _CONSTEXPR20_CONTAINER value_type* _Unwrapped() const noexcept {
        return const_cast<value_type*>(_Unfancy(this->_Ptr));
    }
};

template <class _Mystr>
_NODISCARD _CONSTEXPR20_CONTAINER _String_iterator<_Mystr> operator+(
    typename _String_iterator<_Mystr>::difference_type _Off, _String_iterator<_Mystr> _Next) noexcept {
    return _Next += _Off;
}

#if _HAS_CXX20
template <class _Mystr>
struct pointer_traits<_String_iterator<_Mystr>> {
    using pointer = _String_iterator<_Mystr>;
    using element_type = typename pointer::value_type;
    using difference_type = typename pointer::difference_type;

    _NODISCARD static constexpr element_type* to_address(const pointer _Iter) noexcept {
#if _ITERATOR_DEBUG_LEVEL >= 1
        const auto _Mycont = static_cast<const _Mystr*>(_Iter._Getcont());
        if (!_Mycont) {
            _STL_VERIFY(!_Iter._Ptr, "cannot convert string iterator to pointer because the iterator was invalidated "
                "(e.g. reallocation occurred, or the string was destroyed)");
        }
#endif // _ITERATOR_DEBUG_LEVEL >= 1

        const auto _Rawptr = _STD to_address(_Iter._Ptr);

#if _ITERATOR_DEBUG_LEVEL >= 1
        if (_Mycont) {
            const auto _Contptr = _Mycont->_Myptr();
            _STL_VERIFY(_Contptr <= _Rawptr && _Rawptr <= _Contptr + _Mycont->_Mysize,
                "cannot convert string iterator to pointer because it is out of range");
        }
#endif // _ITERATOR_DEBUG_LEVEL >= 1

        return const_cast<element_type*>(_Rawptr);
    }
};
#endif // _HAS_CXX20

// basic_string TYPE WRAPPERS
template <class _Value_type, class _Size_type, class _Difference_type, class _Pointer, class _Const_pointer,
    class _Reference, class _Const_reference>
    struct _String_iter_types {
    using value_type = _Value_type;
    using size_type = _Size_type;
    using difference_type = _Difference_type;
    using pointer = _Pointer;
    using const_pointer = _Const_pointer;
};

// CLASS TEMPLATE _String_val
template <class _Val_types>
class _String_val : public _Container_base {
public:
    using value_type = typename _Val_types::value_type;
    using size_type = typename _Val_types::size_type;
    using difference_type = typename _Val_types::difference_type;
    using pointer = typename _Val_types::pointer;
    using const_pointer = typename _Val_types::const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;

    _CONSTEXPR20_CONTAINER _String_val() noexcept : _Bx() {}

    // length of internal buffer, [1, 16]:
    static constexpr size_type _BUF_SIZE = 16 / sizeof(value_type) < 1 ? 1 : 16 / sizeof(value_type);
    // roundup mask for allocated buffers, [0, 15]:
    static constexpr size_type _ALLOC_MASK = sizeof(value_type) <= 1 ? 15
        : sizeof(value_type) <= 2 ? 7
        : sizeof(value_type) <= 4 ? 3
        : sizeof(value_type) <= 8 ? 1
        : 0;

    _CONSTEXPR20_CONTAINER value_type* _Myptr() noexcept {
        value_type* _Result = _Bx._Buf;
        if (_Large_string_engaged()) {
            _Result = _Unfancy(_Bx._Ptr);
        }

        return _Result;
    }

    _CONSTEXPR20_CONTAINER const value_type* _Myptr() const noexcept {
        const value_type* _Result = _Bx._Buf;
        if (_Large_string_engaged()) {
            _Result = _Unfancy(_Bx._Ptr);
        }

        return _Result;
    }

    _CONSTEXPR20_CONTAINER bool _Large_string_engaged() const noexcept {
#ifdef __cpp_lib_constexpr_string
        if (_STD is_constant_evaluated()) {
            return true;
        }
#endif // __cpp_lib_constexpr_string
        return _BUF_SIZE <= _Myres;
    }

    _CONSTEXPR20_CONTAINER void _Check_offset(const size_type _Off) const {
        // checks whether _Off is in the bounds of [0, size()]
        if (_Mysize < _Off) {
            _Xran();
        }
    }

    _CONSTEXPR20_CONTAINER void _Check_offset_exclusive(const size_type _Off) const {
        // checks whether _Off is in the bounds of [0, size())
        if (_Mysize <= _Off) {
            _Xran();
        }
    }

    [[noreturn]] static void _Xran() {
        _Xout_of_range("invalid string position");
    }

    _CONSTEXPR20_CONTAINER size_type _Clamp_suffix_size(const size_type _Off, const size_type _Size) const noexcept {
        // trims _Size to the longest it can be assuming a string at/after _Off
        return (_STD min)(_Size, _Mysize - _Off);
    }

    union _Bxty { // storage for small buffer or pointer to larger one
        _CONSTEXPR20_CONTAINER _Bxty() noexcept : _Ptr() {} // user-provided, for fancy pointers

        _CONSTEXPR20_CONTAINER ~_Bxty() noexcept {} // user-provided, for fancy pointers

        value_type _Buf[_BUF_SIZE];
        pointer _Ptr;
        char _Alias[_BUF_SIZE]; // TRANSITION, ABI: _Alias is preserved for binary compatibility (especially /clr)
    } _Bx;

    size_type _Mysize = 0; // current length of string
    size_type _Myres = 0; // current storage reserved for string
};

// CLASS TEMPLATE basic_string
template <class _Ty>
constexpr size_t _Size_after_ebco_v = is_empty_v<_Ty> ? 0 : sizeof(_Ty); // get _Ty's size after being EBCO'd

struct _String_constructor_concat_tag {
    // tag to select constructors used by basic_string's concatenation operators (operator+)
    explicit _String_constructor_concat_tag() = default;
};

struct _String_constructor_rvalue_allocator_tag {
    // tag to select constructors used by basic_stringbuf's rvalue str()
    explicit _String_constructor_rvalue_allocator_tag() = default;
};

[[noreturn]] inline void _Xlen_string() {
    _Xlength_error("string too long");
}

template <class _Elem, class _Traits = char_traits<_Elem>, class _Alloc = allocator<_Elem>>
class basic_string { // null-terminated transparent array of elements
private:
    friend _Tidy_deallocate_guard<basic_string>;
    friend basic_stringbuf<_Elem, _Traits, _Alloc>;

    using _Alty = _Rebind_alloc_t<_Alloc, _Elem>;
    using _Alty_traits = allocator_traits<_Alty>;

    using _Scary_val = _String_val<conditional_t<_Is_simple_alloc_v<_Alty>, _Simple_types<_Elem>,
        _String_iter_types<_Elem, typename _Alty_traits::size_type, typename _Alty_traits::difference_type,
        typename _Alty_traits::pointer, typename _Alty_traits::const_pointer, _Elem&, const _Elem&>>>;

    static_assert(!_ENFORCE_MATCHING_ALLOCATORS || is_same_v<_Elem, typename _Alloc::value_type>,
        _MISMATCHED_ALLOCATOR_MESSAGE("basic_string<T, Traits, Allocator>", "T"));

    static_assert(is_same_v<_Elem, typename _Traits::char_type>,
        "N4659 24.3.2.1 [string.require]/3 requires that the supplied "
        "char_traits character type match the string's character type.");

    static_assert(!is_array_v<_Elem>&& is_trivial_v<_Elem>&& is_standard_layout_v<_Elem>,
        "The character type of basic_string must be a non-array trivial standard-layout type. See N4861 "
        "[strings.general]/1.");

public:
    using traits_type = _Traits;
    using allocator_type = _Alloc;

    using value_type = _Elem;
    using size_type = typename _Alty_traits::size_type;
    using difference_type = typename _Alty_traits::difference_type;
    using pointer = typename _Alty_traits::pointer;
    using const_pointer = typename _Alty_traits::const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;

    using iterator = _String_iterator<_Scary_val>;
    using const_iterator = _String_const_iterator<_Scary_val>;

    using reverse_iterator = _STD reverse_iterator<iterator>;
    using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

private:
    static constexpr auto _BUF_SIZE = _Scary_val::_BUF_SIZE;
    static constexpr auto _ALLOC_MASK = _Scary_val::_ALLOC_MASK;

    // When doing _String_val operations by memcpy, we are touching:
    //   _String_val::_Bx::_Buf (type is array of _Elem)
    //   _String_val::_Bx::_Ptr (type is pointer)
    //   _String_val::_Mysize   (type is size_type)
    //   _String_val::_Myres    (type is size_type)
    // N4810 21.1 [strings.general]/1 says _Elem must be trivial standard-layout, so memcpy is safe.
    // We need to ask if pointer is safe to memcpy.
    // size_type must be an unsigned integral type so memcpy is safe.
    // We also need to disable memcpy if the user has supplied _Traits, since
    //   they can observe traits::assign and similar.
    static constexpr bool _Can_memcpy_val = _Is_specialization_v<_Traits, char_traits> && is_trivial_v<pointer>;
    // This offset skips over the _Container_base members, if any
    static constexpr size_t _Memcpy_val_offset = _Size_after_ebco_v<_Container_base>;
    static constexpr size_t _Memcpy_val_size = sizeof(_Scary_val) - _Memcpy_val_offset;

    template <class _Iter>
    using _Is_elem_cptr = bool_constant<_Is_any_of_v<_Iter, const _Elem* const, _Elem* const, const _Elem*, _Elem*>>;

#if _HAS_CXX17
    template <class _StringViewIsh>
    using _Is_string_view_ish =
        enable_if_t<conjunction_v<is_convertible<const _StringViewIsh&, basic_string_view<_Elem, _Traits>>,
        negation<is_convertible<const _StringViewIsh&, const _Elem*>>>,
        int>;
#endif // _HAS_CXX17

public:
    _CONSTEXPR20_CONTAINER basic_string(const basic_string& _Right)
        : _Mypair(_One_then_variadic_args_t{}, _Alty_traits::select_on_container_copy_construction(_Right._Getal())) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Construct_lv_contents(_Right);
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER basic_string(const basic_string& _Right, const _Alloc& _Al)
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Construct_lv_contents(_Right);
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER basic_string() noexcept(is_nothrow_default_constructible_v<_Alty>)
        : _Mypair(_Zero_then_variadic_args_t{}) {
        _Mypair._Myval2._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal()));
        _Tidy_init();
    }

    _CONSTEXPR20_CONTAINER explicit basic_string(const _Alloc& _Al) noexcept
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        _Mypair._Myval2._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal()));
        _Tidy_init();
    }

    _CONSTEXPR20_CONTAINER basic_string(const basic_string& _Right, const size_type _Roff, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) { // construct from _Right [_Roff, <end>)
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Right, _Roff, npos);
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER basic_string(
        const basic_string& _Right, const size_type _Roff, const size_type _Count, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) { // construct from _Right [_Roff, _Roff + _Count)
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Right, _Roff, _Count);
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER basic_string(
        _In_reads_(_Count) const _Elem* const _Ptr, _CRT_GUARDOVERFLOW const size_type _Count)
        : _Mypair(_Zero_then_variadic_args_t{}) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Ptr, _Count);
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER basic_string(
        _In_reads_(_Count) const _Elem* const _Ptr, _CRT_GUARDOVERFLOW const size_type _Count, const _Alloc& _Al)
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Ptr, _Count);
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER basic_string(_In_z_ const _Elem* const _Ptr) : _Mypair(_Zero_then_variadic_args_t{}) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Ptr);
        _Proxy._Release();
    }

#if _HAS_CXX17
    template <class _Alloc2 = _Alloc, enable_if_t<_Is_allocator<_Alloc2>::value, int> = 0>
#endif // _HAS_CXX17
    _CONSTEXPR20_CONTAINER basic_string(_In_z_ const _Elem* const _Ptr, const _Alloc& _Al)
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Ptr);
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER basic_string(_CRT_GUARDOVERFLOW const size_type _Count, const _Elem _Ch)
        : _Mypair(_Zero_then_variadic_args_t{}) {
        // construct from _Count * _Ch
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Count, _Ch);
        _Proxy._Release();
    }

#if _HAS_CXX17
    template <class _Alloc2 = _Alloc, enable_if_t<_Is_allocator<_Alloc2>::value, int> = 0>
#endif // _HAS_CXX17
    _CONSTEXPR20_CONTAINER basic_string(_CRT_GUARDOVERFLOW const size_type _Count, const _Elem _Ch, const _Alloc& _Al)
        : _Mypair(_One_then_variadic_args_t{}, _Al) { // construct from _Count * _Ch with allocator
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Count, _Ch);
        _Proxy._Release();
    }

    template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20_CONTAINER basic_string(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        _Adl_verify_range(_First, _Last);
        _Construct(_Get_unwrapped(_First), _Get_unwrapped(_Last), _Iter_cat_t<_Iter>{});
        _Proxy._Release();
    }

    template <class _Iter>
    _CONSTEXPR20_CONTAINER void _Construct(_Iter _First, const _Iter _Last, input_iterator_tag) {
        // initialize from [_First, _Last), input iterators
        _Tidy_deallocate_guard<basic_string> _Guard{ this };
        for (; _First != _Last; ++_First) {
            push_back(*_First);
        }

        _Guard._Target = nullptr;
    }

    template <class _Iter>
    _CONSTEXPR20_CONTAINER void _Construct(const _Iter _First, const _Iter _Last, forward_iterator_tag) {
        // initialize from [_First, _Last), forward iterators
        const size_type _Count = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)));
        reserve(_Count);
        _Construct(_First, _Last, input_iterator_tag{});
    }

    _CONSTEXPR20_CONTAINER void _Construct(_Elem* const _First, _Elem* const _Last, random_access_iterator_tag) {
        // initialize from [_First, _Last), pointers
        if (_First != _Last) {
            assign(_First, _Convert_size<size_type>(static_cast<size_t>(_Last - _First)));
        }
    }

    _CONSTEXPR20_CONTAINER void _Construct(
        const _Elem* const _First, const _Elem* const _Last, random_access_iterator_tag) {
        // initialize from [_First, _Last), const pointers
        if (_First != _Last) {
            assign(_First, _Convert_size<size_type>(static_cast<size_t>(_Last - _First)));
        }
    }

    _CONSTEXPR20_CONTAINER basic_string(basic_string&& _Right) noexcept
        : _Mypair(_One_then_variadic_args_t{}, _STD move(_Right._Getal())) {
        _Mypair._Myval2._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal()));
        _Take_contents(_Right);
    }

    _CONSTEXPR20_CONTAINER basic_string(basic_string&& _Right, const _Alloc& _Al) noexcept(
        _Alty_traits::is_always_equal::value) // strengthened
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        if constexpr (!_Alty_traits::is_always_equal::value) {
            if (_Getal() != _Right._Getal()) {
                _Construct_lv_contents(_Right);
                _Proxy._Release();
                return;
            }
        }

        _Take_contents(_Right);
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER basic_string(_String_constructor_concat_tag, const basic_string& _Source_of_al,
        const _Elem* const _Left_ptr, const size_type _Left_size, const _Elem* const _Right_ptr,
        const size_type _Right_size)
        : _Mypair(
            _One_then_variadic_args_t{}, _Alty_traits::select_on_container_copy_construction(_Source_of_al._Getal())) {
        _STL_INTERNAL_CHECK(_Left_size <= max_size());
        _STL_INTERNAL_CHECK(_Right_size <= max_size());
        _STL_INTERNAL_CHECK(_Right_size <= max_size() - _Left_size);
        const auto _New_size = static_cast<size_type>(_Left_size + _Right_size);
        size_type _New_capacity = _BUF_SIZE - 1;
        auto& _My_data = _Mypair._Myval2;
        _Elem* _Ptr = _My_data._Bx._Buf;
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _My_data); // throws

#ifdef __cpp_lib_constexpr_string
        const bool _Activate_large_mode = _New_capacity < _New_size || _STD is_constant_evaluated();
#else // ^^^ __cpp_lib_constexpr_string / !__cpp_lib_constexpr_string vvv
        const bool _Activate_large_mode = _New_capacity < _New_size;
#endif // __cpp_lib_constexpr_string

        if (_Activate_large_mode) {
            // we should never allocate less than _BUF_SIZE space (_New_size could be small if constant evaluated)
            const size_type _Requested_size = (_STD max)(_New_size, _BUF_SIZE);
            _New_capacity = _Calculate_growth(_Requested_size, _BUF_SIZE - 1, max_size());
            const pointer _Fancyptr = _Getal().allocate(_New_capacity + 1); // throws
            _Ptr = _Unfancy(_Fancyptr);
            _Construct_in_place(_My_data._Bx._Ptr, _Fancyptr);
        }

#ifdef __cpp_lib_constexpr_string
        if (_STD is_constant_evaluated()) { // Begin the lifetimes of the objects before copying to avoid UB
            _Traits::assign(_Ptr, _New_capacity + 1, _Elem());
        }
#endif // __cpp_lib_constexpr_string

        _My_data._Mysize = _New_size;
        _My_data._Myres = _New_capacity;
        _Traits::copy(_Ptr, _Left_ptr, _Left_size);
        _Traits::copy(_Ptr + static_cast<ptrdiff_t>(_Left_size), _Right_ptr, _Right_size);
        _Traits::assign(_Ptr[_New_size], _Elem());
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER basic_string(_String_constructor_concat_tag, basic_string& _Left, basic_string& _Right)
        : _Mypair(_One_then_variadic_args_t{}, _Left._Getal()) {
        auto& _My_data = _Mypair._Myval2;
        auto& _Left_data = _Left._Mypair._Myval2;
        auto& _Right_data = _Right._Mypair._Myval2;
        _Left_data._Orphan_all();
        _Right_data._Orphan_all();
        const auto _Left_size = _Left_data._Mysize;
        const auto _Right_size = _Right_data._Mysize;

        const auto _Left_capacity = _Left_data._Myres;
        const auto _Right_capacity = _Right_data._Myres;
        // overflow is OK due to max_size() checks:
        const auto _New_size = static_cast<size_type>(_Left_size + _Right_size);
        const bool _Fits_in_left = _Right_size <= _Left_capacity - _Left_size;
        if (_Fits_in_left && _Right_capacity <= _Left_capacity) {
            // take _Left's buffer, max_size() is OK because _Fits_in_left
            _My_data._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal())); // throws, hereafter nothrow in this block
            _Take_contents(_Left);
            const auto _Ptr = _My_data._Myptr();
            _Traits::copy(_Ptr + _Left_size, _Right_data._Myptr(), _Right_size + 1);
            _My_data._Mysize = _New_size;
            return;
        }

        const bool _Fits_in_right = _Left_size <= _Right_capacity - _Right_size;
        if (_Allocators_equal(_Getal(), _Right._Getal()) && _Fits_in_right) {
            // take _Right's buffer, max_size() is OK because _Fits_in_right
            // At this point, we have tested:
            // !(_Fits_in_left && _Right_capacity <= _Left_capacity) && _Fits_in_right
            // therefore: (by De Morgan's Laws)
            // (!_Fits_in_left || _Right_capacity > _Left_capacity) && _Fits_in_right
            // therefore: (by the distributive property)
            // (!_Fits_in_left && _Fits_in_right)  // implying _Right has more capacity
            //     || (_Right_capacity > _Left_capacity && _Fits_in_right)  // tests that _Right has more capacity
            // therefore: _Right must have more than the minimum capacity, so it must be _Large_string_engaged()
            _STL_INTERNAL_CHECK(_Right_data._Large_string_engaged());
            _My_data._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal())); // throws, hereafter nothrow in this block
            _Take_contents(_Right);
            const auto _Ptr = _Unfancy(_My_data._Bx._Ptr);
            _Traits::move(_Ptr + _Left_size, _Ptr, _Right_size + 1);
            _Traits::copy(_Ptr, _Left_data._Myptr(), _Left_size);
            _My_data._Mysize = _New_size;
            return;
        }

        // can't use either buffer, reallocate
        const auto _Max = max_size();
        if (_Max - _Left_size < _Right_size) { // check if max_size() is OK
            _Xlen_string();
        }

        const auto _New_capacity = _Calculate_growth(_New_size, _BUF_SIZE - 1, _Max);
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _My_data); // throws
        const pointer _Fancyptr = _Getal().allocate(_New_capacity + 1); // throws
        // nothrow hereafter
#ifdef __cpp_lib_constexpr_string
        if (_STD is_constant_evaluated()) { // Begin the lifetimes of the objects before copying to avoid UB
            _Traits::assign(_Unfancy(_Fancyptr), _New_capacity + 1, _Elem());
        }
#endif // __cpp_lib_constexpr_string
        _Construct_in_place(_My_data._Bx._Ptr, _Fancyptr);
        _My_data._Mysize = _New_size;
        _My_data._Myres = _New_capacity;
        const auto _Ptr = _Unfancy(_Fancyptr);
        _Traits::copy(_Ptr, _Left_data._Myptr(), _Left_size);
        _Traits::copy(_Ptr + _Left_size, _Right_data._Myptr(), _Right_size + 1);
        _Proxy._Release();
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER explicit basic_string(const _StringViewIsh& _Right, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Right);
        _Proxy._Release();
    }

    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string(
        const _StringViewIsh& _Right, const size_type _Roff, const size_type _Count, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) { // construct from _Right [_Roff, _Roff + _Count) using _Al
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Right, _Roff, _Count);
        _Proxy._Release();
    }
#endif // _HAS_CXX17

#if _HAS_CXX20
    basic_string(_String_constructor_rvalue_allocator_tag, _Alloc&& _Al)
        : _Mypair(_One_then_variadic_args_t{}, _STD move(_Al)) {
        // Used exclusively by basic_stringbuf
        _Mypair._Myval2._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal()));
        _Tidy_init();
    }
#endif // _HAS_CXX20

private:
    _CONSTEXPR20_CONTAINER void _Move_assign(basic_string& _Right, _Equal_allocators) noexcept {
        _Tidy_deallocate();
        _Pocma(_Getal(), _Right._Getal());
        _Take_contents(_Right);
    }

    _CONSTEXPR20_CONTAINER void _Move_assign(basic_string& _Right, _Propagate_allocators) noexcept {
        if (_Getal() == _Right._Getal()) {
            _Move_assign(_Right, _Equal_allocators{});
        }
        else {
            // intentionally slams into noexcept on OOM, TRANSITION, VSO-466800
            _Mypair._Myval2._Orphan_all();
            _Mypair._Myval2._Reload_proxy(
                _GET_PROXY_ALLOCATOR(_Alty, _Getal()), _GET_PROXY_ALLOCATOR(_Alty, _Right._Getal()));
            _Pocma(_Getal(), _Right._Getal());
            _Take_contents(_Right);
        }
    }

    _CONSTEXPR20_CONTAINER void _Move_assign(basic_string& _Right, _No_propagate_allocators) {
        if (_Getal() == _Right._Getal()) {
            _Move_assign(_Right, _Equal_allocators{});
        }
        else {
            assign(_Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
        }
    }

public:
#if _HAS_CXX20
    _NODISCARD bool _Move_assign_from_buffer(_Elem* const _Right, const size_type _Size, const size_type _Res) {
        // Move assign from a buffer, used exclusively by basic_stringbuf; returns _Large_string_engaged()
        _Tidy_deallocate();
        pointer _Fancy_right = _Refancy<pointer>(_Right);
        auto& _My_data = _Mypair._Myval2;
        _My_data._Mysize = _Size;
        _My_data._Myres = _Res - 1;
        if (_My_data._Large_string_engaged()) {
            _Construct_in_place(_My_data._Bx._Ptr, _Fancy_right);
            return true;
        }
        else {
            _Traits::copy(_My_data._Bx._Buf, _Right, _Res);
            _My_data._Myres = _BUF_SIZE - 1;
            return false;
        }
    }

    // No instance of this type can exist where an exception may be thrown.
    struct _Released_buffer {
        pointer _Ptr;
        size_type _Size;
        size_type _Res;
    };

    _NODISCARD _Released_buffer _Release_to_buffer(_Alloc& _Al) {
        // Release to a buffer, or allocate a new one if in small string mode; used exclusively by basic_stringbuf
        _Released_buffer _Result;
        auto& _My_data = _Mypair._Myval2;
        _Result._Size = _My_data._Mysize;
        if (_My_data._Large_string_engaged()) {
            _Result._Ptr = _My_data._Bx._Ptr;
            _Result._Res = _My_data._Myres + 1;
        }
        else {
            // use _BUF_SIZE + 1 to avoid SSO, if the buffer is assigned back
            _Result._Ptr = _Al.allocate(_BUF_SIZE + 1);
            _Traits::copy(_Unfancy(_Result._Ptr), _My_data._Bx._Buf, _BUF_SIZE);
            _Result._Res = _BUF_SIZE + 1;
        }
        _My_data._Orphan_all();
        _Tidy_init();
        return _Result;
    }
#endif // _HAS_CXX20

    _CONSTEXPR20_CONTAINER basic_string& operator=(basic_string&& _Right) noexcept(
        noexcept(_Move_assign(_Right, _Choose_pocma<_Alty>{}))) {
        if (this != _STD addressof(_Right)) {
            _Move_assign(_Right, _Choose_pocma<_Alty>{});
        }

        return *this;
    }

    _CONSTEXPR20_CONTAINER basic_string& assign(basic_string&& _Right) noexcept(noexcept(*this = _STD move(_Right))) {
        *this = _STD move(_Right);
        return *this;
    }

private:
    void _Memcpy_val_from(const basic_string& _Right) noexcept {
        _STL_INTERNAL_CHECK(_Can_memcpy_val); // TRANSITION, if constexpr
        const auto _My_data_mem =
            reinterpret_cast<unsigned char*>(_STD addressof(_Mypair._Myval2)) + _Memcpy_val_offset;
        const auto _Right_data_mem =
            reinterpret_cast<const unsigned char*>(_STD addressof(_Right._Mypair._Myval2)) + _Memcpy_val_offset;
        _CSTD memcpy(_My_data_mem, _Right_data_mem, _Memcpy_val_size);
    }

    _CONSTEXPR20_CONTAINER void _Take_contents(basic_string& _Right) noexcept {
        // assign by stealing _Right's buffer
        // pre: this != &_Right
        // pre: allocator propagation (POCMA) from _Right, if necessary, is complete
        // pre: *this owns no memory, iterators orphaned
        // (note: _Buf/_Ptr/_Mysize/_Myres may be garbage init)
        auto& _My_data = _Mypair._Myval2;
        auto& _Right_data = _Right._Mypair._Myval2;

        if constexpr (_Can_memcpy_val) {
#ifdef __cpp_lib_constexpr_string
            if (!_STD is_constant_evaluated())
#endif // __cpp_lib_constexpr_string
            {
#if _ITERATOR_DEBUG_LEVEL != 0
                if (_Right_data._Large_string_engaged()) {
                    // take ownership of _Right's iterators along with its buffer
                    _Swap_proxy_and_iterators(_Right);
                }
                else {
                    _Right_data._Orphan_all();
                }
#endif // _ITERATOR_DEBUG_LEVEL != 0

                _Memcpy_val_from(_Right);
                _Right._Tidy_init();
                return;
            }
        }

        if (_Right_data._Large_string_engaged()) { // steal buffer
            _Construct_in_place(_My_data._Bx._Ptr, _Right_data._Bx._Ptr);
            _Right_data._Bx._Ptr = nullptr;
            _Swap_proxy_and_iterators(_Right);
        }
        else { // copy small string buffer
            _Traits::copy(_My_data._Bx._Buf, _Right_data._Bx._Buf, _Right_data._Mysize + 1);
            _Right_data._Orphan_all();
        }

        _My_data._Mysize = _Right_data._Mysize;
        _My_data._Myres = _Right_data._Myres;
        _Right._Tidy_init();
    }

    _CONSTEXPR20_CONTAINER void _Construct_lv_contents(const basic_string& _Right) {
        // assign by copying data stored in _Right
        // pre: this != &_Right
        // pre: *this owns no memory, iterators orphaned (note:
        // _Buf/_Ptr/_Mysize/_Myres may be garbage init)
        auto& _Right_data = _Right._Mypair._Myval2;
        const size_type _Right_size = _Right_data._Mysize;
        const _Elem* const _Right_ptr = _Right_data._Myptr();
        auto& _My_data = _Mypair._Myval2;

#ifdef __cpp_lib_constexpr_string
        const bool _Stay_small = _Right_size < _BUF_SIZE && !_STD is_constant_evaluated();
#else // ^^^ __cpp_lib_constexpr_string / !__cpp_lib_constexpr_string vvv
        const bool _Stay_small = _Right_size < _BUF_SIZE;
#endif // __cpp_lib_constexpr_string

        // NOTE: even if _Right is in large mode, we only go into large mode ourselves if the actual size of _Right
        // requires it
        if (_Stay_small) { // stay small, don't allocate
            _Traits::copy(_My_data._Bx._Buf, _Right_ptr, _BUF_SIZE);
            _My_data._Mysize = _Right_size;
            _My_data._Myres = _BUF_SIZE - 1;
            return;
        }

        auto& _Al = _Getal();
        const size_type _New_capacity = (_STD min)(_Right_size | _ALLOC_MASK, max_size());
        const pointer _New_array = _Al.allocate(_New_capacity + 1); // throws
        _Construct_in_place(_My_data._Bx._Ptr, _New_array);

#ifdef __cpp_lib_constexpr_string
        if (_STD is_constant_evaluated()) { // Begin the lifetimes of the objects before copying to avoid UB
            _Traits::assign(_Unfancy(_New_array), _New_capacity + 1, _Elem());
        }
#endif // __cpp_lib_constexpr_string
        _Traits::copy(_Unfancy(_New_array), _Right_ptr, _Right_size + 1);
        _My_data._Mysize = _Right_size;
        _My_data._Myres = _New_capacity;
    }

public:
    _CONSTEXPR20_CONTAINER basic_string(initializer_list<_Elem> _Ilist, const _Alloc& _Al = allocator_type())
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
        _Tidy_init();
        assign(_Ilist.begin(), _Convert_size<size_type>(_Ilist.size()));
        _Proxy._Release();
    }

    _CONSTEXPR20_CONTAINER basic_string& operator=(initializer_list<_Elem> _Ilist) {
        return assign(_Ilist.begin(), _Convert_size<size_type>(_Ilist.size()));
    }

    _CONSTEXPR20_CONTAINER basic_string& operator+=(initializer_list<_Elem> _Ilist) {
        return append(_Ilist.begin(), _Convert_size<size_type>(_Ilist.size()));
    }

    _CONSTEXPR20_CONTAINER basic_string& assign(initializer_list<_Elem> _Ilist) {
        return assign(_Ilist.begin(), _Convert_size<size_type>(_Ilist.size()));
    }

    _CONSTEXPR20_CONTAINER basic_string& append(initializer_list<_Elem> _Ilist) {
        return append(_Ilist.begin(), _Convert_size<size_type>(_Ilist.size()));
    }

    _CONSTEXPR20_CONTAINER iterator insert(const const_iterator _Where, const initializer_list<_Elem> _Ilist) {
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_Where._Getcont() == _STD addressof(_Mypair._Myval2), "string iterator incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        const auto _Off = static_cast<size_type>(_Unfancy(_Where._Ptr) - _Mypair._Myval2._Myptr());
        insert(_Off, _Ilist.begin(), _Convert_size<size_type>(_Ilist.size()));
        return begin() + static_cast<difference_type>(_Off);
    }

    _CONSTEXPR20_CONTAINER basic_string& replace(
        const const_iterator _First, const const_iterator _Last, const initializer_list<_Elem> _Ilist) {
        // replace with initializer_list
        _Adl_verify_range(_First, _Last);
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_First._Getcont() == _STD addressof(_Mypair._Myval2), "string iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        const auto _Offset = static_cast<size_type>(_Unfancy(_First._Ptr) - _Mypair._Myval2._Myptr());
        const auto _Length = static_cast<size_type>(_Last._Ptr - _First._Ptr);
        return replace(_Offset, _Length, _Ilist.begin(), _Convert_size<size_type>(_Ilist.size()));
    }

    _CONSTEXPR20_CONTAINER ~basic_string() noexcept {
        _Tidy_deallocate();
#if _ITERATOR_DEBUG_LEVEL != 0
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        const auto _To_delete = _Mypair._Myval2._Myproxy;
        _Mypair._Myval2._Myproxy = nullptr;
        _Delete_plain_internal(_Alproxy, _To_delete);
#endif // _ITERATOR_DEBUG_LEVEL != 0
    }

    static constexpr auto npos{ static_cast<size_type>(-1) };

private:
    void _Copy_assign_val_from_small(const basic_string& _Right) {
        // TRANSITION, VSO-761321; inline into only caller when that's fixed
#ifdef __cpp_lib_constexpr_string
        _STL_ASSERT(!_STD is_constant_evaluated(), "SSO should be disabled in a constexpr context");
#endif // __cpp_lib_constexpr_string
        _Tidy_deallocate();
        if constexpr (_Can_memcpy_val) {
            _Memcpy_val_from(_Right);
        }
        else {
            auto& _My_data = _Mypair._Myval2;
            auto& _Right_data = _Right._Mypair._Myval2;

            _Traits::copy(_My_data._Bx._Buf, _Right_data._Bx._Buf, _Right_data._Mysize + 1);
            _My_data._Mysize = _Right_data._Mysize;
            _My_data._Myres = _Right_data._Myres;
        }
    }

    _CONSTEXPR20_CONTAINER void _Copy_assign(const basic_string& _Right, false_type) {
        _Pocca(_Getal(), _Right._Getal());
        assign(_Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
    }

    _CONSTEXPR20_CONTAINER void _Copy_assign(const basic_string& _Right, true_type) {
        auto& _Al = _Getal();
        const auto& _Right_al = _Right._Getal();
        if (_Al == _Right_al) {
            _Copy_assign(_Right, false_type{});
            return;
        }

        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Al);
        auto&& _Right_alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Right_al);
        _Container_proxy_ptr<_Alty> _New_proxy(_Right_alproxy, _Leave_proxy_unbound{}); // throws

        if (_Right._Mypair._Myval2._Large_string_engaged()) {
            const auto _New_size = _Right._Mypair._Myval2._Mysize;
            const auto _New_capacity = _Calculate_growth(_New_size, 0, _Right.max_size());
            auto _Right_al_non_const = _Right_al;
            const auto _New_ptr = _Right_al_non_const.allocate(_New_capacity); // throws

#ifdef __cpp_lib_constexpr_string
            if (_STD is_constant_evaluated()) { // Begin the lifetimes of the objects before copying to avoid UB
                _Traits::assign(_Unfancy(_New_ptr), _New_size + 1, _Elem());
            }
#endif // __cpp_lib_constexpr_string

            _Traits::copy(_Unfancy(_New_ptr), _Unfancy(_Right._Mypair._Myval2._Bx._Ptr), _New_size + 1);
            _Tidy_deallocate();
            _Mypair._Myval2._Bx._Ptr = _New_ptr;
            _Mypair._Myval2._Mysize = _New_size;
            _Mypair._Myval2._Myres = _New_capacity;
        }
        else {
            _Copy_assign_val_from_small(_Right);
        }

        _Pocca(_Al, _Right_al);
        _New_proxy._Bind(_Alproxy, _STD addressof(_Mypair._Myval2));
    }

public:
    _CONSTEXPR20_CONTAINER basic_string& operator=(const basic_string& _Right) {
        if (this != _STD addressof(_Right)) {
            _Copy_assign(_Right, _Choose_pocca<_Alty>{});
        }

        return *this;
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& operator=(const _StringViewIsh& _Right) {
        return assign(_Right);
    }
#endif // _HAS_CXX17

    _CONSTEXPR20_CONTAINER basic_string& operator=(_In_z_ const _Elem* const _Ptr) {
        return assign(_Ptr);
    }

    _CONSTEXPR20_CONTAINER basic_string& operator=(const _Elem _Ch) { // assign {_Ch, _Elem()}
        _Mypair._Myval2._Mysize = 1;
        _Elem* const _Ptr = _Mypair._Myval2._Myptr();
        _Traits::assign(_Ptr[0], _Ch);
        _Traits::assign(_Ptr[1], _Elem());
        return *this;
    }

    _CONSTEXPR20_CONTAINER basic_string& operator+=(const basic_string& _Right) {
        return append(_Right);
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& operator+=(const _StringViewIsh& _Right) {
        return append(_Right);
    }
#endif // _HAS_CXX17

    _CONSTEXPR20_CONTAINER basic_string& operator+=(_In_z_ const _Elem* const _Ptr) { // append [_Ptr, <null>)
        return append(_Ptr);
    }

    _CONSTEXPR20_CONTAINER basic_string& operator+=(_Elem _Ch) {
        push_back(_Ch);
        return *this;
    }

    _CONSTEXPR20_CONTAINER basic_string& append(const basic_string& _Right) {
        return append(_Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
    }

    _CONSTEXPR20_CONTAINER basic_string& append(
        const basic_string& _Right, const size_type _Roff, size_type _Count = npos) {
        // append _Right [_Roff, _Roff + _Count)
        _Right._Mypair._Myval2._Check_offset(_Roff);
        _Count = _Right._Mypair._Myval2._Clamp_suffix_size(_Roff, _Count);
        return append(_Right._Mypair._Myval2._Myptr() + _Roff, _Count);
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& append(const _StringViewIsh& _Right) {
        const basic_string_view<_Elem, _Traits> _As_view = _Right;
        return append(_As_view.data(), _Convert_size<size_type>(_As_view.size()));
    }

    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& append(
        const _StringViewIsh& _Right, const size_type _Roff, const size_type _Count = npos) {
        // append _Right [_Roff, _Roff + _Count)
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return append(_As_view.substr(_Roff, _Count));
    }
#endif // _HAS_CXX17

    _CONSTEXPR20_CONTAINER basic_string& append(
        _In_reads_(_Count) const _Elem* const _Ptr, _CRT_GUARDOVERFLOW const size_type _Count) {
        // append [_Ptr, _Ptr + _Count)
        const size_type _Old_size = _Mypair._Myval2._Mysize;
        if (_Count <= _Mypair._Myval2._Myres - _Old_size) {
            _Mypair._Myval2._Mysize = _Old_size + _Count;
            _Elem* const _Old_ptr = _Mypair._Myval2._Myptr();
            _Traits::move(_Old_ptr + _Old_size, _Ptr, _Count);
            _Traits::assign(_Old_ptr[_Old_size + _Count], _Elem());
            return *this;
        }

        return _Reallocate_grow_by(
            _Count,
            [](_Elem* const _New_ptr, const _Elem* const _Old_ptr, const size_type _Old_size, const _Elem* const _Ptr,
                const size_type _Count) {
                    _Traits::copy(_New_ptr, _Old_ptr, _Old_size);
                    _Traits::copy(_New_ptr + _Old_size, _Ptr, _Count);
                    _Traits::assign(_New_ptr[_Old_size + _Count], _Elem());
            },
            _Ptr, _Count);
    }

    _CONSTEXPR20_CONTAINER basic_string& append(_In_z_ const _Elem* const _Ptr) { // append [_Ptr, <null>)
        return append(_Ptr, _Convert_size<size_type>(_Traits::length(_Ptr)));
    }

    _CONSTEXPR20_CONTAINER basic_string& append(_CRT_GUARDOVERFLOW const size_type _Count, const _Elem _Ch) {
        // append _Count * _Ch
        const size_type _Old_size = _Mypair._Myval2._Mysize;
        if (_Count <= _Mypair._Myval2._Myres - _Old_size) {
            _Mypair._Myval2._Mysize = _Old_size + _Count;
            _Elem* const _Old_ptr = _Mypair._Myval2._Myptr();
            _Traits::assign(_Old_ptr + _Old_size, _Count, _Ch);
            _Traits::assign(_Old_ptr[_Old_size + _Count], _Elem());
            return *this;
        }

        return _Reallocate_grow_by(
            _Count,
            [](_Elem* const _New_ptr, const _Elem* const _Old_ptr, const size_type _Old_size, const size_type _Count,
                const _Elem _Ch) {
                    _Traits::copy(_New_ptr, _Old_ptr, _Old_size);
                    _Traits::assign(_New_ptr + _Old_size, _Count, _Ch);
                    _Traits::assign(_New_ptr[_Old_size + _Count], _Elem());
            },
            _Count, _Ch);
    }

    template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20_CONTAINER basic_string& append(const _Iter _First, const _Iter _Last) {
        // append [_First, _Last), input iterators
        _Adl_verify_range(_First, _Last);
        const auto _UFirst = _Get_unwrapped(_First);
        const auto _ULast = _Get_unwrapped(_Last);
        if constexpr (_Is_elem_cptr<decltype(_UFirst)>::value) {
            return append(_UFirst, _Convert_size<size_type>(static_cast<size_t>(_ULast - _UFirst)));
        }
        else {
            const basic_string _Right(_UFirst, _ULast, get_allocator());
            return append(_Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
        }
    }

    _CONSTEXPR20_CONTAINER basic_string& assign(const basic_string& _Right) {
        *this = _Right;
        return *this;
    }

    _CONSTEXPR20_CONTAINER basic_string& assign(
        const basic_string& _Right, const size_type _Roff, size_type _Count = npos) {
        // assign _Right [_Roff, _Roff + _Count)
        _Right._Mypair._Myval2._Check_offset(_Roff);
        _Count = _Right._Mypair._Myval2._Clamp_suffix_size(_Roff, _Count);
        return assign(_Right._Mypair._Myval2._Myptr() + _Roff, _Count);
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& assign(const _StringViewIsh& _Right) {
        const basic_string_view<_Elem, _Traits> _As_view = _Right;
        return assign(_As_view.data(), _Convert_size<size_type>(_As_view.size()));
    }

    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& assign(
        const _StringViewIsh& _Right, const size_type _Roff, const size_type _Count = npos) {
        // assign _Right [_Roff, _Roff + _Count)
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return assign(_As_view.substr(_Roff, _Count));
    }
#endif // _HAS_CXX17

    _CONSTEXPR20_CONTAINER basic_string& assign(
        _In_reads_(_Count) const _Elem* const _Ptr, _CRT_GUARDOVERFLOW const size_type _Count) {
        // assign [_Ptr, _Ptr + _Count)
        if (_Count <= _Mypair._Myval2._Myres) {
            _Elem* const _Old_ptr = _Mypair._Myval2._Myptr();
            _Mypair._Myval2._Mysize = _Count;
            _Traits::move(_Old_ptr, _Ptr, _Count);
            _Traits::assign(_Old_ptr[_Count], _Elem());
            return *this;
        }

        return _Reallocate_for(
            _Count,
            [](_Elem* const _New_ptr, const size_type _Count, const _Elem* const _Ptr) {
                _Traits::copy(_New_ptr, _Ptr, _Count);
                _Traits::assign(_New_ptr[_Count], _Elem());
            },
            _Ptr);
    }

    _CONSTEXPR20_CONTAINER basic_string& assign(_In_z_ const _Elem* const _Ptr) {
        return assign(_Ptr, _Convert_size<size_type>(_Traits::length(_Ptr)));
    }

    _CONSTEXPR20_CONTAINER basic_string& assign(_CRT_GUARDOVERFLOW const size_type _Count, const _Elem _Ch) {
        // assign _Count * _Ch
        if (_Count <= _Mypair._Myval2._Myres) {
            _Elem* const _Old_ptr = _Mypair._Myval2._Myptr();
            _Mypair._Myval2._Mysize = _Count;
            _Traits::assign(_Old_ptr, _Count, _Ch);
            _Traits::assign(_Old_ptr[_Count], _Elem());
            return *this;
        }

        return _Reallocate_for(
            _Count,
            [](_Elem* const _New_ptr, const size_type _Count, const _Elem _Ch) {
                _Traits::assign(_New_ptr, _Count, _Ch);
                _Traits::assign(_New_ptr[_Count], _Elem());
            },
            _Ch);
    }

    template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20_CONTAINER basic_string& assign(const _Iter _First, const _Iter _Last) {
        _Adl_verify_range(_First, _Last);
        const auto _UFirst = _Get_unwrapped(_First);
        const auto _ULast = _Get_unwrapped(_Last);
        if constexpr (_Is_elem_cptr<decltype(_UFirst)>::value) {
            return assign(_UFirst, _Convert_size<size_type>(static_cast<size_t>(_ULast - _UFirst)));
        }
        else {
            basic_string _Right(_UFirst, _ULast, get_allocator());
            if (_Mypair._Myval2._Myres < _Right._Mypair._Myval2._Myres) {
                _Mypair._Myval2._Orphan_all();
                _Swap_data(_Right);
                return *this;
            }
            else {
                return assign(_Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
            }
        }
    }

    _CONSTEXPR20_CONTAINER basic_string& insert(const size_type _Off, const basic_string& _Right) {
        // insert _Right at _Off
        return insert(_Off, _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
    }

    _CONSTEXPR20_CONTAINER basic_string& insert(
        const size_type _Off, const basic_string& _Right, const size_type _Roff, size_type _Count = npos) {
        // insert _Right [_Roff, _Roff + _Count) at _Off
        _Right._Mypair._Myval2._Check_offset(_Roff);
        _Count = _Right._Mypair._Myval2._Clamp_suffix_size(_Roff, _Count);
        return insert(_Off, _Right._Mypair._Myval2._Myptr() + _Roff, _Count);
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& insert(const size_type _Off, const _StringViewIsh& _Right) {
        // insert _Right at _Off
        const basic_string_view<_Elem, _Traits> _As_view = _Right;
        return insert(_Off, _As_view.data(), _Convert_size<size_type>(_As_view.size()));
    }

    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& insert(
        const size_type _Off, const _StringViewIsh& _Right, const size_type _Roff, const size_type _Count = npos) {
        // insert _Right [_Roff, _Roff + _Count) at _Off
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return insert(_Off, _As_view.substr(_Roff, _Count));
    }
#endif // _HAS_CXX17

    _CONSTEXPR20_CONTAINER basic_string& insert(
        const size_type _Off, _In_reads_(_Count) const _Elem* const _Ptr, _CRT_GUARDOVERFLOW const size_type _Count) {
        // insert [_Ptr, _Ptr + _Count) at _Off
        _Mypair._Myval2._Check_offset(_Off);
        const size_type _Old_size = _Mypair._Myval2._Mysize;

        // checking for overlapping ranges is technically UB (considering string literals), so just always reallocate
        // and copy to the new buffer if constant evaluated
#ifdef __cpp_lib_constexpr_string
        const bool _Check_overlap = _Count <= _Mypair._Myval2._Myres - _Old_size && !_STD is_constant_evaluated();
#else // ^^^ __cpp_lib_constexpr_string / !__cpp_lib_constexpr_string vvv
        const bool _Check_overlap = _Count <= _Mypair._Myval2._Myres - _Old_size;
#endif // __cpp_lib_constexpr_string

        if (_Check_overlap) {
            _Mypair._Myval2._Mysize = _Old_size + _Count;
            _Elem* const _Old_ptr = _Mypair._Myval2._Myptr();
            _Elem* const _Insert_at = _Old_ptr + _Off;
            // the range [_Ptr, _Ptr + _Ptr_shifted_after) is left alone by moving the suffix out,
            // while the range [_Ptr + _Ptr_shifted_after, _Ptr + _Count) shifts down by _Count
            size_type _Ptr_shifted_after;
            if (_Ptr + _Count <= _Insert_at || _Ptr > _Old_ptr + _Old_size) {
                // inserted content is before the shifted region, or does not alias
                _Ptr_shifted_after = _Count; // none of _Ptr's data shifts
            }
            else if (_Insert_at <= _Ptr) { // all of [_Ptr, _Ptr + _Count) shifts
                _Ptr_shifted_after = 0;
            }
            else { // [_Ptr, _Ptr + _Count) contains _Insert_at, so only the part after _Insert_at shifts
                _Ptr_shifted_after = static_cast<size_type>(_Insert_at - _Ptr);
            }

            _Traits::move(_Insert_at + _Count, _Insert_at, _Old_size - _Off + 1); // move suffix + null down
            _Traits::copy(_Insert_at, _Ptr, _Ptr_shifted_after);
            _Traits::copy(
                _Insert_at + _Ptr_shifted_after, _Ptr + _Count + _Ptr_shifted_after, _Count - _Ptr_shifted_after);
            return *this;
        }

        return _Reallocate_grow_by(
            _Count,
            [](_Elem* const _New_ptr, const _Elem* const _Old_ptr, const size_type _Old_size, const size_type _Off,
                const _Elem* const _Ptr, const size_type _Count) {
                    _Traits::copy(_New_ptr, _Old_ptr, _Off);
                    _Traits::copy(_New_ptr + _Off, _Ptr, _Count);
                    _Traits::copy(_New_ptr + _Off + _Count, _Old_ptr + _Off, _Old_size - _Off + 1);
            },
            _Off, _Ptr, _Count);
    }

    _CONSTEXPR20_CONTAINER basic_string& insert(const size_type _Off, _In_z_ const _Elem* const _Ptr) {
        // insert [_Ptr, <null>) at _Off
        return insert(_Off, _Ptr, _Convert_size<size_type>(_Traits::length(_Ptr)));
    }

    _CONSTEXPR20_CONTAINER basic_string& insert(
        const size_type _Off, _CRT_GUARDOVERFLOW const size_type _Count, const _Elem _Ch) {
        // insert _Count * _Ch at _Off
        _Mypair._Myval2._Check_offset(_Off);
        const size_type _Old_size = _Mypair._Myval2._Mysize;
        if (_Count <= _Mypair._Myval2._Myres - _Old_size) {
            _Mypair._Myval2._Mysize = _Old_size + _Count;
            _Elem* const _Old_ptr = _Mypair._Myval2._Myptr();
            _Elem* const _Insert_at = _Old_ptr + _Off;
            _Traits::move(_Insert_at + _Count, _Insert_at, _Old_size - _Off + 1); // move suffix + null down
            _Traits::assign(_Insert_at, _Count, _Ch); // fill hole
            return *this;
        }

        return _Reallocate_grow_by(
            _Count,
            [](_Elem* const _New_ptr, const _Elem* const _Old_ptr, const size_type _Old_size, const size_type _Off,
                const size_type _Count, const _Elem _Ch) {
                    _Traits::copy(_New_ptr, _Old_ptr, _Off);
                    _Traits::assign(_New_ptr + _Off, _Count, _Ch);
                    _Traits::copy(_New_ptr + _Off + _Count, _Old_ptr + _Off, _Old_size - _Off + 1);
            },
            _Off, _Count, _Ch);
    }

    _CONSTEXPR20_CONTAINER iterator insert(const const_iterator _Where, const _Elem _Ch) { // insert _Ch at _Where
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_Where._Getcont() == _STD addressof(_Mypair._Myval2), "string iterator incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        const auto _Off = static_cast<size_type>(_Unfancy(_Where._Ptr) - _Mypair._Myval2._Myptr());
        insert(_Off, 1, _Ch);
        return begin() + static_cast<difference_type>(_Off);
    }

    _CONSTEXPR20_CONTAINER iterator insert(
        const const_iterator _Where, _CRT_GUARDOVERFLOW const size_type _Count, const _Elem _Ch) {
        // insert _Count * _Elem at _Where
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_Where._Getcont() == _STD addressof(_Mypair._Myval2), "string iterator incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        const auto _Off = static_cast<size_type>(_Unfancy(_Where._Ptr) - _Mypair._Myval2._Myptr());
        insert(_Off, _Count, _Ch);
        return begin() + static_cast<difference_type>(_Off);
    }

    template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20_CONTAINER iterator insert(const const_iterator _Where, const _Iter _First, const _Iter _Last) {
        // insert [_First, _Last) at _Where, input iterators
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_Where._Getcont() == _STD addressof(_Mypair._Myval2), "string iterator incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        const auto _Off = static_cast<size_type>(_Unfancy(_Where._Ptr) - _Mypair._Myval2._Myptr());
        _Adl_verify_range(_First, _Last);
        const auto _UFirst = _Get_unwrapped(_First);
        const auto _ULast = _Get_unwrapped(_Last);
        if constexpr (_Is_elem_cptr<decltype(_UFirst)>::value) {
            insert(_Off, _UFirst, _Convert_size<size_type>(static_cast<size_t>(_ULast - _UFirst)));
        }
        else {
            const basic_string _Right(_UFirst, _ULast, get_allocator());
            insert(_Off, _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
        }

        return begin() + static_cast<difference_type>(_Off);
    }

    _CONSTEXPR20_CONTAINER basic_string& erase(const size_type _Off = 0) { // erase elements [_Off, ...)
        _Mypair._Myval2._Check_offset(_Off);
        _Eos(_Off);
        return *this;
    }

private:
    _CONSTEXPR20_CONTAINER basic_string& _Erase_noexcept(const size_type _Off, size_type _Count) noexcept {
        _Count = _Mypair._Myval2._Clamp_suffix_size(_Off, _Count);
        const size_type _Old_size = _Mypair._Myval2._Mysize;
        _Elem* const _My_ptr = _Mypair._Myval2._Myptr();
        _Elem* const _Erase_at = _My_ptr + _Off;
        const size_type _New_size = _Old_size - _Count;
        _Mypair._Myval2._Mysize = _New_size;
        _Traits::move(_Erase_at, _Erase_at + _Count, _New_size - _Off + 1); // move suffix + null up
        return *this;
    }

public:
    _CONSTEXPR20_CONTAINER basic_string& erase(const size_type _Off, const size_type _Count) {
        // erase elements [_Off, _Off + _Count)
        _Mypair._Myval2._Check_offset(_Off);
        return _Erase_noexcept(_Off, _Count);
    }

    _CONSTEXPR20_CONTAINER iterator erase(const const_iterator _Where) noexcept /* strengthened */ {
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_Where._Getcont() == _STD addressof(_Mypair._Myval2), "string iterator incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        const auto _Off = static_cast<size_type>(_Unfancy(_Where._Ptr) - _Mypair._Myval2._Myptr());
        _Erase_noexcept(_Off, 1);
        return begin() + static_cast<difference_type>(_Off);
    }

    _CONSTEXPR20_CONTAINER iterator erase(const const_iterator _First, const const_iterator _Last) noexcept
        /* strengthened */ {
        _Adl_verify_range(_First, _Last);
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_First._Getcont() == _STD addressof(_Mypair._Myval2), "string iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        const auto _Off = static_cast<size_type>(_Unfancy(_First._Ptr) - _Mypair._Myval2._Myptr());
        _Erase_noexcept(_Off, static_cast<size_type>(_Last._Ptr - _First._Ptr));
        return begin() + static_cast<difference_type>(_Off);
    }

    _CONSTEXPR20_CONTAINER void clear() noexcept { // erase all
        _Eos(0);
    }

    _CONSTEXPR20_CONTAINER basic_string& replace(
        const size_type _Off, const size_type _Nx, const basic_string& _Right) {
        // replace [_Off, _Off + _Nx) with _Right
        return replace(_Off, _Nx, _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
    }

    _CONSTEXPR20_CONTAINER basic_string& replace(const size_type _Off, size_type _Nx, const basic_string& _Right,
        const size_type _Roff, size_type _Count = npos) {
        // replace [_Off, _Off + _Nx) with _Right [_Roff, _Roff + _Count)
        _Right._Mypair._Myval2._Check_offset(_Roff);
        _Count = _Right._Mypair._Myval2._Clamp_suffix_size(_Roff, _Count);
        return replace(_Off, _Nx, _Right._Mypair._Myval2._Myptr() + _Roff, _Count);
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& replace(
        const size_type _Off, const size_type _Nx, const _StringViewIsh& _Right) {
        // replace [_Off, _Off + _Nx) with _Right
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return replace(_Off, _Nx, _As_view.data(), _Convert_size<size_type>(_As_view.size()));
    }

    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& replace(const size_type _Off, const size_type _Nx,
        const _StringViewIsh& _Right, const size_type _Roff, const size_type _Count = npos) {
        // replace [_Off, _Off + _Nx) with _Right [_Roff, _Roff + _Count)
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return replace(_Off, _Nx, _As_view.substr(_Roff, _Count));
    }
#endif // _HAS_CXX17

    _CONSTEXPR20_CONTAINER basic_string& replace(
        const size_type _Off, size_type _Nx, _In_reads_(_Count) const _Elem* const _Ptr, const size_type _Count) {
        // replace [_Off, _Off + _Nx) with [_Ptr, _Ptr + _Count)
        _Mypair._Myval2._Check_offset(_Off);
        _Nx = _Mypair._Myval2._Clamp_suffix_size(_Off, _Nx);
        if (_Nx == _Count) { // size doesn't change, so a single move does the trick
            _Traits::move(_Mypair._Myval2._Myptr() + _Off, _Ptr, _Count);
            return *this;
        }

        const size_type _Old_size = _Mypair._Myval2._Mysize;
        const size_type _Suffix_size = _Old_size - _Nx - _Off + 1;
        if (_Count < _Nx) { // suffix shifts backwards; we don't have to move anything out of the way
            _Mypair._Myval2._Mysize = _Old_size - (_Nx - _Count);
            _Elem* const _Old_ptr = _Mypair._Myval2._Myptr();
            _Elem* const _Insert_at = _Old_ptr + _Off;
            _Traits::move(_Insert_at, _Ptr, _Count);
            _Traits::move(_Insert_at + _Count, _Insert_at + _Nx, _Suffix_size);
            return *this;
        }

        const size_type _Growth = static_cast<size_type>(_Count - _Nx);

        // checking for overlapping ranges is technically UB (considering string literals), so just always reallocate
        // and copy to the new buffer if constant evaluated
#ifdef __cpp_lib_constexpr_string
        if (!_STD is_constant_evaluated())
#endif // __cpp_lib_constexpr_string
        {
            if (_Growth <= _Mypair._Myval2._Myres - _Old_size) { // growth fits
                _Mypair._Myval2._Mysize = _Old_size + _Growth;
                _Elem* const _Old_ptr = _Mypair._Myval2._Myptr();
                _Elem* const _Insert_at = _Old_ptr + _Off;
                _Elem* const _Suffix_at = _Insert_at + _Nx;

                size_type _Ptr_shifted_after; // see rationale in insert
                if (_Ptr + _Count <= _Insert_at || _Ptr > _Old_ptr + _Old_size) {
                    _Ptr_shifted_after = _Count;
                }
                else if (_Suffix_at <= _Ptr) {
                    _Ptr_shifted_after = 0;
                }
                else {
                    _Ptr_shifted_after = static_cast<size_type>(_Suffix_at - _Ptr);
                }

                _Traits::move(_Suffix_at + _Growth, _Suffix_at, _Suffix_size);
                // next case must be move, in case _Ptr begins before _Insert_at and contains part of the hole;
                // this case doesn't occur in insert because the new content must come from outside the removed
                // content there (because in insert there is no removed content)
                _Traits::move(_Insert_at, _Ptr, _Ptr_shifted_after);
                // the next case can be copy, because it comes from the chunk moved out of the way in the
                // first move, and the hole we're filling can't alias the chunk we moved out of the way
                _Traits::copy(
                    _Insert_at + _Ptr_shifted_after, _Ptr + _Growth + _Ptr_shifted_after, _Count - _Ptr_shifted_after);
                return *this;
            }
        }

        return _Reallocate_grow_by(
            _Growth,
            [](_Elem* const _New_ptr, const _Elem* const _Old_ptr, const size_type _Old_size, const size_type _Off,
                const size_type _Nx, const _Elem* const _Ptr, const size_type _Count) {
                    _Traits::copy(_New_ptr, _Old_ptr, _Off);
                    _Traits::copy(_New_ptr + _Off, _Ptr, _Count);
                    _Traits::copy(_New_ptr + _Off + _Count, _Old_ptr + _Off + _Nx, _Old_size - _Nx - _Off + 1);
            },
            _Off, _Nx, _Ptr, _Count);
    }

    _CONSTEXPR20_CONTAINER basic_string& replace(
        const size_type _Off, const size_type _Nx, _In_z_ const _Elem* const _Ptr) {
        // replace [_Off, _Off + _Nx) with [_Ptr, <null>)
        return replace(_Off, _Nx, _Ptr, _Convert_size<size_type>(_Traits::length(_Ptr)));
    }

    _CONSTEXPR20_CONTAINER basic_string& replace(
        const size_type _Off, size_type _Nx, const size_type _Count, const _Elem _Ch) {
        // replace [_Off, _Off + _Nx) with _Count * _Ch
        _Mypair._Myval2._Check_offset(_Off);
        _Nx = _Mypair._Myval2._Clamp_suffix_size(_Off, _Nx);
        if (_Count == _Nx) {
            _Traits::assign(_Mypair._Myval2._Myptr() + _Off, _Count, _Ch);
            return *this;
        }

        const size_type _Old_size = _Mypair._Myval2._Mysize;
        if (_Count < _Nx || _Count - _Nx <= _Mypair._Myval2._Myres - _Old_size) {
            // either we are shrinking, or the growth fits
            _Mypair._Myval2._Mysize = _Old_size + _Count - _Nx; // may temporarily overflow;
                                                                // OK because size_type must be unsigned
            _Elem* const _Old_ptr = _Mypair._Myval2._Myptr();
            _Elem* const _Insert_at = _Old_ptr + _Off;
            _Traits::move(_Insert_at + _Count, _Insert_at + _Nx, _Old_size - _Nx - _Off + 1);
            _Traits::assign(_Insert_at, _Count, _Ch);
            return *this;
        }

        return _Reallocate_grow_by(
            _Count - _Nx,
            [](_Elem* const _New_ptr, const _Elem* const _Old_ptr, const size_type _Old_size, const size_type _Off,
                const size_type _Nx, const size_type _Count, const _Elem _Ch) {
                    _Traits::copy(_New_ptr, _Old_ptr, _Off);
                    _Traits::assign(_New_ptr + _Off, _Count, _Ch);
                    _Traits::copy(_New_ptr + _Off + _Count, _Old_ptr + _Off + _Nx, _Old_size - _Nx - _Off + 1);
            },
            _Off, _Nx, _Count, _Ch);
    }

    _CONSTEXPR20_CONTAINER basic_string& replace(
        const const_iterator _First, const const_iterator _Last, const basic_string& _Right) {
        // replace [_First, _Last) with _Right
        _Adl_verify_range(_First, _Last);
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_First._Getcont() == _STD addressof(_Mypair._Myval2), "string iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        return replace(static_cast<size_type>(_Unfancy(_First._Ptr) - _Mypair._Myval2._Myptr()),
            static_cast<size_type>(_Last._Ptr - _First._Ptr), _Right);
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _CONSTEXPR20_CONTAINER basic_string& replace(
        const const_iterator _First, const const_iterator _Last, const _StringViewIsh& _Right) {
        // replace [_First, _Last) with _Right
        _Adl_verify_range(_First, _Last);
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_First._Getcont() == _STD addressof(_Mypair._Myval2), "string iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        return replace(static_cast<size_type>(_Unfancy(_First._Ptr) - _Mypair._Myval2._Myptr()),
            static_cast<size_type>(_Last._Ptr - _First._Ptr), _Right);
    }
#endif // _HAS_CXX17

    _CONSTEXPR20_CONTAINER basic_string& replace(const const_iterator _First, const const_iterator _Last,
        _In_reads_(_Count) const _Elem* const _Ptr, const size_type _Count) {
        // replace [_First, _Last) with [_Ptr, _Ptr + _Count)
        _Adl_verify_range(_First, _Last);
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_First._Getcont() == _STD addressof(_Mypair._Myval2), "string iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        return replace(static_cast<size_type>(_Unfancy(_First._Ptr) - _Mypair._Myval2._Myptr()),
            static_cast<size_type>(_Last._Ptr - _First._Ptr), _Ptr, _Count);
    }

    _CONSTEXPR20_CONTAINER basic_string& replace(
        const const_iterator _First, const const_iterator _Last, _In_z_ const _Elem* const _Ptr) {
        // replace [_First, _Last) with [_Ptr, <null>)
        _Adl_verify_range(_First, _Last);
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_First._Getcont() == _STD addressof(_Mypair._Myval2), "string iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        return replace(static_cast<size_type>(_Unfancy(_First._Ptr) - _Mypair._Myval2._Myptr()),
            static_cast<size_type>(_Last._Ptr - _First._Ptr), _Ptr);
    }

    _CONSTEXPR20_CONTAINER basic_string& replace(
        const const_iterator _First, const const_iterator _Last, const size_type _Count, const _Elem _Ch) {
        // replace [_First, _Last) with _Count * _Ch
        _Adl_verify_range(_First, _Last);
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_First._Getcont() == _STD addressof(_Mypair._Myval2), "string iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        return replace(static_cast<size_type>(_Unfancy(_First._Ptr) - _Mypair._Myval2._Myptr()),
            static_cast<size_type>(_Last._Ptr - _First._Ptr), _Count, _Ch);
    }

    template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20_CONTAINER basic_string& replace(
        const const_iterator _First, const const_iterator _Last, const _Iter _First2, const _Iter _Last2) {
        // replace [_First, _Last) with [_First2, _Last2), input iterators
        _Adl_verify_range(_First, _Last);
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_First._Getcont() == _STD addressof(_Mypair._Myval2), "string iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        const auto _Off = static_cast<size_type>(_Unfancy(_First._Ptr) - _Mypair._Myval2._Myptr());
        const auto _Length = static_cast<size_type>(_Last._Ptr - _First._Ptr);
        _Adl_verify_range(_First2, _Last2);
        const auto _UFirst2 = _Get_unwrapped(_First2);
        const auto _ULast2 = _Get_unwrapped(_Last2);
        if constexpr (_Is_elem_cptr<decltype(_UFirst2)>::value) {
            return replace(_Off, _Length, _UFirst2, _Convert_size<size_type>(static_cast<size_t>(_ULast2 - _UFirst2)));
        }
        else {
            const basic_string _Right(_UFirst2, _ULast2, get_allocator());
            return replace(_Off, _Length, _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
        }
    }

    _NODISCARD _CONSTEXPR20_CONTAINER iterator begin() noexcept {
        return iterator(_Refancy<pointer>(_Mypair._Myval2._Myptr()), _STD addressof(_Mypair._Myval2));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_iterator begin() const noexcept {
        return const_iterator(_Refancy<const_pointer>(_Mypair._Myval2._Myptr()), _STD addressof(_Mypair._Myval2));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER iterator end() noexcept {
        return iterator(
            _Refancy<pointer>(_Mypair._Myval2._Myptr()) + static_cast<difference_type>(_Mypair._Myval2._Mysize),
            _STD addressof(_Mypair._Myval2));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_iterator end() const noexcept {
        return const_iterator(
            _Refancy<const_pointer>(_Mypair._Myval2._Myptr()) + static_cast<difference_type>(_Mypair._Myval2._Mysize),
            _STD addressof(_Mypair._Myval2));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _Elem* _Unchecked_begin() noexcept {
        return _Mypair._Myval2._Myptr();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const _Elem* _Unchecked_begin() const noexcept {
        return _Mypair._Myval2._Myptr();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _Elem* _Unchecked_end() noexcept {
        return _Mypair._Myval2._Myptr() + _Mypair._Myval2._Mysize;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const _Elem* _Unchecked_end() const noexcept {
        return _Mypair._Myval2._Myptr() + _Mypair._Myval2._Mysize;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    _NODISCARD _CONSTEXPR20_CONTAINER reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_iterator cbegin() const noexcept {
        return begin();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_iterator cend() const noexcept {
        return end();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reverse_iterator crend() const noexcept {
        return rend();
    }

    _CONSTEXPR20_CONTAINER void shrink_to_fit() { // reduce capacity
        auto& _My_data = _Mypair._Myval2;

#ifdef __cpp_lib_constexpr_string
        if (!_STD is_constant_evaluated())
#endif // __cpp_lib_constexpr_string
        {
            if (!_My_data._Large_string_engaged()) { // can't shrink from small mode
                return;
            }

            if (_My_data._Mysize < _BUF_SIZE) {
                _Become_small();
                return;
            }
        }

        size_type _Target_capacity = (_STD min)(_My_data._Mysize | _ALLOC_MASK, max_size());
#ifdef __cpp_lib_constexpr_string
        // must allocate at least _BUF_SIZE space
        _Target_capacity = (_STD max)(_Target_capacity, _BUF_SIZE);
#endif // __cpp_lib_constexpr_string

        if (_Target_capacity < _My_data._Myres) { // worth shrinking, do it
            auto& _Al = _Getal();
            const pointer _New_ptr = _Al.allocate(_Target_capacity + 1); // throws

#ifdef __cpp_lib_constexpr_string
            if (_STD is_constant_evaluated()) { // Begin the lifetimes of the objects before copying to avoid UB
                _Traits::assign(_Unfancy(_New_ptr), _Target_capacity + 1, _Elem());
            }
#endif // __cpp_lib_constexpr_string

            _My_data._Orphan_all();
            _Traits::copy(_Unfancy(_New_ptr), _Unfancy(_My_data._Bx._Ptr), _My_data._Mysize + 1);
            _Al.deallocate(_My_data._Bx._Ptr, _My_data._Myres + 1);
            _My_data._Bx._Ptr = _New_ptr;
            _My_data._Myres = _Target_capacity;
        }
    }

    _NODISCARD _CONSTEXPR20_CONTAINER reference at(const size_type _Off) {
        _Mypair._Myval2._Check_offset_exclusive(_Off);
        return _Mypair._Myval2._Myptr()[_Off];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reference at(const size_type _Off) const {
        _Mypair._Myval2._Check_offset_exclusive(_Off);
        return _Mypair._Myval2._Myptr()[_Off];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER reference operator[](const size_type _Off) noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Off <= _Mypair._Myval2._Mysize, "string subscript out of range");
#endif // _CONTAINER_DEBUG_LEVEL > 0
        return _Mypair._Myval2._Myptr()[_Off];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reference operator[](const size_type _Off) const noexcept
        /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Off <= _Mypair._Myval2._Mysize, "string subscript out of range");
#endif // _CONTAINER_DEBUG_LEVEL > 0
        return _Mypair._Myval2._Myptr()[_Off];
    }

#if _HAS_CXX17
    /* implicit */ _CONSTEXPR20_CONTAINER operator basic_string_view<_Elem, _Traits>() const noexcept {
        // return a string_view around *this's character-type sequence
        return basic_string_view<_Elem, _Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize);
    }
#endif // _HAS_CXX17

    _CONSTEXPR20_CONTAINER void push_back(const _Elem _Ch) { // insert element at end
        const size_type _Old_size = _Mypair._Myval2._Mysize;
        if (_Old_size < _Mypair._Myval2._Myres) {
            _Mypair._Myval2._Mysize = _Old_size + 1;
            _Elem* const _Ptr = _Mypair._Myval2._Myptr();
            _Traits::assign(_Ptr[_Old_size], _Ch);
            _Traits::assign(_Ptr[_Old_size + 1], _Elem());
            return;
        }

        _Reallocate_grow_by(
            1,
            [](_Elem* const _New_ptr, const _Elem* const _Old_ptr, const size_type _Old_size, const _Elem _Ch) {
                _Traits::copy(_New_ptr, _Old_ptr, _Old_size);
                _Traits::assign(_New_ptr[_Old_size], _Ch);
                _Traits::assign(_New_ptr[_Old_size + 1], _Elem());
            },
            _Ch);
    }

    _CONSTEXPR20_CONTAINER void pop_back() noexcept /* strengthened */ {
        const size_type _Old_size = _Mypair._Myval2._Mysize;
#if _ITERATOR_DEBUG_LEVEL >= 1
        _STL_VERIFY(_Old_size != 0, "invalid to pop_back empty string");
#endif // _ITERATOR_DEBUG_LEVEL >= 1
        _Eos(_Old_size - 1);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER reference front() noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Mypair._Myval2._Mysize != 0, "front() called on empty string");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        return _Mypair._Myval2._Myptr()[0];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reference front() const noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Mypair._Myval2._Mysize != 0, "front() called on empty string");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        return _Mypair._Myval2._Myptr()[0];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER reference back() noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Mypair._Myval2._Mysize != 0, "back() called on empty string");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        return _Mypair._Myval2._Myptr()[_Mypair._Myval2._Mysize - 1];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER const_reference back() const noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Mypair._Myval2._Mysize != 0, "back() called on empty string");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        return _Mypair._Myval2._Myptr()[_Mypair._Myval2._Mysize - 1];
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _Ret_z_ const _Elem* c_str() const noexcept {
        return _Mypair._Myval2._Myptr();
    }

    _NODISCARD _CONSTEXPR20_CONTAINER _Ret_z_ const _Elem* data() const noexcept {
        return _Mypair._Myval2._Myptr();
    }

#if _HAS_CXX17
    _NODISCARD _CONSTEXPR20_CONTAINER _Ret_z_ _Elem* data() noexcept {
        return _Mypair._Myval2._Myptr();
    }
#endif // _HAS_CXX17

    _NODISCARD _CONSTEXPR20_CONTAINER size_type length() const noexcept {
        return _Mypair._Myval2._Mysize;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type size() const noexcept {
        return _Mypair._Myval2._Mysize;
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type max_size() const noexcept {
        const size_type _Alloc_max = _Alty_traits::max_size(_Getal());
        const size_type _Storage_max = // can always store small string
            (_STD max)(_Alloc_max, static_cast<size_type>(_BUF_SIZE));
        return (_STD min)(static_cast<size_type>((numeric_limits<difference_type>::max)()),
            _Storage_max - 1 // -1 is for null terminator and/or npos
            );
    }

    _CONSTEXPR20_CONTAINER void resize(_CRT_GUARDOVERFLOW const size_type _Newsize, const _Elem _Ch = _Elem()) {
        // determine new length, padding with _Ch elements as needed
        const size_type _Old_size = size();
        if (_Newsize <= _Old_size) {
            _Eos(_Newsize);
        }
        else {
            append(_Newsize - _Old_size, _Ch);
        }
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type capacity() const noexcept {
        return _Mypair._Myval2._Myres;
    }

#if _HAS_CXX20
    _CONSTEXPR20_CONTAINER void reserve(_CRT_GUARDOVERFLOW const size_type _Newcap) {
        // determine new minimum length of allocated storage
        if (_Mypair._Myval2._Myres >= _Newcap) { // requested capacity is not larger than current capacity, ignore
            return; // nothing to do
        }

        const size_type _Old_size = _Mypair._Myval2._Mysize;
        _Reallocate_grow_by(
            _Newcap - _Old_size, [](_Elem* const _New_ptr, const _Elem* const _Old_ptr, const size_type _Old_size) {
                _Traits::copy(_New_ptr, _Old_ptr, _Old_size + 1);
            });

        _Mypair._Myval2._Mysize = _Old_size;
    }

    _CXX20_DEPRECATE_STRING_RESERVE_WITHOUT_ARGUMENT void reserve() {
        if (_Mypair._Myval2._Mysize == 0 && _Mypair._Myval2._Large_string_engaged()) {
            _Become_small();
        }
    }
#else // _HAS_CXX20
    void reserve(_CRT_GUARDOVERFLOW const size_type _Newcap = 0) { // determine new minimum length of allocated storage
        if (_Mypair._Myval2._Mysize > _Newcap) { // requested capacity is not large enough for current size, ignore
            return; // nothing to do
        }

        if (_Mypair._Myval2._Myres == _Newcap) { // we're already at the requested capacity
            return; // nothing to do
        }

        if (_Mypair._Myval2._Myres < _Newcap) { // reallocate to grow
            const size_type _Old_size = _Mypair._Myval2._Mysize;
            _Reallocate_grow_by(
                _Newcap - _Old_size, [](_Elem* const _New_ptr, const _Elem* const _Old_ptr, const size_type _Old_size) {
                    _Traits::copy(_New_ptr, _Old_ptr, _Old_size + 1);
                });

            _Mypair._Myval2._Mysize = _Old_size;
            return;
        }

        if (_BUF_SIZE > _Newcap && _Mypair._Myval2._Large_string_engaged()) {
            // deallocate everything; switch back to "small" mode
            _Become_small();
            return;
        }

        // ignore requests to reserve to [_BUF_SIZE, _Myres)
    }
#endif // _HAS_CXX20

    _NODISCARD _CONSTEXPR20_CONTAINER bool empty() const noexcept {
        return size() == 0;
    }

    _CONSTEXPR20_CONTAINER size_type copy(
        _Out_writes_(_Count) _Elem* const _Ptr, size_type _Count, const size_type _Off = 0) const {
        // copy [_Off, _Off + _Count) to [_Ptr, _Ptr + _Count)
        _Mypair._Myval2._Check_offset(_Off);
        _Count = _Mypair._Myval2._Clamp_suffix_size(_Off, _Count);
        _Traits::copy(_Ptr, _Mypair._Myval2._Myptr() + _Off, _Count);
        return _Count;
    }

    _CONSTEXPR20_CONTAINER _Pre_satisfies_(_Dest_size >= _Count) size_type
        _Copy_s(_Out_writes_all_(_Dest_size) _Elem* const _Dest, const size_type _Dest_size, size_type _Count,
            const size_type _Off = 0) const {
        // copy [_Off, _Off + _Count) to [_Dest, _Dest + _Dest_size)
        _Mypair._Myval2._Check_offset(_Off);
        _Count = _Mypair._Myval2._Clamp_suffix_size(_Off, _Count);
        _Traits::_Copy_s(_Dest, _Dest_size, _Mypair._Myval2._Myptr() + _Off, _Count);
        return _Count;
    }

    void _Swap_bx_large_with_small(_Scary_val& _Starts_large, _Scary_val& _Starts_small) {
        // exchange a string in large mode with one in small mode
        // (not _CONSTEXPR20_CONTAINER; SSO should be disabled in a constexpr context)

        const pointer _Ptr = _Starts_large._Bx._Ptr;
        _Destroy_in_place(_Starts_large._Bx._Ptr);
        _Traits::copy(_Starts_large._Bx._Buf, _Starts_small._Bx._Buf, _BUF_SIZE);
        _Construct_in_place(_Starts_small._Bx._Ptr, _Ptr);
    }

    _CONSTEXPR20_CONTAINER void _Swap_data(basic_string& _Right) {
        auto& _My_data = _Mypair._Myval2;
        auto& _Right_data = _Right._Mypair._Myval2;

        if constexpr (_Can_memcpy_val) {
#ifdef __cpp_lib_constexpr_string
            if (!_STD is_constant_evaluated())
#endif // __cpp_lib_constexpr_string
            {
                const auto _My_data_mem =
                    reinterpret_cast<unsigned char*>(_STD addressof(_My_data)) + _Memcpy_val_offset;
                const auto _Right_data_mem =
                    reinterpret_cast<unsigned char*>(_STD addressof(_Right_data)) + _Memcpy_val_offset;
                unsigned char _Temp_mem[_Memcpy_val_size];
                _CSTD memcpy(_Temp_mem, _My_data_mem, _Memcpy_val_size);
                _CSTD memcpy(_My_data_mem, _Right_data_mem, _Memcpy_val_size);
                _CSTD memcpy(_Right_data_mem, _Temp_mem, _Memcpy_val_size);
                return;
            }
        }

        const bool _My_large = _My_data._Large_string_engaged();
        const bool _Right_large = _Right_data._Large_string_engaged();
        if (_My_large) {
            if (_Right_large) { // swap buffers, iterators preserved
                _Swap_adl(_My_data._Bx._Ptr, _Right_data._Bx._Ptr);
            }
            else { // swap large with small
                _Swap_bx_large_with_small(_My_data, _Right_data);
            }
        }
        else {
            if (_Right_large) { // swap small with large
                _Swap_bx_large_with_small(_Right_data, _My_data);
            }
            else {
                _Elem _Temp_buf[_BUF_SIZE];
                _Traits::copy(_Temp_buf, _My_data._Bx._Buf, _BUF_SIZE);
                _Traits::copy(_My_data._Bx._Buf, _Right_data._Bx._Buf, _BUF_SIZE);
                _Traits::copy(_Right_data._Bx._Buf, _Temp_buf, _BUF_SIZE);
            }
        }

        _STD swap(_My_data._Mysize, _Right_data._Mysize);
        _STD swap(_My_data._Myres, _Right_data._Myres);
    }

    _CONSTEXPR20_CONTAINER void swap(basic_string& _Right) noexcept /* strengthened */ {
        if (this != _STD addressof(_Right)) {
            _Pocs(_Getal(), _Right._Getal());

#if _ITERATOR_DEBUG_LEVEL != 0
            const bool _My_large = _Mypair._Myval2._Large_string_engaged();
            const bool _Right_large = _Right._Mypair._Myval2._Large_string_engaged();
            if (!_My_large) {
                _Mypair._Myval2._Orphan_all();
            }

            if (!_Right_large) {
                _Right._Mypair._Myval2._Orphan_all();
            }

            if (_My_large || _Right_large) {
                _Mypair._Myval2._Swap_proxy_and_iterators(_Right._Mypair._Myval2);
            }
#endif // _ITERATOR_DEBUG_LEVEL != 0
        }

        _Swap_data(_Right);
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _NODISCARD _CONSTEXPR20_CONTAINER size_type find(const _StringViewIsh& _Right, const size_type _Off = 0) const {
        // look for _Right beginning at or after _Off
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return static_cast<size_type>(_Traits_find<_Traits>(
            _Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _As_view.data(), _As_view.size()));
    }
#endif // _HAS_CXX17

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find(
        const basic_string& _Right, const size_type _Off = 0) const noexcept {
        // look for _Right beginning at or after _Off
        return static_cast<size_type>(_Traits_find<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off,
            _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept /* strengthened */ {
        // look for [_Ptr, _Ptr + _Count) beginning at or after _Off
        return static_cast<size_type>(
            _Traits_find<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _Ptr, _Count));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find(
        _In_z_ const _Elem* const _Ptr, const size_type _Off = 0) const noexcept /* strengthened */ {
        // look for [_Ptr, <null>) beginning at or after _Off
        return static_cast<size_type>(_Traits_find<_Traits>(
            _Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _Ptr, _Traits::length(_Ptr)));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find(const _Elem _Ch, const size_type _Off = 0) const noexcept
        /* strengthened */ {
        // look for _Ch at or after _Off
        return static_cast<size_type>(
            _Traits_find_ch<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _Ch));
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _NODISCARD _CONSTEXPR20_CONTAINER size_type rfind(const _StringViewIsh& _Right, const size_type _Off = npos) const {
        // look for _Right beginning before _Off
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return static_cast<size_type>(_Traits_rfind<_Traits>(
            _Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _As_view.data(), _As_view.size()));
    }
#endif // _HAS_CXX17

    _NODISCARD _CONSTEXPR20_CONTAINER size_type rfind(
        const basic_string& _Right, const size_type _Off = npos) const noexcept {
        // look for _Right beginning before _Off
        return static_cast<size_type>(_Traits_rfind<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off,
            _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type rfind(_In_reads_(_Count) const _Elem* const _Ptr, const size_type _Off,
        const size_type _Count) const noexcept /* strengthened */ {
        // look for [_Ptr, _Ptr + _Count) beginning before _Off
        return static_cast<size_type>(
            _Traits_rfind<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _Ptr, _Count));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type rfind(
        _In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const noexcept /* strengthened */ {
        // look for [_Ptr, <null>) beginning before _Off
        return static_cast<size_type>(_Traits_rfind<_Traits>(
            _Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _Ptr, _Traits::length(_Ptr)));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type rfind(const _Elem _Ch, const size_type _Off = npos) const noexcept
        /* strengthened */ {
        // look for _Ch before _Off
        return static_cast<size_type>(
            _Traits_rfind_ch<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _Ch));
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_first_of(
        const _StringViewIsh& _Right, const size_type _Off = 0) const {
        // look for one of _Right at or after _Off
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return static_cast<size_type>(_Traits_find_first_of<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize,
            _Off, _As_view.data(), _As_view.size(), _Is_specialization<_Traits, char_traits>{}));
    }
#endif // _HAS_CXX17

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_first_of(
        const basic_string& _Right, const size_type _Off = 0) const noexcept {
        // look for one of _Right at or after _Off
        return static_cast<size_type>(_Traits_find_first_of<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize,
            _Off, _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize,
            _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_first_of(_In_reads_(_Count) const _Elem* const _Ptr,
        const size_type _Off, const size_type _Count) const noexcept /* strengthened */ {
        // look for one of [_Ptr, _Ptr + _Count) at or after _Off
        return static_cast<size_type>(_Traits_find_first_of<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize,
            _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_first_of(
        _In_z_ const _Elem* const _Ptr, const size_type _Off = 0) const noexcept /* strengthened */ {
        // look for one of [_Ptr, <null>) at or after _Off
        return static_cast<size_type>(_Traits_find_first_of<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize,
            _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_first_of(const _Elem _Ch, const size_type _Off = 0) const noexcept
        /* strengthened */ {
        // look for _Ch at or after _Off
        return static_cast<size_type>(
            _Traits_find_ch<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _Ch));
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_last_of(
        const _StringViewIsh& _Right, const size_type _Off = npos) const {
        // look for one of _Right before _Off
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return static_cast<size_type>(_Traits_find_last_of<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize,
            _Off, _As_view.data(), _As_view.size(), _Is_specialization<_Traits, char_traits>{}));
    }
#endif // _HAS_CXX17

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_last_of(
        const basic_string& _Right, size_type _Off = npos) const noexcept {
        // look for one of _Right before _Off
        return static_cast<size_type>(_Traits_find_last_of<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize,
            _Off, _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize,
            _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_last_of(_In_reads_(_Count) const _Elem* const _Ptr,
        const size_type _Off, const size_type _Count) const noexcept /* strengthened */ {
        // look for one of [_Ptr, _Ptr + _Count) before _Off
        return static_cast<size_type>(_Traits_find_last_of<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize,
            _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_last_of(
        _In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const noexcept /* strengthened */ {
        // look for one of [_Ptr, <null>) before _Off
        return static_cast<size_type>(_Traits_find_last_of<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize,
            _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_last_of(
        const _Elem _Ch, const size_type _Off = npos) const noexcept /* strengthened */ {
        // look for _Ch before _Off
        return static_cast<size_type>(
            _Traits_rfind_ch<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _Ch));
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_first_not_of(
        const _StringViewIsh& _Right, const size_type _Off = 0) const {
        // look for none of _Right at or after _Off
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return static_cast<size_type>(
            _Traits_find_first_not_of<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _As_view.data(),
                _As_view.size(), _Is_specialization<_Traits, char_traits>{}));
    }
#endif // _HAS_CXX17

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_first_not_of(
        const basic_string& _Right, const size_type _Off = 0) const noexcept {
        // look for none of _Right at or after _Off
        return static_cast<size_type>(_Traits_find_first_not_of<_Traits>(_Mypair._Myval2._Myptr(),
            _Mypair._Myval2._Mysize, _Off, _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize,
            _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_first_not_of(_In_reads_(_Count) const _Elem* const _Ptr,
        const size_type _Off, const size_type _Count) const noexcept /* strengthened */ {
        // look for none of [_Ptr, _Ptr + _Count) at or after _Off
        return static_cast<size_type>(_Traits_find_first_not_of<_Traits>(_Mypair._Myval2._Myptr(),
            _Mypair._Myval2._Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_first_not_of(
        _In_z_ const _Elem* const _Ptr, size_type _Off = 0) const noexcept /* strengthened */ {
        // look for one of [_Ptr, <null>) at or after _Off
        return static_cast<size_type>(_Traits_find_first_not_of<_Traits>(_Mypair._Myval2._Myptr(),
            _Mypair._Myval2._Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_first_not_of(
        const _Elem _Ch, const size_type _Off = 0) const noexcept /* strengthened */ {
        // look for non-_Ch at or after _Off
        return static_cast<size_type>(
            _Traits_find_not_ch<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _Ch));
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_last_not_of(
        const _StringViewIsh& _Right, const size_type _Off = npos) const {
        // look for none of _Right before _Off
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return static_cast<size_type>(
            _Traits_find_last_not_of<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _As_view.data(),
                _As_view.size(), _Is_specialization<_Traits, char_traits>{}));
    }
#endif // _HAS_CXX17

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_last_not_of(
        const basic_string& _Right, const size_type _Off = npos) const noexcept {
        // look for none of _Right before _Off
        return static_cast<size_type>(_Traits_find_last_not_of<_Traits>(_Mypair._Myval2._Myptr(),
            _Mypair._Myval2._Mysize, _Off, _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize,
            _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_last_not_of(_In_reads_(_Count) const _Elem* const _Ptr,
        const size_type _Off, const size_type _Count) const noexcept /* strengthened */ {
        // look for none of [_Ptr, _Ptr + _Count) before _Off
        return static_cast<size_type>(_Traits_find_last_not_of<_Traits>(_Mypair._Myval2._Myptr(),
            _Mypair._Myval2._Mysize, _Off, _Ptr, _Count, _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_last_not_of(
        _In_z_ const _Elem* const _Ptr, const size_type _Off = npos) const noexcept /* strengthened */ {
        // look for none of [_Ptr, <null>) before _Off
        return static_cast<size_type>(_Traits_find_last_not_of<_Traits>(_Mypair._Myval2._Myptr(),
            _Mypair._Myval2._Mysize, _Off, _Ptr, _Traits::length(_Ptr), _Is_specialization<_Traits, char_traits>{}));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type find_last_not_of(
        const _Elem _Ch, const size_type _Off = npos) const noexcept /* strengthened */ {
        // look for non-_Ch before _Off
        return static_cast<size_type>(
            _Traits_rfind_not_ch<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Off, _Ch));
    }

#if _HAS_CXX17
    _NODISCARD bool _Starts_with(const basic_string_view<_Elem, _Traits> _Right) const noexcept {
        // Used exclusively by filesystem
        return basic_string_view<_Elem, _Traits>(*this)._Starts_with(_Right);
    }
#endif // _HAS_CXX17

    _NODISCARD _CONSTEXPR20_CONTAINER basic_string substr(
        const size_type _Off = 0, const size_type _Count = npos) const {
        // return [_Off, _Off + _Count) as new string
        return basic_string(*this, _Off, _Count, get_allocator());
    }

    _CONSTEXPR20_CONTAINER bool _Equal(const basic_string& _Right) const noexcept {
        // compare [0, size()) with _Right for equality
        return _Traits_equal<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize,
            _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
    }

    _CONSTEXPR20_CONTAINER bool _Equal(_In_z_ const _Elem* const _Ptr) const noexcept {
        // compare [0, size()) with _Ptr for equality
        return _Traits_equal<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Ptr, _Traits::length(_Ptr));
    }

#if _HAS_CXX17
    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _NODISCARD _CONSTEXPR20_CONTAINER int compare(const _StringViewIsh& _Right) const {
        // compare [0, size()) with _Right
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        return _Traits_compare<_Traits>(
            _Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _As_view.data(), _As_view.size());
    }

    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _NODISCARD _CONSTEXPR20_CONTAINER int compare(
        const size_type _Off, const size_type _Nx, const _StringViewIsh& _Right) const {
        // compare [_Off, _Off + _Nx) with _Right
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        _Mypair._Myval2._Check_offset(_Off);
        return _Traits_compare<_Traits>(_Mypair._Myval2._Myptr() + _Off, _Mypair._Myval2._Clamp_suffix_size(_Off, _Nx),
            _As_view.data(), _As_view.size());
    }

    template <class _StringViewIsh, _Is_string_view_ish<_StringViewIsh> = 0>
    _NODISCARD _CONSTEXPR20_CONTAINER int compare(const size_type _Off, const size_type _Nx,
        const _StringViewIsh& _Right, const size_type _Roff, const size_type _Count = npos) const {
        // compare [_Off, _Off + _Nx) with _Right [_Roff, _Roff + _Count)
        basic_string_view<_Elem, _Traits> _As_view = _Right;
        _Mypair._Myval2._Check_offset(_Off);
        const auto _With_substr = _As_view.substr(_Roff, _Count);
        return _Traits_compare<_Traits>(_Mypair._Myval2._Myptr() + _Off, _Mypair._Myval2._Clamp_suffix_size(_Off, _Nx),
            _With_substr.data(), _With_substr.size());
    }
#endif // _HAS_CXX17

    _NODISCARD _CONSTEXPR20_CONTAINER int compare(const basic_string& _Right) const noexcept {
        // compare [0, size()) with _Right
        return _Traits_compare<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize,
            _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER int compare(size_type _Off, size_type _Nx, const basic_string& _Right) const {
        // compare [_Off, _Off + _Nx) with _Right
        _Mypair._Myval2._Check_offset(_Off);
        return _Traits_compare<_Traits>(_Mypair._Myval2._Myptr() + _Off, _Mypair._Myval2._Clamp_suffix_size(_Off, _Nx),
            _Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER int compare(const size_type _Off, const size_type _Nx, const basic_string& _Right,
        const size_type _Roff, const size_type _Count = npos) const {
        // compare [_Off, _Off + _Nx) with _Right [_Roff, _Roff + _Count)
        _Mypair._Myval2._Check_offset(_Off);
        _Right._Mypair._Myval2._Check_offset(_Roff);
        return _Traits_compare<_Traits>(_Mypair._Myval2._Myptr() + _Off, _Mypair._Myval2._Clamp_suffix_size(_Off, _Nx),
            _Right._Mypair._Myval2._Myptr() + _Roff, _Right._Mypair._Myval2._Clamp_suffix_size(_Roff, _Count));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER int compare(_In_z_ const _Elem* const _Ptr) const noexcept /* strengthened */ {
        // compare [0, size()) with [_Ptr, <null>)
        return _Traits_compare<_Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize, _Ptr, _Traits::length(_Ptr));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER int compare(
        const size_type _Off, const size_type _Nx, _In_z_ const _Elem* const _Ptr) const {
        // compare [_Off, _Off + _Nx) with [_Ptr, <null>)
        _Mypair._Myval2._Check_offset(_Off);
        return _Traits_compare<_Traits>(_Mypair._Myval2._Myptr() + _Off, _Mypair._Myval2._Clamp_suffix_size(_Off, _Nx),
            _Ptr, _Traits::length(_Ptr));
    }

    _NODISCARD _CONSTEXPR20_CONTAINER int compare(const size_type _Off, const size_type _Nx,
        _In_reads_(_Count) const _Elem* const _Ptr, const size_type _Count) const {
        // compare [_Off, _Off + _Nx) with [_Ptr, _Ptr + _Count)
        _Mypair._Myval2._Check_offset(_Off);
        return _Traits_compare<_Traits>(
            _Mypair._Myval2._Myptr() + _Off, _Mypair._Myval2._Clamp_suffix_size(_Off, _Nx), _Ptr, _Count);
    }

#if _HAS_CXX20
    _NODISCARD _CONSTEXPR20_CONTAINER bool starts_with(const basic_string_view<_Elem, _Traits> _Right) const noexcept {
        return basic_string_view<_Elem, _Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize).starts_with(_Right);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER bool starts_with(const _Elem _Right) const noexcept {
        return basic_string_view<_Elem, _Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize).starts_with(_Right);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER bool starts_with(const _Elem* const _Right) const noexcept /* strengthened */ {
        return basic_string_view<_Elem, _Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize).starts_with(_Right);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER bool ends_with(const basic_string_view<_Elem, _Traits> _Right) const noexcept {
        return basic_string_view<_Elem, _Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize).ends_with(_Right);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER bool ends_with(const _Elem _Right) const noexcept {
        return basic_string_view<_Elem, _Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize).ends_with(_Right);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER bool ends_with(const _Elem* const _Right) const noexcept /* strengthened */ {
        return basic_string_view<_Elem, _Traits>(_Mypair._Myval2._Myptr(), _Mypair._Myval2._Mysize).ends_with(_Right);
    }
#endif // _HAS_CXX20

    _NODISCARD _CONSTEXPR20_CONTAINER allocator_type get_allocator() const noexcept {
        return static_cast<allocator_type>(_Getal());
    }

private:
    _NODISCARD static _CONSTEXPR20_CONTAINER size_type _Calculate_growth(
        const size_type _Requested, const size_type _Old, const size_type _Max) noexcept {
        const size_type _Masked = _Requested | _ALLOC_MASK;
        if (_Masked > _Max) { // the mask overflows, settle for max_size()
            return _Max;
        }

        if (_Old > _Max - _Old / 2) { // similarly, geometric overflows
            return _Max;
        }

        return (_STD max)(_Masked, _Old + _Old / 2);
    }

    _NODISCARD _CONSTEXPR20_CONTAINER size_type _Calculate_growth(const size_type _Requested) const noexcept {
        return _Calculate_growth(_Requested, _Mypair._Myval2._Myres, max_size());
    }

    template <class _Fty, class... _ArgTys>
    _CONSTEXPR20_CONTAINER basic_string& _Reallocate_for(const size_type _New_size, _Fty _Fn, _ArgTys... _Args) {
        // reallocate to store exactly _New_size elements, new buffer prepared by
        // _Fn(_New_ptr, _New_size, _Args...)
        if (_New_size > max_size()) {
            _Xlen_string(); // result too long
        }

        const size_type _Old_capacity = _Mypair._Myval2._Myres;
        const size_type _New_capacity = _Calculate_growth(_New_size);
        auto& _Al = _Getal();
        const pointer _New_ptr = _Al.allocate(_New_capacity + 1); // throws

#ifdef __cpp_lib_constexpr_string
        if (_STD is_constant_evaluated()) { // Begin the lifetimes of the objects before copying to avoid UB
            _Traits::assign(_Unfancy(_New_ptr), _New_capacity + 1, _Elem());
        }
#endif // __cpp_lib_constexpr_string
        _Mypair._Myval2._Orphan_all();
        _Mypair._Myval2._Mysize = _New_size;
        _Mypair._Myval2._Myres = _New_capacity;
        _Fn(_Unfancy(_New_ptr), _New_size, _Args...);
        if (_BUF_SIZE <= _Old_capacity) {
            _Al.deallocate(_Mypair._Myval2._Bx._Ptr, _Old_capacity + 1);
            _Mypair._Myval2._Bx._Ptr = _New_ptr;
        }
        else {
            _Construct_in_place(_Mypair._Myval2._Bx._Ptr, _New_ptr);
        }

        return *this;
    }

    template <class _Fty, class... _ArgTys>
    _CONSTEXPR20_CONTAINER basic_string& _Reallocate_grow_by(
        const size_type _Size_increase, _Fty _Fn, _ArgTys... _Args) {
        // reallocate to increase size by _Size_increase elements, new buffer prepared by
        // _Fn(_New_ptr, _Old_ptr, _Old_size, _Args...)
        auto& _My_data = _Mypair._Myval2;
        const size_type _Old_size = _My_data._Mysize;
        if (max_size() - _Old_size < _Size_increase) {
            _Xlen_string(); // result too long
        }

        const size_type _New_size = _Old_size + _Size_increase;
        const size_type _Old_capacity = _My_data._Myres;
        const size_type _New_capacity = _Calculate_growth(_New_size);
        auto& _Al = _Getal();
        const pointer _New_ptr = _Al.allocate(_New_capacity + 1); // throws

#ifdef __cpp_lib_constexpr_string
        if (_STD is_constant_evaluated()) { // Begin the lifetimes of the objects before copying to avoid UB
            _Traits::assign(_Unfancy(_New_ptr), _New_capacity + 1, _Elem());
        }
#endif // __cpp_lib_constexpr_string
        _My_data._Orphan_all();
        _My_data._Mysize = _New_size;
        _My_data._Myres = _New_capacity;
        _Elem* const _Raw_new = _Unfancy(_New_ptr);
        if (_BUF_SIZE <= _Old_capacity) {
            const pointer _Old_ptr = _My_data._Bx._Ptr;
            _Fn(_Raw_new, _Unfancy(_Old_ptr), _Old_size, _Args...);
            _Al.deallocate(_Old_ptr, _Old_capacity + 1);
            _My_data._Bx._Ptr = _New_ptr;
        }
        else {
            _Fn(_Raw_new, _My_data._Bx._Buf, _Old_size, _Args...);
            _Construct_in_place(_My_data._Bx._Ptr, _New_ptr);
        }

        return *this;
    }

    void _Become_small() {
        // release any held storage and return to small string mode
        // pre: *this is in large string mode
        // pre: this is small enough to return to small string mode
        // (not _CONSTEXPR20_CONTAINER; SSO should be disabled in a constexpr context)

        _Mypair._Myval2._Orphan_all();
        const pointer _Ptr = _Mypair._Myval2._Bx._Ptr;
        auto& _Al = _Getal();
        _Destroy_in_place(_Mypair._Myval2._Bx._Ptr);
        _Traits::copy(_Mypair._Myval2._Bx._Buf, _Unfancy(_Ptr), _Mypair._Myval2._Mysize + 1);
        _Al.deallocate(_Ptr, _Mypair._Myval2._Myres + 1);
        _Mypair._Myval2._Myres = _BUF_SIZE - 1;
    }

    _CONSTEXPR20_CONTAINER void _Eos(const size_type _Newsize) { // set new length and null terminator
        _Traits::assign(_Mypair._Myval2._Myptr()[_Mypair._Myval2._Mysize = _Newsize], _Elem());
    }

    _CONSTEXPR20_CONTAINER void _Tidy_init() noexcept { // initialize basic_string data members
        auto& _My_data = _Mypair._Myval2;
        _My_data._Mysize = 0;

#ifdef __cpp_lib_constexpr_string
        if (_STD is_constant_evaluated()) {
            _My_data._Myres = _BUF_SIZE; // SSO disabled in constexpr context
            auto& _Al = _Getal();
            const pointer _New_ptr = _Al.allocate(_BUF_SIZE + 1); // throws
            _My_data._Bx._Ptr = _New_ptr;

            _Elem* const _Raw_new = _Unfancy(_New_ptr);
            _Traits::assign(_Raw_new, _BUF_SIZE + 1, _Elem());
        }
        else
#endif // __cpp_lib_constexpr_string
        {
            _My_data._Myres = _BUF_SIZE - 1;
            // the _Traits::assign is last so the codegen doesn't think the char write can alias this
            _Traits::assign(_My_data._Bx._Buf[0], _Elem());
        }
    }

    _CONSTEXPR20_CONTAINER void _Tidy_deallocate() noexcept { // initialize buffer, deallocating any storage
        _Mypair._Myval2._Orphan_all();
        if (_Mypair._Myval2._Large_string_engaged()) {
            const pointer _Ptr = _Mypair._Myval2._Bx._Ptr;
            auto& _Al = _Getal();
            _Destroy_in_place(_Mypair._Myval2._Bx._Ptr);
            _Al.deallocate(_Ptr, _Mypair._Myval2._Myres + 1);
        }

#ifdef __cpp_lib_constexpr_string
        if (_STD is_constant_evaluated()) {
            _Mypair._Myval2._Bx._Ptr = nullptr;
            _Mypair._Myval2._Mysize = 0;
            _Mypair._Myval2._Myres = 0;
        }
        else
#endif // __cpp_lib_constexpr_string
        {
            _Mypair._Myval2._Mysize = 0;
            _Mypair._Myval2._Myres = _BUF_SIZE - 1;
            // the _Traits::assign is last so the codegen doesn't think the char write can alias this
            _Traits::assign(_Mypair._Myval2._Bx._Buf[0], _Elem());
        }
    }

public:
    _CONSTEXPR20_CONTAINER void _Orphan_all() noexcept { // used by filesystem::path
        _Mypair._Myval2._Orphan_all();
    }

private:
    _CONSTEXPR20_CONTAINER void _Swap_proxy_and_iterators(basic_string& _Right) {
        _Mypair._Myval2._Swap_proxy_and_iterators(_Right._Mypair._Myval2);
    }

    _CONSTEXPR20_CONTAINER _Alty& _Getal() noexcept {
        return _Mypair._Get_first();
    }

    _CONSTEXPR20_CONTAINER const _Alty& _Getal() const noexcept {
        return _Mypair._Get_first();
    }

    _Compressed_pair<_Alty, _Scary_val> _Mypair;
};

#if _HAS_CXX17
template <class _Iter, class _Alloc = allocator<_Iter_value_t<_Iter>>,
    enable_if_t<conjunction_v<_Is_iterator<_Iter>, _Is_allocator<_Alloc>>, int> = 0>
    basic_string(_Iter, _Iter, _Alloc = _Alloc())
    ->basic_string<_Iter_value_t<_Iter>, char_traits<_Iter_value_t<_Iter>>, _Alloc>;

template <class _Elem, class _Traits, class _Alloc = allocator<_Elem>,
    enable_if_t<_Is_allocator<_Alloc>::value, int> = 0>
    explicit basic_string(basic_string_view<_Elem, _Traits>, const _Alloc & = _Alloc())
    ->basic_string<_Elem, _Traits, _Alloc>;

template <class _Elem, class _Traits, class _Alloc = allocator<_Elem>,
    enable_if_t<_Is_allocator<_Alloc>::value, int> = 0>
    basic_string(basic_string_view<_Elem, _Traits>, _Guide_size_type_t<_Alloc>, _Guide_size_type_t<_Alloc>,
        const _Alloc & = _Alloc())->basic_string<_Elem, _Traits, _Alloc>;
#endif // _HAS_CXX17

template <class _Elem, class _Traits, class _Alloc>
_CONSTEXPR20_CONTAINER void swap(basic_string<_Elem, _Traits, _Alloc>& _Left,
    basic_string<_Elem, _Traits, _Alloc>& _Right) noexcept /* strengthened */ {
    _Left.swap(_Right);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) {
    const auto _Left_size = _Left.size();
    const auto _Right_size = _Right.size();
    if (_Left.max_size() - _Left_size < _Right_size) {
        _Xlen_string();
    }

    return { _String_constructor_concat_tag{}, _Left, _Left.c_str(), _Left_size, _Right.c_str(), _Right_size };
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    _In_z_ const _Elem* const _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) {
    using _Size_type = typename basic_string<_Elem, _Traits, _Alloc>::size_type;
    const auto _Left_size = _Convert_size<_Size_type>(_Traits::length(_Left));
    const auto _Right_size = _Right.size();
    if (_Right.max_size() - _Right_size < _Left_size) {
        _Xlen_string();
    }

    return { _String_constructor_concat_tag{}, _Right, _Left, _Left_size, _Right.c_str(), _Right_size };
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    const _Elem _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) {
    const auto _Right_size = _Right.size();
    if (_Right_size == _Right.max_size()) {
        _Xlen_string();
    }

    return { _String_constructor_concat_tag{}, _Right, _STD addressof(_Left), 1, _Right.c_str(), _Right_size };
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, _In_z_ const _Elem* const _Right) {
    using _Size_type = typename basic_string<_Elem, _Traits, _Alloc>::size_type;
    const auto _Left_size = _Left.size();
    const auto _Right_size = _Convert_size<_Size_type>(_Traits::length(_Right));
    if (_Left.max_size() - _Left_size < _Right_size) {
        _Xlen_string();
    }

    return { _String_constructor_concat_tag{}, _Left, _Left.c_str(), _Left_size, _Right, _Right_size };
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, const _Elem _Right) {
    const auto _Left_size = _Left.size();
    if (_Left_size == _Left.max_size()) {
        _Xlen_string();
    }

    return { _String_constructor_concat_tag{}, _Left, _Left.c_str(), _Left_size, _STD addressof(_Right), 1 };
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, basic_string<_Elem, _Traits, _Alloc>&& _Right) {
    return _STD move(_Right.insert(0, _Left));
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    basic_string<_Elem, _Traits, _Alloc>&& _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) {
    return _STD move(_Left.append(_Right));
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    basic_string<_Elem, _Traits, _Alloc>&& _Left, basic_string<_Elem, _Traits, _Alloc>&& _Right) {
#if _ITERATOR_DEBUG_LEVEL == 2
    _STL_VERIFY(_STD addressof(_Left) != _STD addressof(_Right),
        "You cannot concatenate the same moved string to itself. See "
        "N4849 [res.on.arguments]/1.3: If a function argument binds to an rvalue reference "
        "parameter, the implementation may assume that this parameter is a unique reference "
        "to this argument");
#endif // _ITERATOR_DEBUG_LEVEL == 2
    return { _String_constructor_concat_tag{}, _Left, _Right };
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    _In_z_ const _Elem* const _Left, basic_string<_Elem, _Traits, _Alloc>&& _Right) {
    return _STD move(_Right.insert(0, _Left));
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    const _Elem _Left, basic_string<_Elem, _Traits, _Alloc>&& _Right) {
    return _STD move(_Right.insert(0, 1, _Left));
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    basic_string<_Elem, _Traits, _Alloc>&& _Left, _In_z_ const _Elem* const _Right) {
    return _STD move(_Left.append(_Right));
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER basic_string<_Elem, _Traits, _Alloc> operator+(
    basic_string<_Elem, _Traits, _Alloc>&& _Left, const _Elem _Right) {
    _Left.push_back(_Right);
    return _STD move(_Left);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER bool operator==(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) noexcept {
    return _Left._Equal(_Right);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER bool operator==(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, _In_z_ const _Elem* const _Right) {
    return _Left._Equal(_Right);
}

#if _HAS_CXX20
template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER _Get_comparison_category_t<_Traits> operator<=>(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) noexcept {
    return static_cast<_Get_comparison_category_t<_Traits>>(_Left.compare(_Right) <=> 0);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD _CONSTEXPR20_CONTAINER _Get_comparison_category_t<_Traits> operator<=>(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, _In_z_ const _Elem* const _Right) {
    return static_cast<_Get_comparison_category_t<_Traits>>(_Left.compare(_Right) <=> 0);
}
#else // ^^^ _HAS_CXX20 / !_HAS_CXX20 vvv
template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator==(_In_z_ const _Elem* const _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) {
    return _Right._Equal(_Left);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator!=(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) noexcept {
    return !(_Left == _Right);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator!=(_In_z_ const _Elem* const _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) {
    return !(_Left == _Right);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator!=(const basic_string<_Elem, _Traits, _Alloc>& _Left, _In_z_ const _Elem* const _Right) {
    return !(_Left == _Right);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator<(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) noexcept {
    return _Left.compare(_Right) < 0;
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator<(_In_z_ const _Elem* const _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) {
    return _Right.compare(_Left) > 0;
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator<(const basic_string<_Elem, _Traits, _Alloc>& _Left, _In_z_ const _Elem* const _Right) {
    return _Left.compare(_Right) < 0;
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator>(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) noexcept {
    return _Right < _Left;
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator>(_In_z_ const _Elem* const _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) {
    return _Right < _Left;
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator>(const basic_string<_Elem, _Traits, _Alloc>& _Left, _In_z_ const _Elem* const _Right) {
    return _Right < _Left;
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator<=(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) noexcept {
    return !(_Right < _Left);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator<=(_In_z_ const _Elem* const _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) {
    return !(_Right < _Left);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator<=(const basic_string<_Elem, _Traits, _Alloc>& _Left, _In_z_ const _Elem* const _Right) {
    return !(_Right < _Left);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator>=(
    const basic_string<_Elem, _Traits, _Alloc>& _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) noexcept {
    return !(_Left < _Right);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator>=(_In_z_ const _Elem* const _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right) {
    return !(_Left < _Right);
}

template <class _Elem, class _Traits, class _Alloc>
_NODISCARD bool operator>=(const basic_string<_Elem, _Traits, _Alloc>& _Left, _In_z_ const _Elem* const _Right) {
    return !(_Left < _Right);
}
#endif // ^^^ !_HAS_CXX20 ^^^

using string = basic_string<char, char_traits<char>, allocator<char>>;
using wstring = basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t>>;
#ifdef __cpp_lib_char8_t
using u8string = basic_string<char8_t, char_traits<char8_t>, allocator<char8_t>>;
#endif // __cpp_lib_char8_t
using u16string = basic_string<char16_t, char_traits<char16_t>, allocator<char16_t>>;
using u32string = basic_string<char32_t, char_traits<char32_t>, allocator<char32_t>>;

// STRUCT TEMPLATE SPECIALIZATION hash
template <class _Elem, class _Traits, class _Alloc>
struct hash<basic_string<_Elem, _Traits, _Alloc>> {
    _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef basic_string<_Elem, _Traits, _Alloc> _ARGUMENT_TYPE_NAME;
    _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef size_t _RESULT_TYPE_NAME;

    _NODISCARD size_t operator()(const basic_string<_Elem, _Traits, _Alloc>& _Keyval) const noexcept {
        return _Hash_array_representation(_Keyval.c_str(), _Keyval.size());
    }
};

// basic_string INSERTERS AND EXTRACTORS
template <class _Elem, class _Traits, class _Alloc>
basic_istream<_Elem, _Traits>& operator>>(
    basic_istream<_Elem, _Traits>&& _Istr, basic_string<_Elem, _Traits, _Alloc>& _Str) {
    using _Myis = basic_istream<_Elem, _Traits>;
    using _Ctype = typename _Myis::_Ctype;
    using _Mystr = basic_string<_Elem, _Traits, _Alloc>;
    using _Mysizt = typename _Mystr::size_type;

    typename _Myis::iostate _State = _Myis::goodbit;
    bool _Changed = false;
    const typename _Myis::sentry _Ok(_Istr);

    if (_Ok) { // state okay, extract characters
        const _Ctype& _Ctype_fac = _STD use_facet<_Ctype>(_Istr.getloc());
        _Str.erase();

        _TRY_IO_BEGIN
            _Mysizt _Size;
        if (0 < _Istr.width() && static_cast<_Mysizt>(_Istr.width()) < _Str.max_size()) {
            _Size = static_cast<_Mysizt>(_Istr.width());
        }
        else {
            _Size = _Str.max_size();
        }

        typename _Traits::int_type _Meta = _Istr.rdbuf()->sgetc();

        for (; 0 < _Size; --_Size, _Meta = _Istr.rdbuf()->snextc()) {
            if (_Traits::eq_int_type(_Traits::eof(), _Meta)) { // end of file, quit
                _State |= _Myis::eofbit;
                break;
            }
            else if (_Ctype_fac.is(_Ctype::space, _Traits::to_char_type(_Meta))) {
                break; // whitespace, quit
            }
            else { // add character to string
                _Str.push_back(_Traits::to_char_type(_Meta));
                _Changed = true;
            }
        }
        _CATCH_IO_(_Myis, _Istr)
    }

    _Istr.width(0);
    if (!_Changed) {
        _State |= _Myis::failbit;
    }

    _Istr.setstate(_State);
    return _Istr;
}

template <class _Elem, class _Traits, class _Alloc>
basic_istream<_Elem, _Traits>& operator>>(
    basic_istream<_Elem, _Traits>& _Istr, basic_string<_Elem, _Traits, _Alloc>& _Str) {
    return _STD move(_Istr) >> _Str;
}

template <class _Elem, class _Traits, class _Alloc>
basic_ostream<_Elem, _Traits>& operator<<(
    basic_ostream<_Elem, _Traits>& _Ostr, const basic_string<_Elem, _Traits, _Alloc>& _Str) {
    return _Insert_string(_Ostr, _Str.data(), _Str.size());
}

// basic_string LITERALS
inline namespace literals {
    inline namespace string_literals {

#ifdef __EDG__ // TRANSITION, VSO-1273381
#define _CONSTEXPR20_STRING_LITERALS inline
#else // ^^^ workaround / no workaround vvv
#define _CONSTEXPR20_STRING_LITERALS _CONSTEXPR20_CONTAINER
#endif // ^^^ no workaround ^^^

        _NODISCARD _CONSTEXPR20_STRING_LITERALS string operator"" s(const char* _Str, size_t _Len) {
            return string(_Str, _Len);
        }

        _NODISCARD _CONSTEXPR20_STRING_LITERALS wstring operator"" s(const wchar_t* _Str, size_t _Len) {
            return wstring(_Str, _Len);
        }

#ifdef __cpp_char8_t
        _NODISCARD _CONSTEXPR20_STRING_LITERALS basic_string<char8_t> operator"" s(const char8_t* _Str, size_t _Len) {
            return basic_string<char8_t>(_Str, _Len);
        }
#endif // __cpp_char8_t

        _NODISCARD _CONSTEXPR20_STRING_LITERALS u16string operator"" s(const char16_t* _Str, size_t _Len) {
            return u16string(_Str, _Len);
        }

        _NODISCARD _CONSTEXPR20_STRING_LITERALS u32string operator"" s(const char32_t* _Str, size_t _Len) {
            return u32string(_Str, _Len);
        }

#undef _CONSTEXPR20_STRING_LITERALS // TRANSITION, VSO-1273381

    } // namespace string_literals
} // namespace literals

#if _HAS_CXX20
template <class _Elem, class _Traits, class _Alloc, class _Uty>
_CONSTEXPR20_CONTAINER typename basic_string<_Elem, _Traits, _Alloc>::size_type erase(
    basic_string<_Elem, _Traits, _Alloc>& _Cont, const _Uty& _Val) {
    return _Erase_remove(_Cont, _Val);
}

template <class _Elem, class _Traits, class _Alloc, class _Pr>
_CONSTEXPR20_CONTAINER typename basic_string<_Elem, _Traits, _Alloc>::size_type erase_if(
    basic_string<_Elem, _Traits, _Alloc>& _Cont, _Pr _Pred) {
    return _Erase_remove_if(_Cont, _Pass_fn(_Pred));
}
#endif // _HAS_CXX20

#if _HAS_CXX17
namespace pmr {
    template <class _Elem, class _Traits = char_traits<_Elem>>
    using basic_string = _STD basic_string<_Elem, _Traits, polymorphic_allocator<_Elem>>;

    using string = basic_string<char>;
#ifdef __cpp_lib_char8_t
    using u8string = basic_string<char8_t>;
#endif // __cpp_lib_char8_t
    using u16string = basic_string<char16_t>;
    using u32string = basic_string<char32_t>;
    using wstring = basic_string<wchar_t>;
} // namespace pmr
#endif // _HAS_CXX17
_STD_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)
#endif // _STL_COMPILER_PREPROCESSOR
#endif // _XSTRING_
