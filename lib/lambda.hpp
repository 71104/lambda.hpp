#ifndef __LAMBDA_HPP__
#define __LAMBDA_HPP__

namespace Lambda {
	template<unsigned int const _i, typename ..._Types>
	struct FindType {
		typedef typename FindType<_i - 1, _Types...>::Type Type;
	};

	template<typename _First, typename ..._Others>
	struct FindType<0, _First, _Others...> {
		typedef _First Type;
	};

	struct Functor {};

	template<unsigned int const _i>
	struct Bind :
		public Functor
	{
		template<typename _First, typename ..._Others>
		inline typename FindType<_i, _First, _Others...>::Type operator () (_First &&rrFirst, _Others &&...rrOthers) {
			return Bind<_i - 1>()((_Others&&)rrOthers...);
		}
	};

	template<>
	struct Bind<0> :
		public Functor
	{
		template<typename _First, typename ..._Others>
		inline _First operator () (_First &&rrFirst, _Others &&...rrOthers) {
			return rrFirst;
		}
	};
}

#endif
