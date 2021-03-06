/**
 * @file simd_test_base.h
 *
 * @brief
 *
 * @author Dahua Lin
 */

#ifndef SIMD_TEST_BASE_H_
#define SIMD_TEST_BASE_H_

#include "../test_base.h"
#include <light_mat/simd/simd_debug.h>


namespace lmat { namespace test {

	template<typename T, int I>
	class TI_case : public ltest::test_case
	{
		std::string m_name;

	public:
		TI_case(const char *nam)
		{
			std::stringstream ss;
			ss << nam << " [" << type_name<T>::get() << " (" << I << ")]";
			m_name = ss.str();
		}

		virtual ~TI_case() { }

		const char *name() const
		{
			return m_name.c_str();
		}
	};

	template<typename T, typename Kind>
	inline bool test_simd_ulp(
			const simd_pack<T, Kind>& a, const T *b, unsigned int dtol)
	{
		const unsigned int width = a.width();

		for (unsigned i = 0; i < width; ++i)
		{
			if (ltest::ulp_distance(a[i], b[i]) > dtol)
				return false;
		}

		return true;
	}

} }


#define TI_CASE( Name ) \
	template<typename T, int I> \
	class Name : public lmat::test::TI_case<T, I> { \
	public: \
		Name() : lmat::test::TI_case<T, I>( #Name ) { } \
		virtual ~Name() { } \
		virtual void run(); \
	}; \
	template<typename T, int I> \
	void Name<T, I>::run()

#define ADD_TI_CASE( Name, ty, id ) this->add( new Name<ty, id>() );


#define ASSERT_SIMD_EQ( a, b ) \
	if (!::lmat::test_equal(a, b)) throw ::ltest::assertion_failure(__FILE__, __LINE__, #a " == " #b)

#define ASSERT_SIMD_ULP( a, b, dtol ) \
	if (!::lmat::test::test_simd_ulp(a, b, dtol)) throw ::ltest::assertion_failure(__FILE__, __LINE__, "ULP(" #a ", " #b ") <= " #dtol  )


#endif /* SIMD_TEST_BASE_H_ */
