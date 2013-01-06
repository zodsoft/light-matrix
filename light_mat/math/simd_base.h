/**
 * @file simd_base.h
 *
 * @brief The basis for SIMD
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_SIMD_BASE_H_
#define LIGHTMAT_SIMD_BASE_H_

#include <light_mat/math/simd_arch.h>
#include <light_mat/common/basic_defs.h>
#include <light_mat/math/fun_tags.h>
#include <limits>

// system headers for SIMD intrinsics

#if (defined(LMAT_HAS_AVX2))
#ifdef __GNUC__
#include <x86intrin.h>
#else
#include <immintrin.h>
#endif // __GNUC__
#elif (defined(LMAT_HAS_AVX))
#include <immintrin.h> 	// AVX
#elif (defined(LMAT_HAS_SSE4_2))
#include <nmmintrin.h> 	// SSE4.2
#elif (defined(LMAT_HAS_SSE4_1))
#include <smmintrin.h> 	// SSE4.1
#elif (defined(LMAT_HAS_SSSE3))
#include <tmmintrin.h>	// SSSE3
#elif (defined(LMAT_HAS_SSE3))
#include <pmmintrin.h> 	// SSE3
#elif (defined(LMAT_HAS_SSE2))
#include <emmintrin.h> 	// SSE2
#elif (defined(LMAT_HAS_SSE))
#include <xmmintrin.h>	// SSE
#endif


#define LMAT_SIMD_PACK_(T, K) simd_pack<T, K>
#define LMAT_SIMD_PACK_M(T, K) math::simd_pack<T, K>

namespace lmat {

	struct sse_t { };
	struct avx_t { };

	namespace meta
	{
		template<typename A>
		struct is_simd_kind
		{
			static const bool value = false;
		};

		template<> struct is_simd_kind<sse_t>
		{
			static const bool value = true;
		};

		template<> struct is_simd_kind<avx_t>
		{
			static const bool value = true;
		};

		template<typename FTag, typename T, typename Kind>
		struct has_simd_support
		{
			static const bool value = false;
		};
	}


#if (defined(LMAT_HAS_AVX))
	typedef avx_t default_simd_kind;
#else
	typedef sse_t default_simd_kind;
#endif


namespace math {

	// SIMD kind

	using lmat::sse_t;
	using lmat::avx_t;

	using lmat::default_simd_kind;

	// forward declaration of classes


	template<typename T, typename Kind> struct simd_traits;

	template<typename T, typename Kind> class simd_bpack;

	template<typename T, typename Kind> class simd_pack;

} }


// Useful macros

#define LMAT_ALIGN_SSE LMAT_ALIGN(16)
#define LMAT_ALIGN_AVX LMAT_ALIGN(32)

#define LMAT_DEFINE_SIMD_TRAITS( Kind, ScalarT, Wid, Bytes ) \
	template<> struct simd_traits<ScalarT, Kind> { \
		typedef ScalarT scalar_type; \
		static const unsigned int scalar_bytes = sizeof(scalar_type); \
		static const unsigned int pack_width = Wid; \
		static const unsigned int pack_bytes = Bytes; \
		static const unsigned int pack_nbits = Bytes * 8; \
	};


#define LMAT_DEFINE_FOR_SIMD_PACK( Kind, ScalarT, Wid ) \
		typedef Kind simd_kind; \
		typedef ScalarT scalar_type; \
		static const unsigned int scalar_bytes = sizeof(scalar_type); \
		static const unsigned int pack_width = Wid;


#endif /* SIMD_BASE_H_ */
