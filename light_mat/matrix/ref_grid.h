/**
 * @file ref_grid.h
 *
 * @brief cref_grid and ref_grid classes
 *
 * @author Dahua Lin
 */

#ifndef LIGHTMAT_REF_GRID_H_
#define LIGHTMAT_REF_GRID_H_

#include <light_mat/matrix/dense_mat_base.h>

namespace lmat
{

	template<typename T, int CTRows, int CTCols>
	struct matrix_traits<cref_grid<T, CTRows, CTCols> >
	{
		static const int num_dimensions = 2;
		static const int ct_num_rows = CTRows;
		static const int ct_num_cols = CTCols;

		static const bool is_readonly = true;

		typedef T value_type;
		typedef grid_layout_cm<CTRows, CTCols> layout_type;
		typedef cpu_domain domain;
	};


	template<typename T, int CTRows, int CTCols>
	class cref_grid : public dense_mat_base<cref_grid<T, CTRows, CTCols>, T>
	{
#ifdef LMAT_USE_STATIC_ASSERT
		static_assert(is_supported_matrix_value_type<T>::value,
				"T must be a supported matrix value type");
#endif

	public:
		LMAT_MAT_TRAITS_CDEFS(T)
		typedef grid_layout_cm<CTRows, CTCols> layout_type;

	public:

		LMAT_ENSURE_INLINE
		cref_grid(const T* pdata, index_t m, index_t n, index_t rs, index_t cs)
		: m_data(pdata), m_layout(m, n, rs, cs)
		{
		}

	private:
		cref_grid& operator = (const cref_grid& );  // no assignment

	public:
		LMAT_ENSURE_INLINE const layout_type& layout() const
		{
			return m_layout;
		}

		LMAT_ENSURE_INLINE const_pointer ptr_data() const
		{
			return m_data;
		}

	private:
		const T *m_data;
		layout_type m_layout;

	}; // end class cref_grid


	template<typename T, int CTRows, int CTCols>
	struct matrix_traits<ref_grid<T, CTRows, CTCols> >
	{
		static const int num_dimensions = 2;
		static const int ct_num_rows = CTRows;
		static const int ct_num_cols = CTCols;

		static const bool is_readonly = false;

		typedef T value_type;
		typedef grid_layout_cm<CTRows, CTCols> layout_type;
		typedef cpu_domain domain;
	};


	template<typename T, int CTRows, int CTCols>
	class ref_grid : public dense_mat_base<ref_grid<T, CTRows, CTCols>, T>
	{
#ifdef LMAT_USE_STATIC_ASSERT
		static_assert(is_supported_matrix_value_type<T>::value,
				"T must be a supported matrix value type");
#endif

	public:
		LMAT_MAT_TRAITS_DEFS(T)
		typedef grid_layout_cm<CTRows, CTCols> layout_type;

	public:
		LMAT_ENSURE_INLINE
		ref_grid(T* pdata, index_t m, index_t n, index_t rs, index_t cs)
		: m_data(pdata), m_layout(m, n, rs, cs)
		{
		}

	public:
		LMAT_ENSURE_INLINE ref_grid& operator = (const ref_grid& r)
		{
			if (this != &r)
			{
				copy(r, *this);
			}
			return *this;
		}

		template<class Expr>
		LMAT_ENSURE_INLINE ref_grid& operator = (const IMatrixXpr<Expr, T>& r)
		{
			assign(r);
			return *this;
		}

	public:
		LMAT_ENSURE_INLINE const layout_type& layout() const
		{
			return m_layout;
		}

		LMAT_ENSURE_INLINE const_pointer ptr_data() const
		{
			return m_data;
		}

		LMAT_ENSURE_INLINE pointer ptr_data()
		{
			return m_data;
		}

	private:
		template<class Expr>
		LMAT_ENSURE_INLINE
		void assign(const IMatrixXpr<Expr, T>& r)
		{
			LMAT_CHECK_SAME_SHAPE(*this, r);
			default_evaluate(r, *this);
		}

	private:
		T *m_data;
		layout_type m_layout;

	}; // end ref_grid

}

#endif /* REF_GRID_H_ */