/**
 * @file avx_math.h
 *
 * @brief Math functions on AVX packs
 *
 * @author Dahua Lin
 */

#ifndef LIGHTMAT_AVX_MATH_H_
#define LIGHTMAT_AVX_MATH_H_

#include <light_mat/math/avx_ops.h>
#include "internal/avx_math_emulate.h"

#if defined(LMAT_USE_INTEL_SVML) && defined(LMAT_USE_AMD_LIBM)
#error SVML and LIBM cannot be used simultaneously.
#endif


#ifdef LMAT_USE_INTEL_SVML

// External function prototypes

#define LMAT_SVML_AVX_F( name ) __svml_##name##f8
#define LMAT_SVML_AVX_D( name ) __svml_##name##4

#define LMAT_DECLARE_SVML_AVX_EXTERN1( name ) \
	__m256  LMAT_SVML_AVX_F(name)( __m256 ); \
	__m256d LMAT_SVML_AVX_D(name)( __m256d );

#define LMAT_DECLARE_SVML_AVX_EXTERN2( name ) \
	__m256  LMAT_SVML_AVX_F(name)( __m256,  __m256  ); \
	__m256d LMAT_SVML_AVX_D(name)( __m256d, __m256d );

#define LMAT_AVX_F( name ) LMAT_SVML_AVX_F( name )
#define LMAT_AVX_D( name ) LMAT_SVML_AVX_D( name )

extern "C"
{
	// C++ 03

#define LMAT_HAS_EXTERN_AVX_POW
#define LMAT_HAS_EXTERN_AVX_EXPLOG
#define LMAT_HAS_EXTERN_AVX_TRI
#define LMAT_HAS_EXTERN_AVX_ARCTRI
#define LMAT_HAS_EXTERN_AVX_HYPERB

	LMAT_DECLARE_SVML_AVX_EXTERN2( pow )
	LMAT_DECLARE_SVML_AVX_EXTERN1( exp )
	LMAT_DECLARE_SVML_AVX_EXTERN1( log )
	LMAT_DECLARE_SVML_AVX_EXTERN1( log10 )

	LMAT_DECLARE_SVML_AVX_EXTERN1( sin )
	LMAT_DECLARE_SVML_AVX_EXTERN1( cos )
	LMAT_DECLARE_SVML_AVX_EXTERN1( tan )

	LMAT_DECLARE_SVML_AVX_EXTERN1( asin )
	LMAT_DECLARE_SVML_AVX_EXTERN1( acos )
	LMAT_DECLARE_SVML_AVX_EXTERN1( atan )
	LMAT_DECLARE_SVML_AVX_EXTERN2( atan2 )

	LMAT_DECLARE_SVML_AVX_EXTERN1( sinh )
	LMAT_DECLARE_SVML_AVX_EXTERN1( cosh )
	LMAT_DECLARE_SVML_AVX_EXTERN1( tanh )

	// C++ 11

#define LMAT_HAS_EXTERN_AVX_HYPOT
#define LMAT_HAS_EXTERN_AVX_CBRT
#define LMAT_HAS_EXTERN_AVX_EXPLOG2
#define LMAT_HAS_EXTERN_AVX_EXPLOG_EX
#define LMAT_HAS_EXTERN_AVX_ARCHYPERB
#define LMAT_HAS_EXTERN_AVX_ERF
// #define LMAT_HAS_EXTERN_AVX_GAMMA

	LMAT_DECLARE_SVML_AVX_EXTERN2( hypot )
	LMAT_DECLARE_SVML_AVX_EXTERN1( cbrt )

	LMAT_DECLARE_SVML_AVX_EXTERN1( exp2 )
	LMAT_DECLARE_SVML_AVX_EXTERN1( log2 )
	LMAT_DECLARE_SVML_AVX_EXTERN1( expm1 )
	LMAT_DECLARE_SVML_AVX_EXTERN1( log1p )

	LMAT_DECLARE_SVML_AVX_EXTERN1( asinh )
	LMAT_DECLARE_SVML_AVX_EXTERN1( acosh )
	LMAT_DECLARE_SVML_AVX_EXTERN1( atanh )

	LMAT_DECLARE_SVML_AVX_EXTERN1( erf )
	LMAT_DECLARE_SVML_AVX_EXTERN1( erfc )

	// other functions

	LMAT_DECLARE_SVML_AVX_EXTERN1( exp10 )
	LMAT_DECLARE_SVML_AVX_EXTERN1( cdfnorm )
	LMAT_DECLARE_SVML_AVX_EXTERN1( cdfnorminv )

}

#endif // has intel SVML

#define LMAT_ACTIVATE_AVX_EXTERN_1( Name ) \
	LMAT_ENSURE_INLINE \
	inline avx_f32pk Name( const avx_f32pk& a ) { \
		return LMAT_AVX_F(Name)(a); } \
	LMAT_ENSURE_INLINE \
	inline avx_f64pk Name( const avx_f64pk& a ) { \
		return LMAT_AVX_D(Name)(a); } \
	struct has_avx_##Name { static const bool value = true; };

