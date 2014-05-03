#ifndef __LAMBDA_HPP__
#define __LAMBDA_HPP__

namespace Lambda {
	struct Unused {
		template<typename... _Arguments>
		Unused(_Arguments&...) {}
	};

	struct Yes {
		char m[1];
	};

	struct No {
		char m[2];
	};

	enum True {
		TRUE = 1
	};

	enum False {
		FALSE = 0
	};

	template<unsigned int const _cb>
	struct Boolean;

	template<>
	struct Boolean<sizeof(Yes)> {
		typedef True Type;
		static True constexpr s_Value = TRUE;
	};

	template<>
	struct Boolean<sizeof(No)> {
		typedef False Type;
		static False constexpr s_Value = FALSE;
	};

	template<typename _Type>
	struct CleanType {
		typedef _Type Type;
	};

	template<typename _Type>
	struct CleanType<_Type&> {
		typedef typename CleanType<_Type>::Type Type;
	};

	template<typename _Type>
	struct CleanType<_Type&&> {
		typedef typename CleanType<_Type>::Type Type;
	};

	template<typename _Type>
	struct CleanType<_Type const> {
		typedef typename CleanType<_Type>::Type Type;
	};

	template<typename _Type>
	struct CleanType<_Type volatile> {
		typedef typename CleanType<_Type>::Type Type;
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

	template<typename _Type>
	struct IsFunctor {
		static Yes test(Functor*);
		static No test(void*);
		typedef Boolean<sizeof(test((typename CleanType<_Type>::Type*)0))> BooleanType;
		static typename BooleanType::Type constexpr s_Value = BooleanType::s_Value;
	};

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

	template<typename _Type>
	struct Constant :
		public Functor
	{
		_Type m_Value;

		explicit Constant(_Type &&a_rrValue)
			:
		m_Value((_Type&&)a_rrValue) {}

		template<typename ..._Arguments>
		inline _Type operator () (_Arguments&&...) {
			return m_Value;
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

#define LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(Operator, Name) \
	template<typename _Operand, True const _IsFunctor = IsFunctor<_Operand>::s_Value> \
	struct Name : \
		public UnaryFunctor \
	{ \
		_Operand m_Operand; \
		explicit Name(_Operand &&a_rrOperand) \
			: \
		m_Operand((_Operand&&)a_rrOperand) {} \
		template<typename ..._Arguments> \
		inline auto operator () (_Arguments &&...rrArguments) -> decltype(Operator(m_Operand((_Arguments&&)rrArguments...))) { \
			return Operator(m_Operand((_Arguments&&)rrArguments...)); \
		} \
	};

#define LAMBDA_POSTFIX_UNARY_FUNCTOR_CLASS(Operator, Name) \
	template<typename _Operand, True const _IsFunctor = IsFunctor<_Operand>::s_Value> \
	struct Name : \
		public UnaryFunctor \
	{ \
		_Operand m_Operand; \
		explicit Name(_Operand &&a_rrOperand) \
			: \
		m_Operand((_Operand&&)a_rrOperand) {} \
		template<typename ..._Arguments> \
		inline auto operator () (_Arguments &&...rrArguments) -> decltype((m_Operand((_Arguments&&)rrArguments...))Operator) { \
			return (m_Operand((_Arguments&&)rrArguments...))Operator; \
		} \
	};

namespace Lambda {
	struct UnaryFunctor : public Functor {};

	LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(&, Address)
	LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(*, Indirection)
	LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(+, UnaryPlus)
	LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(-, UnaryMinus)
	LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(!, LogicalNot)
	LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(~, BitwiseNot)
	LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(++, PreIncrement)
	LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(--, PreDecrement)
	LAMBDA_POSTFIX_UNARY_FUNCTOR_CLASS(++, PostIncrement)
	LAMBDA_POSTFIX_UNARY_FUNCTOR_CLASS(--, PostDecrement)
}

#define LAMBDA_PREFIX_UNARY_OPERATOR(Operator, Name) \
	template<typename _Operand> \
	inline Lambda::Name<_Operand> operator Operator (_Operand &&rrOperand) { \
		return Lambda::Name<_Operand>((_Operand&&)rrOperand); \
	}

LAMBDA_PREFIX_UNARY_OPERATOR(&, Address)
LAMBDA_PREFIX_UNARY_OPERATOR(*, Indirection)
LAMBDA_PREFIX_UNARY_OPERATOR(+, UnaryPlus)
LAMBDA_PREFIX_UNARY_OPERATOR(-, UnaryMinus)
LAMBDA_PREFIX_UNARY_OPERATOR(!, LogicalNot)
LAMBDA_PREFIX_UNARY_OPERATOR(~, BitwiseNot)
LAMBDA_PREFIX_UNARY_OPERATOR(++, PreIncrement)
LAMBDA_PREFIX_UNARY_OPERATOR(--, PreDecrement)

#endif
