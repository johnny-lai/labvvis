/*
 *  tuple.h
 *  ct
 *
 *  Created by Bing-Chang Lai on Sat Jul 12 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _CT_TUPLE_H
#define _CT_TUPLE_H

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/call_traits.hpp>
#include <ct/meta_meta.h>
#include <ct/boost_type_traits.h>
#include <ct/typelist.h>

namespace ct {
	template<typename typelistT> struct tuple;
	
	//===============================================================================
	// Metafunctions to make dealing with tuples easier
	//-------------------------------------------------------------------------------
	template<typename T> struct remove_reference {
		typedef typename boost::remove_reference<T>::type type;
	};
	template<typename typelistT> struct remove_reference<tuple<typelistT> > {
		typedef tuple<typename ct::apply<typelistT, boost::remove_reference>::type> type;
	};
	template<typename T> struct remove_pointer {
		typedef typename boost::remove_pointer<T>::type type;
	};
	template<typename typelistT> struct remove_pointer<tuple<typelistT> > {
		typedef tuple<typename ct::apply<typelistT, boost::remove_pointer>::type> type;
	};
	
	template<typename tupleT> struct dereference_tuple {
	};
	template<typename TL> struct dereference_tuple<tuple<TL> > {
		typedef tuple<
			typename apply<
				typename apply<TL, boost::remove_pointer>::type,
				boost::add_reference>::type> type;
	};

	namespace priv {
		//- deref --------------------------------------------------------------
		template<int count, typename derefT, typename originalT> struct deref;
		template<typename derefT, typename originalT>
		struct deref<1, derefT, originalT> {
			static derefT exec(const originalT& o) {
				return derefT(*o.template get<0>());
			}
		};
		template<typename derefT, typename originalT>
		struct deref<2, derefT, originalT> {
			static derefT exec(const originalT& o) {
				return derefT(*o.template get<0>(), *o.template get<1>());
			}
		};
		template<typename derefT, typename originalT>
		struct deref<3, derefT, originalT> {
			static derefT exec(const originalT& o) {
				return derefT(*o.template get<0>(), *o.template get<1>(), *o.template get<2>());
			}
		};
		template<typename derefT, typename originalT>
		struct deref<4, derefT, originalT> {
			static derefT exec(const originalT& o) {
				return derefT(*o.template get<0>(), *o.template get<1>(), *o.template get<2>(),
					*o.template get<3>());
			}
		};
		template<typename derefT, typename originalT>
		struct deref<5, derefT, originalT> {
			static derefT exec(const originalT& o) {
				return derefT(*o.template get<0>(), *o.template get<1>(), *o.template get<2>(),
					*o.template get<3>(), *o.template get<4>());
			}
		};
		//- assign -------------------------------------------------------------
		struct assign {
			template<int i> struct Code {
				template<typename T, typename U>
				static void exec(T& self, const U& rhs) {
					self.template get<i>() = rhs.template get<i>();
				}
			};
		};
		//- assign_value -------------------------------------------------------
		struct assign_value {
			template<int i> struct Code {
				template<typename T, typename U>
				static void exec(T& self, const U& v) {
					self.template get<i>() = v;
				}
			};
		};
		//- tuple_impl ---------------------------------------------------------
		template<typename tupleT> class tuple_impl;
		template<typename typelistT> class tuple_impl<tuple<typelistT> > {
		public:
			typedef typelistT type_tl;
		protected:
			typedef tuple<typelistT> tuple_type;
			typedef typename dereference_tuple<tuple<typelistT> >::type tuple_deref_type;
			typedef tuple<
				typename apply<typelistT,
					boost::remove_reference>::type> tuple_ret_type;
			enum { count = length<typelistT>::value };
		protected:
			//- pre_inc -------------------------------------------------------------
			struct pre_inc {
				template<int i> struct Code {
					static void exec(tuple_type& self) {
						++self.template get<i>();
					}
				};
			};
			//- pre_dec -------------------------------------------------------------
			struct pre_dec {
				template<int i> struct Code {
					static void exec(tuple_type& self) {
						--self.template get<i>();
					}
				};
			};	
			//- add -----------------------------------------------------------------
			struct add {
				template<int i> struct Code {
					static void exec(tuple_ret_type& ret, const tuple_type& a, const tuple_type& b) {
						ret.template get<i>() = a.template get<i>() + b.template get<i>();
					}
				};
			};
			//- add_const  ----------------------------------------------------------
			struct add_const {
				template<int i> struct Code {
					template<typename T>
					static void exec(tuple_ret_type& ret, const tuple_type& a, const T b) {
						ret.template get<i>() = a.template get<i>() + b;
					}
				};
			};
			//- sub -----------------------------------------------------------------
			struct sub {
				template<int i> struct Code {
					static void exec(tuple_ret_type& ret, const tuple_type& a, const tuple_type& b) {
						ret.template get<i>() = a.template get<i>() - b.template get<i>();
					}
				};
			};
			//- sub_const  ----------------------------------------------------------
			struct sub_const {
				template<int i> struct Code {
					template<typename T>
					static void exec(tuple_ret_type& ret, const tuple_type& a, const T b) {
						ret.template get<i>() = a.template get<i>() - b;
					}
				};
			};
			//- mul -----------------------------------------------------------------
			struct mul {
				template<int i> struct Code {
					static void exec(tuple_ret_type& ret, const tuple_type& a, const tuple_type& b) {
						ret.template get<i>() = a.template get<i>() * b.template get<i>();
					}
				};
			};
			//- mul_const  ----------------------------------------------------------
			struct mul_const {
				template<int i> struct Code {
					template<typename T>
					static void exec(tuple_ret_type& ret, const tuple_type& a, const T b) {
						ret.template get<i>() = a.template get<i>() * b;
					}
				};
			};
			//- div -----------------------------------------------------------------
			struct div {
				template<int i> struct Code {
					static void exec(tuple_ret_type& ret, const tuple_type& a, const tuple_type& b) {
						ret.template get<i>() = a.template get<i>() / b.template get<i>();
					}
				};
			};
			//- div_const  ----------------------------------------------------------
			struct div_const {
				template<int i> struct Code {
					template<typename T>
					static void exec(tuple_ret_type& ret, const tuple_type& a, const T b) {
						ret.template get<i>() = a.template get<i>() / b;
					}
				};
			};
			//- mod -----------------------------------------------------------------
			struct mod {
				template<int i> struct Code {
					static void exec(tuple_ret_type& ret, const tuple_type& a, const tuple_type& b) {
						ret.template get<i>() = a.template get<i>() % b.template get<i>();
					}
				};
			};
			//- mod_const  ----------------------------------------------------------
			struct mod_const {
				template<int i> struct Code {
					template<typename T>
					static void exec(tuple_ret_type& ret, const tuple_type& a, const T b) {
						ret.template get<i>() = a.template get<i>() % b;
					}
				};
			};
			//- negate --------------------------------------------------------------
			struct negate {
				template<int i> struct Code {
					static void exec(tuple_ret_type& ret, const tuple_type& a) {
						ret.template get<i>() = -a.template get<i>();
					}
				};
			};
			//- bitwise_not ---------------------------------------------------------
			struct bitwise_not {
				template<int i> struct Code {
					static void exec(tuple_ret_type& ret, const tuple_type& a) {
						ret.template get<i>() = ~a.template get<i>();
					}
				};
			};
			//- bitwise_and ---------------------------------------------------------
			struct bitwise_and {
				template<int i> struct Code {
					static void exec(tuple_ret_type& ret, const tuple_type& a, const tuple_type& b) {
						ret.template get<i>() = a.template get<i>() & b.template get<i>();
					}
				};
			};
			//- bitwise_or ----------------------------------------------------------
			struct bitwise_or {
				template<int i> struct Code {
					static void exec(tuple_ret_type& ret, const tuple_type& a, const tuple_type& b) {
						ret.template get<i>() = a.template get<i>() | b.template get<i>();
					}
				};
			};
		public:
			tuple_deref_type operator*() const {
				return priv::deref<count, tuple_deref_type, tuple_type>
					::exec(static_cast<const tuple_type&>(*this));
			}
			tuple_type& operator++() {
				meta::EFOR1<0, meta::Less, count, +1, pre_inc>::exec(
					static_cast<tuple_type&>(*this));
				return static_cast<tuple_type&>(*this);
			}
			tuple_type& operator--() {
				meta::EFOR1<0, meta::Less, count, +1, pre_dec>::exec(
					static_cast<tuple_type&>(*this));
				return static_cast<tuple_type&>(*this);
			}
			tuple_type& operator+=(const tuple_type& rhs) {
				meta::EFOR3<0, meta::Less, count, +1, add>::exec(
					static_cast<tuple_type&>(*this),
					static_cast<const tuple_type&>(*this),
					rhs);
				return static_cast<tuple_type&>(*this);
			}
			template<typename T> tuple_type& operator+=(const T& rhs) {
				meta::EFOR3<0, meta::Less, count, +1, add_const>::exec(
					static_cast<tuple_type&>(*this),
					static_cast<const tuple_type&>(*this),
					rhs);
				return static_cast<tuple_type&>(*this);
			}
			tuple_type& operator-=(const tuple_type& rhs) {
				meta::EFOR3<0, meta::Less, count, +1, sub>::exec(
					static_cast<tuple_type&>(*this),
					static_cast<const tuple_type&>(*this),
					rhs);
				return static_cast<tuple_type&>(*this);
			}
			template<typename T> tuple_type& operator-=(const T& rhs) {
				meta::EFOR3<0, meta::Less, count, +1, sub_const>::exec(
					static_cast<tuple_type&>(*this),
					static_cast<const tuple_type&>(*this),
					rhs);
				return static_cast<tuple_type&>(*this);
			}
			tuple_type& operator*=(const tuple_type& rhs) {
				meta::EFOR3<0, meta::Less, count, +1, mul>::exec(
					static_cast<tuple_type&>(*this),
					static_cast<const tuple_type&>(*this),
					rhs);
				return static_cast<tuple_type&>(*this);
			}
			template<typename T> tuple_type& operator*=(const T& rhs) {
				meta::EFOR3<0, meta::Less, count, +1, mul_const>::exec(
					static_cast<tuple_type&>(*this),
					static_cast<const tuple_type&>(*this),
					rhs);
				return static_cast<tuple_type&>(*this);
			}
			tuple_type& operator/=(const tuple_type& rhs) {
				meta::EFOR3<0, meta::Less, count, +1, div>::exec(
					static_cast<tuple_type&>(*this),
					static_cast<const tuple_type&>(*this),
					rhs);
				return static_cast<tuple_type&>(*this);
			}
			template<typename T> tuple_type& operator/=(const T& rhs) {
				meta::EFOR3<0, meta::Less, count, +1, div_const>::exec(
					static_cast<tuple_type&>(*this),
					static_cast<const tuple_type&>(*this),
					rhs);
				return static_cast<tuple_type&>(*this);
			}
			tuple_ret_type operator+(const tuple_type& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, add>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			template<typename T> tuple_ret_type operator+(const T& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, add_const>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			tuple_ret_type operator-(const tuple_type& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, sub>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			template<typename T> tuple_ret_type operator-(const T& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, sub_const>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			tuple_ret_type operator*(const tuple_type& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, mul>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			template<typename T> tuple_ret_type operator*(const T& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, mul_const>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			tuple_ret_type operator/(const tuple_type& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, div>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			template<typename T> tuple_ret_type operator/(const T& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, div_const>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			tuple_ret_type operator%(const tuple_type& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, mod>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			template<typename T> tuple_ret_type operator%(const T& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, mod_const>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			tuple_ret_type operator-() const {
				tuple_ret_type ret;
				meta::EFOR2<0, meta::Less, count, +1, negate>::exec(
					ret, static_cast<const tuple_type&>(*this));
				return ret;
			}
			tuple_ret_type operator~() const {
				tuple_ret_type ret;
				meta::EFOR2<0, meta::Less, count, +1, bitwise_not>::exec(
					ret, static_cast<const tuple_type&>(*this));
				return ret;
			}
			tuple_ret_type operator&(const tuple_type& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, bitwise_and>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
			tuple_ret_type operator|(const tuple_type& rhs) const {
				tuple_ret_type ret;
				meta::EFOR3<0, meta::Less, count, +1, bitwise_or>::exec(
					ret, static_cast<const tuple_type&>(*this), rhs);
				return ret;
			}
		};
	} // End of priv namespace
	
	template<typename T1>
	struct tuple<typelist<T1, null_type> > 
	: public boost::tuple<T1>
	, public priv::tuple_impl<tuple<typelist<T1, null_type> > > {
	private:
		enum { count = 1 };
	public:
		tuple() : boost::tuple<T1>() {
		}
		template<typename U1>
		tuple(const boost::tuple<U1>& p)
		: boost::tuple<T1>(p) {
		}
		tuple(typename boost::call_traits<T1>::param_type t1)
		: boost::tuple<T1>(t1) {
		}
		template<typename typelistT>
		tuple& operator=(const tuple<typelistT>& rhs) {
			meta::EFOR2<0, meta::Less, count, +1, priv::assign>::exec(
				*this, rhs);
			return *this;
		}
		template<typename T>
		tuple& operator=(const T& v) {
			meta::EFOR2<0, meta::Less, count, +1, priv::assign_value>::exec(
				*this, v);
			return *this;
		}
	};
	template<typename T1, typename T2>
	struct tuple<typelist<T1, typelist<T2, null_type> > > 
	: public boost::tuple<T1, T2>
	, public priv::tuple_impl<tuple<typelist<T1, typelist<T2, null_type> > > > {
	private:
		enum { count = 2 };
	public:
		tuple() : boost::tuple<T1, T2>() {
		}
		template<typename U1, typename U2>
		tuple(const boost::tuple<U1, U2>& p)
		: boost::tuple<T1, T2>(p) {
		}
		tuple(typename boost::call_traits<T1>::param_type t1,
		      typename boost::call_traits<T2>::param_type t2)
		: boost::tuple<T1, T2>(t1, t2) {
		}
		tuple(typename boost::call_traits<T1>::param_type t1)
		: boost::tuple<T1, T2>(t1, t1) {
		}
		template<typename typelistT>
		tuple& operator=(const tuple<typelistT>& rhs) {
			meta::EFOR2<0, meta::Less, count, +1, priv::assign>::exec(
				*this, rhs);
			return *this;
		}
		template<typename T>
		tuple& operator=(const T& v) {
			meta::EFOR2<0, meta::Less, count, +1, priv::assign_value>::exec(
				*this, v);
			return *this;
		}
	};
	template<typename T1, typename T2, typename T3>
	struct tuple<typelist<T1, typelist<T2, typelist<T3, null_type> > > > 
	: public boost::tuple<T1, T2, T3>
	, public priv::tuple_impl<tuple<typelist<T1, typelist<T2, typelist<T3, null_type> > > > > {
	private:
		enum { count = 3 };
	public:
		tuple() : boost::tuple<T1, T2, T3>() {
		}
		template<typename U1, typename U2, typename U3>
		tuple(const boost::tuple<U1, U2, U3>& p)
		: boost::tuple<T1, T2, T3>(p) {
		}
		tuple(typename boost::call_traits<T1>::param_type t1,
		      typename boost::call_traits<T2>::param_type t2,
			  typename boost::call_traits<T3>::param_type t3)
		: boost::tuple<T1, T2, T3>(t1, t2, t3) {
		}
		tuple(typename boost::call_traits<T1>::param_type t1)
		: boost::tuple<T1, T2, T3>(t1, t1, t1) {
		}
		template<typename typelistT>
		tuple& operator=(const tuple<typelistT>& rhs) {
			meta::EFOR2<0, meta::Less, count, +1, priv::assign>::exec(
				*this, rhs);
			return *this;
		}
		template<typename T>
		tuple& operator=(const T& v) {
			meta::EFOR2<0, meta::Less, count, +1, priv::assign_value>::exec(
				*this, v);
			return *this;
		}
	};
	template<typename T1, typename T2, typename T3, typename T4>
	struct tuple<typelist<T1, typelist<T2, typelist<T3, typelist<T4, null_type> > > > >
	: public boost::tuple<T1, T2, T3, T4> 
	, public priv::tuple_impl<tuple<typelist<T1, typelist<T2, typelist<T3, typelist<T4, null_type> > > > > > {
	private:
		enum { count = 4 };
	public:
		tuple() : boost::tuple<T1, T2, T3, T4>() {
		}
		template<typename U1, typename U2, typename U3, typename U4>
		tuple(const boost::tuple<U1, U2, U3, U4>& p)
		: boost::tuple<T1, T2, T3, T4>(p) {
		}
		tuple(typename boost::call_traits<T1>::param_type t1,
		      typename boost::call_traits<T2>::param_type t2,
			  typename boost::call_traits<T3>::param_type t3,
			  typename boost::call_traits<T4>::param_type t4)
		: boost::tuple<T1, T2, T3, T4>(t1, t2, t3, t4) {
		}
		tuple(typename boost::call_traits<T1>::param_type t1)
		: boost::tuple<T1, T2, T3, T4>(t1, t1, t1, t1) {
		}
		template<typename typelistT>
		tuple& operator=(const tuple<typelistT>& rhs) {
			meta::EFOR2<0, meta::Less, count, +1, priv::assign>::exec(
				*this, rhs);
			return *this;
		}
		template<typename T>
		tuple& operator=(const T& v) {
			meta::EFOR2<0, meta::Less, count, +1, priv::assign_value>::exec(
				*this, v);
			return *this;
		}
	};
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	struct tuple<typelist<T1, typelist<T2, typelist<T3, typelist<T4, typelist<T5, null_type> > > > > > 
	: public boost::tuple<T1, T2, T3, T4, T5> 
	, public priv::tuple_impl<tuple<typelist<T1, typelist<T2, typelist<T3, typelist<T4, typelist<T5, null_type> > > > > > > {
	private:
		enum { count = 5 };
	public:
		tuple() : boost::tuple<T1, T2, T3, T4, T5>() {
		}
		template<typename U1, typename U2, typename U3, typename U4, typename U5>
		tuple(const boost::tuple<U1, U2, U3, U4, U5>& p)
		: boost::tuple<T1, T2, T3, T4, T5>(p) {
		}
		tuple(typename boost::call_traits<T1>::param_type t1,
			  typename boost::call_traits<T2>::param_type t2,
			  typename boost::call_traits<T3>::param_type t3,
			  typename boost::call_traits<T4>::param_type t4,
			  typename boost::call_traits<T5>::param_type t5)
		: boost::tuple<T1, T2, T3, T4, T5>(t1, t2, t3, t4, t5) {
		}
		tuple(typename boost::call_traits<T1>::param_type t1)
		: boost::tuple<T1, T2, T3, T4, T5>(t1, t1, t1, t1, t1) {
		}
		template<typename typelistT>
		tuple& operator=(const tuple<typelistT>& rhs) {
			meta::EFOR2<0, meta::Less, count, +1, priv::assign>::exec(
				*this, rhs);
			return *this;
		}
		template<typename T>
		tuple& operator=(const T& v) {
			meta::EFOR2<0, meta::Less, count, +1, priv::assign_value>::exec(
				*this, v);
			return *this;
		}
	};
} // End of namespace

#endif