#define LMAT_ACTIVATE_AVX_EXTERN_2( Name ) \
	LMAT_ENSURE_INLINE \
	inline avx_f32pk Name( const avx_f32pk& a, const avx_f32pk& b ) { \
		return LMAT_AVX_F(Name)(a, b); } \
	LMAT_ENSURE_INLINE \
	inline avx_f64pk Name( const avx_f64pk& a, const avx_f64pk& b ) { \
		return LMAT_AVX_D(Name)(a, b); } \
	struct has_avx_##Name { static const bool value = true; };


namespace lmat { namespace math {

	// C++ 03

#ifdef LMAT_HAS_EXTERN_AVX_POW
	LMAT_ACTIVATE_AVX_EXTERN_2( pow )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_2( pow )
#endif


#ifdef LMAT_HAS_EXTERN_AVX_EXPLOG
	LMAT_ACTIVATE_AVX_EXTERN_1( exp )
	LMAT_ACTIVATE_AVX_EXTERN_1( log )
	LMAT_ACTIVATE_AVX_EXTERN_1( log10 )

	LMAT_ENSURE_INLINE
	inline avx_f32pk xlogy(const avx_f32pk& a, const avx_f32pk& b)
	{
		avx_f32pk z = avx_f32pk::zeros();
		return cond(a > z, log(b), z) * a;
	}

	LMAT_ENSURE_INLINE
	inline avx_f64pk xlogy(const avx_f64pk& a, const avx_f64pk& b)
	{
		avx_f64pk z = avx_f64pk::zeros();
		return cond(a > z, log(b), z) * a;
	}

	struct has_avx_xlogy { static const bool value = true; };

#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( exp )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( log )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( log10 )
#endif


#ifdef LMAT_HAS_EXTERN_AVX_TRI
	LMAT_ACTIVATE_AVX_EXTERN_1( sin )
	LMAT_ACTIVATE_AVX_EXTERN_1( cos )
	LMAT_ACTIVATE_AVX_EXTERN_1( tan )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( sin )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( cos )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( tan )
#endif


#ifdef LMAT_HAS_EXTERN_AVX_ARCTRI
	LMAT_ACTIVATE_AVX_EXTERN_1( asin )
	LMAT_ACTIVATE_AVX_EXTERN_1( acos )
	LMAT_ACTIVATE_AVX_EXTERN_1( atan )
	LMAT_ACTIVATE_AVX_EXTERN_2( atan2 )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( asin )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( acos )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( atan )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_2( atan2 )
#endif


#ifdef LMAT_HAS_EXTERN_AVX_HYPERB
	LMAT_ACTIVATE_AVX_EXTERN_1( sinh )
	LMAT_ACTIVATE_AVX_EXTERN_1( cosh )
	LMAT_ACTIVATE_AVX_EXTERN_1( tanh )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( sinh )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( cosh )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( tanh )
#endif

	// C++ 11

#ifdef LMAT_HAS_CXX11_MATH

#ifdef LMAT_HAS_EXTERN_AVX_HYPOT
	LMAT_ACTIVATE_AVX_EXTERN_2( hypot )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_2( hypot )
#endif


#ifdef LMAT_HAS_EXTERN_AVX_CBRT
	LMAT_ACTIVATE_AVX_EXTERN_1( cbrt )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( cbrt )
#endif


#ifdef LMAT_HAS_EXTERN_AVX_EXPLOG2
	LMAT_ACTIVATE_AVX_EXTERN_1( exp2 )
	LMAT_ACTIVATE_AVX_EXTERN_1( log2 )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( exp2 )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( log2 )
#endif


#ifdef LMAT_HAS_EXTERN_AVX_EXPLOG_EX
	LMAT_ACTIVATE_AVX_EXTERN_1( expm1 )
	LMAT_ACTIVATE_AVX_EXTERN_1( log1p )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( expm1 )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( log1p )
#endif


#ifdef LMAT_HAS_EXTERN_AVX_ARCHYPERB
	LMAT_ACTIVATE_AVX_EXTERN_1( asinh )
	LMAT_ACTIVATE_AVX_EXTERN_1( acosh )
	LMAT_ACTIVATE_AVX_EXTERN_1( atanh )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( asinh )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( acosh )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( atanh )
#endif


#ifdef LMAT_HAS_EXTERN_AVX_ERF
	LMAT_ACTIVATE_AVX_EXTERN_1( erf )
	LMAT_ACTIVATE_AVX_EXTERN_1( erfc )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( erf )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( erfc )
#endif

#ifdef LMAT_HAS_EXTERN_AVX_GAMMA
	LMAT_ACTIVATE_AVX_EXTERN_1( lgamma )
	LMAT_ACTIVATE_AVX_EXTERN_1( tgamma )
#elif (defined(LMAT_ENABLE_SIMD_EMULATE))
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( lgamma )
	LMAT_ACTIVATE_AVX_MATH_EMULATE_1( tgamma )
#endif

#endif // has C++11 math

} }


#endif /* AVX_MATH_H_ */
