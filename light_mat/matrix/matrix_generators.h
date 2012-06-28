/*
 * @file matrix_generators.h
 *
 * Basic matrix generators
 *
 * @author Dahua Lin
 */

#ifndef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_MATRIX_GENERATORS_H_
#define LIGHTMAT_MATRIX_GENERATORS_H_

#include <light_mat/matrix/matrix_concepts.h>
#include <light_mat/core/mem_op.h>

namespace lmat
{

	template<typename T>
	class zero_gen : public IMatrixGenerator<zero_gen, T>
	{
	public:
		LMAT_ENSURE_INLINE
		void generate_to(const index_t m, const index_t n,
				const index_t ldim, T *dst) const
		{
			if (n == 1 || ldim == m)
			{
				zero_mem(m * n, dst);
			}
			else
			{
				for (index_t j = 0; j < n; ++j, dst += ldim)
				{
					zero_mem(m, dst);
				}
			}
		}

	}; // end class zero_gen


	template<typename T>
	class fill_gen : public IMatrixGenerator<fill_gen, T>
	{
	public:
		LMAT_ENSURE_INLINE
		fill_gen(const T& v) : m_val(v) { }

		LMAT_ENSURE_INLINE
		void generate_to(const index_t m, const index_t n,
				const index_t ldim, T *dst) const
		{
			if (n == 1 || ldim == m)
			{
				fill_mem(m * n, dst, m_val);
			}
			else
			{
				for (index_t j = 0; j < n; ++j, dst += ldim)
				{
					fill_mem(m, dst, m_val);
				}
			}
		}

	private:
		const T m_val;

	}; // end class fill_gen


	template<typename T>
	class copy_gen : public IMatrixGenerator<copy_gen, T>
	{
	public:
		LMAT_ENSURE_INLINE
		copy_gen(const T *src) : m_src(src) { }

		LMAT_ENSURE_INLINE
		void generate_to(const index_t m, const index_t n,
				const index_t ldim, T *dst) const
		{
			if (n == 1 || m == ldim)
			{
				copy_mem(m * n, m_src, dst);
			}
			else if (m == 1)
			{
				unpack_vec(n, m_src, dst, ldim);
			}
			else
			{
				const T *s = m_src;
				for (index_t j = 0; j < n; ++j, s += m, dst += ldim)
				{
					copy_mem(m, src, dst);
				}
			}
		}

	private:
		const T *m_src;

	}; // end class copy_gen
}


#endif 