#ifndef _LABVVIS_FUNCTOR_H
#define _LABVVIS_FUNCTOR_H

#include <limits>
#include <map>
#include <cmath>
#include <functional>

#include <labvvis/kernel.h>
#include <ct/metafunction.h>

#define VVM_TRUE ~0
#define VVM_FALSE 0
	
namespace labvvis
{
	/** Binary functor
	 * Wrapper for Standard C++ functors.
	 */
	template<template<typename> class functorT>
	struct binary_functor {
		template<typename T> typename functorT<T>::result_type
		operator()(const typename functorT<T>::first_argument_type& t1,
		const typename functorT<T>::second_argument_type& t2) const {
			functorT<T> f;
			return f(t1, t2);
		}
	};

	
	//==========================================================================
	// Standard C++ functors
	//--------------------------------------------------------------------------
	//- plus -------------------------------------------------------------------
	template<typename T = void> struct plus
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a + b;
		}
	};
	template<> struct plus<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a + b;
		}
	};
	//- minus ------------------------------------------------------------------
	template<typename T = void> struct minus
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a - b;
		}
	};
	template<> struct minus<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a - b;
		}
	};
	//- multiplies -------------------------------------------------------------
	template<typename T = void> struct multiplies
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a * b;
		}
	};
	template<> struct multiplies<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a * b;
		}
	};
	//- divides ----------------------------------------------------------------
	template<typename T = void> struct divides
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a / b;
		}
	};
	template<> struct divides<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a / b;
		}
	};
	//- modulus ----------------------------------------------------------------
	template<typename T = void> struct modulus
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a % b;
		}
	};
	template<> struct modulus<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a % b;
		}
	};
	//- negate -----------------------------------------------------------------
	template<typename T = void> struct negate
	: public std::unary_function<T, T> {
		T operator()(const T& a) const {
			return -a;
		}
	};
	template<> struct negate<void> {
		template<typename T> T operator()(const T& a) const {
			return -a;
		}
	};
	
	//- equal_to ---------------------------------------------------------------
	template<typename T = void> struct equal_to
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a == b ? VVM_TRUE : VVM_FALSE;
		}
	};
    template<> struct equal_to<void> {
		template<typename T>
		T operator()(const T& a, const T& b) const {
			return a == b ? VVM_TRUE : VVM_FALSE;
		}
	};
	//- not_equal_to -----------------------------------------------------------
	template<typename T = void> struct not_equal_to
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a != b ? VVM_TRUE : VVM_FALSE;
		}
	};
    template<> struct not_equal_to<void> {
		template<typename T>
		T operator()(const T& a, const T& b) const {
			return a != b ? VVM_TRUE : VVM_FALSE;
		}
	};
	//- greater ----------------------------------------------------------------
	template<typename T = void> struct greater
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a > b ? VVM_TRUE : VVM_FALSE;
		}
	};
    template<> struct greater<void> {
		template<typename T>
		T operator()(const T& a, const T& b) const {
			return a > b ? VVM_TRUE : VVM_FALSE;
		}
	};
	//- less -------------------------------------------------------------------
	template<typename T = void> struct less
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a < b ? VVM_TRUE : VVM_FALSE;
		}
	};
    template<> struct less<void> {
		template<typename T>
		T operator()(const T& a, const T& b) const {
			return a < b ? VVM_TRUE : VVM_FALSE;
		}
	};
	//- greater_equal ----------------------------------------------------------
	template<typename T = void> struct greater_equal
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a >= b ? VVM_TRUE : VVM_FALSE;
		}
	};
    template<> struct greater_equal<void> {
		template<typename T>
		T operator()(const T& a, const T& b) const {
			return a >=  b ? VVM_TRUE : VVM_FALSE;
		}
	};
	//- less_equal -------------------------------------------------------------
    template<typename T = void> struct less_equal
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a <= b ? VVM_TRUE : VVM_FALSE;
		}
	};
    template<> struct less_equal<void> {
		template<typename T>
		T operator()(const T& a, const T& b) const {
			return a <=  b ? VVM_TRUE : VVM_FALSE;
		}
	};
	
	//- logical_and ------------------------------------------------------------
    template<typename T = void> struct logical_and
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a && b ? VVM_TRUE : VVM_FALSE;
		}
	};
    template<> struct logical_and<void> {
		template<typename T>
		T operator()(const T& a, const T& b) const {
			return a &&  b ? VVM_TRUE : VVM_FALSE;
        }
    };
    //- logical_or -------------------------------------------------------------
    template<typename T = void> struct logical_or
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a || b ? VVM_TRUE : VVM_FALSE;
		}
	};
    template<> struct logical_or<void> {
		template<typename T>
		T operator()(const T& a, const T& b) const {
			return a ||  b ? VVM_TRUE : VVM_FALSE;
        }
    };
	//- logical_not ------------------------------------------------------------
	template<typename T = void> struct logical_not
	: public std::unary_function<T, T> {
		T operator()(const T& a) const {
			return !a ? VVM_TRUE : VVM_FALSE;
		}
	};
	template<> struct logical_not<void> {
		template<typename T>
		T operator()(const T& a) const {
			return !a ? VVM_TRUE : VVM_FALSE;
		}
	};
	
    //==========================================================================
	// Additional functors
	//--------------------------------------------------------------------------
	//- plus_saturated ---------------------------------------------------------
	template<typename T = void> struct plus_saturated
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			typename ct::promote<T>::type r = a + b;
			return r < std::numeric_limits<T>::max() ?
				static_cast<T>(r) : std::numeric_limits<T>::max();
		}
	};
	template<> struct plus_saturated<void> {
		/// Scalar implementation
		template<typename T> T operator()(const T& a, const T& b) const {
			typedef typename ct::promote<T>::type promote_t;
			promote_t r = a + b;
			return r < std::numeric_limits<T>::max() ?
				static_cast<T>(r) : std::numeric_limits<T>::max();
		}
	};
	//- minus_saturated --------------------------------------------------------
	template<typename T = void> struct minus_saturated
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			typename ct::promote_signed<T>::type r = a - b;
			return r > std::numeric_limits<T>::min() ?
				r : std::numeric_limits<T>::min();
		}
	};
	template<> struct minus_saturated<void> {
		/// Scalar implementation
		template<typename T> T operator()(const T& a, const T& b) const {
			typename ct::promote_signed<T>::type r = a - b;
			return r > std::numeric_limits<T>::min() ?
				r : std::numeric_limits<T>::min();
		}
	};
	//- bitwise_and ------------------------------------------------------------
	template<typename T = void> struct bitwise_and
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a & b;
		}
	};
	template<> struct bitwise_and<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a & b;
		}
	};
	//- bitwise_not ------------------------------------------------------------
	template<typename T = void> struct bitwise_not
	: public std::unary_function<T, T> {
		T operator()(const T& a) const {
			return ~a;
		}
	};
	template<> struct bitwise_not<void> {
		template<typename T> T operator()(const T& a) const {
			return ~a;
		}
	};
	//- bitwise_or -------------------------------------------------------------
	template<typename T = void> struct bitwise_or
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a | b;
		}
	};
	template<> struct bitwise_or<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a | b;
		}
	};
	//- constant ---------------------------------------------------------------
	template<typename T> struct constant {
		constant(const T a) : _scalar_constant(a) {
		}
		T operator()(const T&) const {
			return _scalar_constant;
		}
	private:
		T _scalar_constant;
	};
	//- noop -------------------------------------------------------------------
	template<typename T = void> struct noop
		: public std::unary_function<T, T> {
			T operator()(const T& a) const {
				return a;
			}
		};
	template<> struct noop<void> {
		template<typename T> T operator()(const T& a) const {
			return a;
		}
	};	
	//- min_max ----------------------------------------------------------------
	template<typename T> class min_max {
	public:
		min_max() {
			_min = std::numeric_limits<T>::max();
			_max = std::numeric_limits<T>::min();
		}
		T operator()(const T& s) {
			if(s < _min)
				_min = s;
			else if(s > _max)
				_max = s;
			return s;
		}
		const T min() {
			return _min;
		}
		const T max() {
			return _max;
		}
	private:
		T _min, _max;
	};
	
	//= Discrete ===============================================================
	//- equalize ---------------------------------------------------------------
	/** Equalize
	 */
	template<typename T> struct equalize {
		equalize(const T min, const T max)
		: _smin(min), _smax(max) {
			// Calculate m & c
			_sm = (std::numeric_limits<T>::max() - 1) / (F)(_smax - _smin);
			_sc = 1 - _sm * _smin;
		}
		T operator()(const T a) {
			if(a < _smin)
				return 0;
			else if(a > _smax)
				return std::numeric_limits<T>::max();
			else
				return _sm * a + _sc;
		}
	private:
		T _smin, _smax;
		typedef typename ct::promote_float<T>::type F;
		F _sm, _sc;

	};
	//- logarithmic ------------------------------------------------------------
	/** Natural Logarithm
	 */
	template<typename T = void> struct logarithmic {
	private:
		typedef typename ct::promote_float<T>::type promote_type;
	public:
		logarithmic() {
		}
		/// Scalar implementation
		promote_type operator()(const T a) const {
			promote_type gx = (a / static_cast<promote_type>(std::numeric_limits<T>::max())) + 1;
			return std::log(static_cast<promote_type>(gx)) * std::numeric_limits<T>::max();
		}
	};
	//- exponential ------------------------------------------------------------
	/** Exponential
	 */
	template<typename T = void> struct exponential {
		exponential() {
		}
		T operator()(const T a) {
			T gx = a / std::numeric_limits<T>::max();
			return std::exp(static_cast<typename ct::promote_float<T>::type>(gx)) * std::numeric_limits<T>::max();
		}
	};
    
	//- histogram --------------------------------------------------------------
	template<typename T, typename mapT = std::map<T, int> > struct histogram {
		typedef const mapT result_type;
		histogram() {
			_pcounts = new mapT();
			_delete_pcount = true;
		}
		histogram(mapT &counts)
		: _pcounts(&counts), _delete_pcount(false) {
		}
		~histogram() {
			if(_delete_pcount) {
				delete _pcounts;
			}
		}
		void operator()(const T p) {
			++((*_pcounts)[p]);
		}
		result_type &result() const {
			return *_pcounts;
		}
	private:
		bool _delete_pcount;
		mapT *_pcounts;
	};
	//- count_if ---------------------------------------------------------------
	template<typename T, typename opT>
	struct count_if {
		count_if(const opT& op, const T& reference)
		: _op(op), _count(0), _reference(reference) {
		}
		T operator()(const T a) {
			if(_op(a, _reference)) {
				++_count;
			}
		}
		void reset() {
			_count = 0;
		}
		int count() const {
			return _count;
		}
	private:
		int _count;
		opT _op;
		const T& _reference;
	};

	//==========================================================================
	// Convolution functors
	//--------------------------------------------------------------------------
	//- base_filter ------------------------------------------------------------
	/** Base Filter
	 */
	template<
		typename kernelT,
		typename T,
		template<typename> class promoteP = ct::promote
	> class base_filter {
	protected:
		typedef typename promoteP<CT_TYPELIST2(kernelT, T)>::type scalar_type;
	public:
		typedef kernel2d<kernelT> kernel_type;
		base_filter() {
		}
		base_filter(const base_filter& rhs)
		: _kernel(rhs._kernel) {
			allocate();
		}
		base_filter(kernel_type kernel)
		: _kernel(kernel) {
			allocate();
		}
		base_filter& operator=(const base_filter& rhs) {
			// Check if the same object
			if(&rhs == this)
				return *this;
			// Deallocate memory
			deallocate();
			// Then make copy
			_kernel = rhs._kernel;
			allocate();
			return *this;
		}
		const kernel_type kernel() const {
			return _kernel;
		}
	protected:
		// Destructor is protected so that derived classes do not have to be
		// virtual. In addition, it prevent base_filter from being used by
		// the user which is good.
		~base_filter() {
			deallocate();
		}
	private:
		void allocate() {
		}
		void deallocate() {
		}
	protected:
		kernel_type _kernel;
	};

	//- linear_filter ----------------------------------------------------------
	/** Linear Filter
	 * Scalar kernel not implemented because can get value from vector kernel
	 * -> Allows object to be smaller
	 * -> Scalar code would not be used much anyway
	 */
	template<
		typename kernelT,
		typename T,
		template<typename> class promoteP = ct::promote
	> class linear_filter : public base_filter<kernelT, T, promoteP> {
	private:
		typedef base_filter<kernelT, T, promoteP> parent_type;
		typedef typename parent_type::scalar_type scalar_type;
	public:
		typedef typename parent_type::kernel_type kernel_type;
		linear_filter() : parent_type() {
		}
		linear_filter(const linear_filter& rhs)
		: parent_type(rhs), _scalar_result(rhs._scalar_result),
		_i(rhs._i) {
		}
		linear_filter(kernel_type kernel)
		: parent_type(kernel) {
		}
		~linear_filter() {
		}
	public:
		void reset() {
			_i = 0;
			_scalar_result = 0;
		}
		const scalar_type scalar_result() const {
			// TODO: Restrict result to range
			return _scalar_result / this->_kernel.sum();
		}
		void accumulate(const T& v) {
			_scalar_result += v * this->_kernel.begin()[_i];
			++_i;
		}
	private:
		scalar_type _scalar_result;
		int _i;
	};

	//- max_filter ----------------------------------------------------------
	/** Filter that returns the largest value
	 * Scalar kernel not implemented because can get value from vector kernel
	 * -> Allows object to be smaller
	 * -> Scalar code would not be used much anyway
	 */ 
	template<
		typename kernelT,
		typename T,
		template<typename> class promoteP = ct::promote
	> class max_filter : public base_filter<kernelT, T, promoteP> {
	private:
		typedef base_filter<kernelT, T, promoteP> parent_type;
		typedef typename parent_type::scalar_type scalar_type;
	public:
		typedef typename parent_type::kernel_type kernel_type;
		max_filter() : parent_type() {
		}
		max_filter(const max_filter& rhs)
		: parent_type(rhs), _scalar_result(rhs._scalar_result),
        _i(rhs._i) {
		}
		max_filter(kernel_type kernel)
		: parent_type(kernel) {
		}
		~max_filter() {
		}
	public:
		void reset() {
			_i = 0;
			_scalar_result = 0;
		}
		const scalar_type scalar_result() const {
			return _scalar_result;
		}
		void accumulate(const T& v) {
			scalar_type val = v * this->_kernel.begin()[_i];
			if(val > _scalar_result)
				_scalar_result = val;
			++_i;
		}
	private:
		scalar_type _scalar_result;
		int _i;
	};
    
	//==========================================================================
	// Hough Transform
	//--------------------------------------------------------------------------
	struct hough_line {
		hough_line(double ar, int aq, int acount)
		: r(ar), q(aq), count(acount) {
		}
		double r;
		int q;
		int count;
	};

	template<typename T> class hough {
		typedef std::map<int, int> rq_t;
	public:
		typedef hough_line value_type;
	public:
		hough() {
		}
		~hough() {
		}
		
		void operator()(const T value, const int x, const int y) {
			if(value) {
				for(int q = 0; q < 180; q += 1) {
					double rad = q*M_PI/180.0;
					// Keep up to 2 decimals
					int r = round((x*cos(rad) + y*sin(rad)) * 100);
					++_accumulator[q][r];
				}
			}
		}

		// TODO: Hough Transform is not vectorised
		// void operator()(const vector_type& v);
		
		std::vector<value_type> result(const int threshold) {
			std::vector<value_type > ret;
			for(int q = 0; q < 180; ++q) {
				rq_t &rq = _accumulator[q];
				for(rq_t::const_iterator i = rq.begin(); i != rq.end(); ++i) {
					if(i->second >= threshold) {
						std::clog << "[vvis] Line r=" << (i->first / 100.0) << ", q=" << q << std::endl; 
						ret.push_back(value_type(i->first / 100.0, q, i->second));
					}
				}
			}
			return ret;
		}
	private:
		rq_t _accumulator[180];
	};
} // End of vvis namespace

#endif
