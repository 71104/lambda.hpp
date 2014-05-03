#ifndef __LAMBDA_HPP__
#define __LAMBDA_HPP__

namespace Lambda {
	struct Unused {
		template<typename... _Arguments>
		Unused(_Arguments&...) {}
	};

	template<unsigned int const _i, typename ..._Types>
	struct FindType {
		typedef typename FindType<_i - 1, _Types...>::Type Type;
	};

	template<typename _First, typename ..._Others>
	struct FindType<0, _First, _Others...> {
		typedef _First Type;
	};

	struct Functor {};

	struct Null :
		public Functor
	{
		template<typename ..._Arguments>
		inline void operator () (_Arguments &&...rrArguments) {}
	};

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

static Lambda::Null _0;
static Lambda::Bind<0> _1;
static Lambda::Bind<1> _2;
static Lambda::Bind<2> _3;
static Lambda::Bind<3> _4;
static Lambda::Bind<4> _5;
static Lambda::Bind<5> _6;
static Lambda::Bind<6> _7;
static Lambda::Bind<7> _8;
static Lambda::Bind<8> _9;
static Lambda::Bind<9> _10;

static Lambda::Unused g_Unused = Lambda::Unused(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10);

#endif
