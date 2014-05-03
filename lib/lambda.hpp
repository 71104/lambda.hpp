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

	static Bind<0> _1;
	static Bind<1> _2;
	static Bind<2> _3;
	static Bind<3> _4;
	static Bind<4> _5;
	static Bind<5> _6;
	static Bind<6> _7;
	static Bind<7> _8;
	static Bind<8> _9;
	static Bind<9> _10;

	template<typename _Operand>
	struct UnaryFunctor :
		public Functor
	{
		_Operand m_Operand;

		template<typename ..._Arguments>
		inline void operator () (_Arguments &&...rrArguments) {
			m_Operand((_Arguments&&)rrArguments...);
		}
	};
}

#endif
