#ifndef __LAMBDA_HPP__
#define __LAMBDA_HPP__

#include <type_traits>

namespace Lambda {
	using namespace std;

	struct Unused {
		template<typename ..._Arguments>
		Unused(_Arguments&...) {}
	};

	enum True {
		TRUE = 1
	};

	enum False {
		FALSE = 0
	};

	template<bool const _f>
	struct Boolean;

	template <>
	struct Boolean<true> {
		typedef True Type;
		static True constexpr s_Value = TRUE;
	};

	template <>
	struct Boolean<false> {
		typedef False Type;
		static False constexpr s_Value = FALSE;
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
		typedef typename remove_cv<typename remove_reference<_Type>::type>::type Clean;
		typedef Boolean<is_base_of<Functor, Clean>::value> BooleanType;
		static typename BooleanType::Type constexpr s_Value = BooleanType::s_Value;
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

	template<typename _Type, False const _IsFunctor = IsFunctor<_Type>::s_Value>
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

template<typename _Operand>
inline Lambda::Constant<_Operand> _0(_Operand &&rrOperand) {
	return Lambda::Constant<_Operand>((_Operand&&)rrOperand);
}

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

static Lambda::Unused g_Unused = Lambda::Unused(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10);

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

#define LAMBDA_BINARY_FUNCTOR_CLASS(Operator, Name) \
	template<typename _Left, typename _Right, \
		True const _LeftIsFunctor = IsFunctor<_Left>::s_Value, \
		True const _RightIsFunctor = IsFunctor<_Right>::s_Value> \
	struct Name : \
		public BinaryFunctor \
	{ \
		_Left m_Left; \
		_Right m_Right; \
		Name(_Left &&a_rrLeft, _Right &&a_rrRight) \
			: \
		m_Left((_Left&&)a_rrLeft), \
			m_Right((_Right&&)a_rrRight) {} \
		template<typename ..._Arguments> \
		inline auto operator () (_Arguments &&...rrArguments) -> decltype((m_Left((_Arguments&&)rrArguments...)) Operator (m_Right((_Arguments&&)rrArguments...))) { \
			return (m_Left((_Arguments&&)rrArguments...)) Operator (m_Right((_Arguments&&)rrArguments...)); \
		} \
	};

#define LAMBDA_COMMA ,

namespace Lambda {
	struct BinaryFunctor : public Functor {};

	LAMBDA_BINARY_FUNCTOR_CLASS(+, BinaryPlus)
	LAMBDA_BINARY_FUNCTOR_CLASS(+=, CompoundPlus)
	LAMBDA_BINARY_FUNCTOR_CLASS(-, BinaryMinus)
	LAMBDA_BINARY_FUNCTOR_CLASS(-=, CompoundMinus)
	LAMBDA_BINARY_FUNCTOR_CLASS(*, Multiply)
	LAMBDA_BINARY_FUNCTOR_CLASS(*=, CompoundMultiply)
	LAMBDA_BINARY_FUNCTOR_CLASS(/, Divide)
	LAMBDA_BINARY_FUNCTOR_CLASS(/=, CompoundDivide)
	LAMBDA_BINARY_FUNCTOR_CLASS(%, Modulus)
	LAMBDA_BINARY_FUNCTOR_CLASS(%=, CompoundModulus)
	LAMBDA_BINARY_FUNCTOR_CLASS(&, BitwiseAnd)
	LAMBDA_BINARY_FUNCTOR_CLASS(|, BitwiseOr)
	LAMBDA_BINARY_FUNCTOR_CLASS(^, BitwiseXor)
	LAMBDA_BINARY_FUNCTOR_CLASS(&=, CompoundAnd)
	LAMBDA_BINARY_FUNCTOR_CLASS(|=, CompoundOr)
	LAMBDA_BINARY_FUNCTOR_CLASS(^=, CompoundXor)
	LAMBDA_BINARY_FUNCTOR_CLASS(&&, LogicalAnd)
	LAMBDA_BINARY_FUNCTOR_CLASS(||, LogicalOr)
	LAMBDA_BINARY_FUNCTOR_CLASS(<<, LeftShift)
	LAMBDA_BINARY_FUNCTOR_CLASS(<<=, CompoundLeftShift)
	LAMBDA_BINARY_FUNCTOR_CLASS(>>, RightShift)
	LAMBDA_BINARY_FUNCTOR_CLASS(>>=, CompoundRightShift)
	LAMBDA_BINARY_FUNCTOR_CLASS(==, Equals)
	LAMBDA_BINARY_FUNCTOR_CLASS(!=, NotEqual)
	LAMBDA_BINARY_FUNCTOR_CLASS(<, LessThan)
	LAMBDA_BINARY_FUNCTOR_CLASS(>, GreaterThan)
	LAMBDA_BINARY_FUNCTOR_CLASS(<=, LessThanOrEqualTo)
	LAMBDA_BINARY_FUNCTOR_CLASS(>=, GreaterThanOrEqualTo)
	LAMBDA_BINARY_FUNCTOR_CLASS(LAMBDA_COMMA, Comma)

	template<typename _Left, typename _Right,
		True const _LeftIsFunctor = IsFunctor<_Left>::s_Value,
		True const _RightIsFunctor = IsFunctor<_Right>::s_Value>
	struct Subscript :
		public BinaryFunctor
	{
		_Left m_Left;
		_Right m_Right;

		Subscript(_Left &&a_rrLeft, _Right &&a_rrRight)
			:
		m_Left((_Left&&)a_rrLeft),
			m_Right((_Right&&)a_rrRight) {}

		template<typename ..._Arguments>
		inline auto operator () (_Arguments &&...rrArguments) -> decltype((m_Left((_Arguments&&)rrArguments...))[(m_Right((_Arguments&&)rrArguments...))]) {
			return (m_Left((_Arguments&&)rrArguments...))[(m_Right((_Arguments&&)rrArguments...))];
		}
	};
}

#define LAMBDA_BINARY_OPERATOR(Operator, Name) \
	template<typename _Left, typename _Right> \
	inline Lambda::Name<_Left, _Right> operator Operator (_Left &&rrLeft, _Right &&rrRight) { \
		return Lambda::Name<_Left, _Right>((_Left&&)rrLeft, (_Right&&)rrRight); \
	} \
	template<typename _Left, typename _Right> \
	inline Lambda::Name<Lambda::Constant<_Left>, _Right> operator Operator (_Left &&rrLeft, _Right &&rrRight) { \
		return Lambda::Name<Lambda::Constant<_Left>, _Right>(Lambda::Constant<_Left>((_Left&&)rrLeft), (_Right&&)rrRight); \
	} \
	template<typename _Left, typename _Right> \
	inline Lambda::Name<_Left, Lambda::Constant<_Right>> operator Operator (_Left &&rrLeft, _Right &&rrRight) { \
		return Lambda::Name<_Left, Lambda::Constant<_Right>>((_Left&&)rrLeft, Lambda::Constant<_Right>((_Right&&)rrRight)); \
	}

LAMBDA_BINARY_OPERATOR(+, BinaryPlus)
LAMBDA_BINARY_OPERATOR(+=, CompoundPlus)
LAMBDA_BINARY_OPERATOR(-, BinaryMinus)
LAMBDA_BINARY_OPERATOR(-=, CompoundMinus)
LAMBDA_BINARY_OPERATOR(*, Multiply)
LAMBDA_BINARY_OPERATOR(*=, CompoundMultiply)
LAMBDA_BINARY_OPERATOR(/, Divide)
LAMBDA_BINARY_OPERATOR(/=, CompoundDivide)
LAMBDA_BINARY_OPERATOR(%, Modulus)
LAMBDA_BINARY_OPERATOR(%=, CompoundModulus)
LAMBDA_BINARY_OPERATOR(&, BitwiseAnd)
LAMBDA_BINARY_OPERATOR(|, BitwiseOr)
LAMBDA_BINARY_OPERATOR(^, BitwiseXor)
LAMBDA_BINARY_OPERATOR(&=, CompoundAnd)
LAMBDA_BINARY_OPERATOR(|=, CompoundOr)
LAMBDA_BINARY_OPERATOR(^=, CompoundXor)
LAMBDA_BINARY_OPERATOR(&&, LogicalAnd)
LAMBDA_BINARY_OPERATOR(||, LogicalOr)
LAMBDA_BINARY_OPERATOR(<<, LeftShift)
LAMBDA_BINARY_OPERATOR(<<=, CompoundLeftShift)
LAMBDA_BINARY_OPERATOR(>>, RightShift)
LAMBDA_BINARY_OPERATOR(>>=, CompoundRightShift)
LAMBDA_BINARY_OPERATOR(==, Equals)
LAMBDA_BINARY_OPERATOR(!=, NotEqual)
LAMBDA_BINARY_OPERATOR(<, LessThan)
LAMBDA_BINARY_OPERATOR(>, GreaterThan)
LAMBDA_BINARY_OPERATOR(<=, LessThanOrEqualTo)
LAMBDA_BINARY_OPERATOR(>=, GreaterThanOrEqualTo)
LAMBDA_BINARY_OPERATOR(LAMBDA_COMMA, Comma)

template<typename _Condition, typename _Then>
struct if_then :
	public Lambda::Functor
{
	_Condition m_Condition;
	_Then m_Then;

	if_then(_Condition &&a_rrCondition, _Then &&a_rrThen)
		:
	m_Condition((_Condition&&)a_rrCondition),
		m_Then((_Then&&)a_rrThen) {}

	template<typename ..._Arguments>
	inline void operator () (_Arguments &&...rrArguments) {
		if (m_Condition((_Arguments&&)rrArguments...)) {
			m_Then((_Arguments&&)rrArguments...);
		}
	}
};

template<typename _Condition, typename _Then, typename _Else>
struct if_then_else :
	public Lambda::Functor
{
	_Condition m_Condition;
	_Then m_Then;
	_Else m_Else;

	if_then_else(_Condition &&a_rrCondition, _Then &&a_rrThen, _Else &&a_rrElse)
		:
	m_Condition((_Condition&&)a_rrCondition),
		m_Then((_Then&&)a_rrThen),
		m_Else((_Else&&)a_rrElse) {}

	template<typename ..._Arguments>
	inline void operator () (_Arguments &&...rrArguments) {
		if (m_Condition((_Arguments&&)rrArguments...)) {
			m_Then((_Arguments&&)rrArguments...);
		} else {
			m_Else((_Arguments&&)rrArguments...);
		}
	}
};

template<typename _Condition, typename _Then, typename _Else>
struct if_then_else_return :
	public Lambda::Functor
{
	_Condition m_Condition;
	_Then m_Then;
	_Else m_Else;

	if_then_else_return(_Condition &&a_rrCondition, _Then &&a_rrThen, _Else &&a_rrElse)
		:
	m_Condition((_Condition&&)a_rrCondition),
		m_Then((_Then&&)a_rrThen),
		m_Else((_Else&&)a_rrElse) {}

	template<typename ..._Arguments>
	inline auto operator () (_Arguments &&...rrArguments) -> decltype(m_Condition((_Arguments&&)rrArguments...) ? m_Then((_Arguments&&)rrArguments...) : m_Else((_Arguments&&)rrArguments...)) {
		return m_Condition((_Arguments&&)rrArguments...) ? m_Then((_Arguments&&)rrArguments...) : m_Else((_Arguments&&)rrArguments...);
	}
};

template<typename _Condition, typename _Body>
struct while_loop :
	public Lambda::Functor
{
	_Condition m_Condition;
	_Body m_Body;

	while_loop(_Condition &&a_rrCondition, _Body &&a_rrBody = _0(0))
		:
	m_Condition((_Condition&&)a_rrCondition),
		m_Body((_Body&&)a_rrBody) {}

	template<typename ..._Arguments>
	inline void operator () (_Arguments &&...rrArguments) {
		while (m_Condition((_Arguments&&)rrArguments...)) {
			m_Body((_Arguments&&)rrArguments...);
		}
	}
};

template<typename _Condition, typename _Body>
struct do_while_loop :
	public Lambda::Functor
{
	_Body m_Body;
	_Condition m_Condition;

	do_while_loop(_Body &&a_rrBody, _Condition &&a_rrCondition)
		:
	m_Body((_Body&&)a_rrBody),
		m_Condition((_Condition&&)a_rrCondition) {}

	do_while_loop(_Condition &&a_rrCondition)
		:
	m_Body(_0(0)),
		m_Condition((_Condition&&)a_rrCondition) {}

	template<typename ..._Arguments>
	inline void operator () (_Arguments &&...rrArguments) {
		do {
			m_Body((_Arguments&&)rrArguments...);
		} while (m_Condition((_Arguments&&)rrArguments...));
	}
};

template<typename _Initializer, typename _Condition, typename _Increment, typename _Body>
struct for_loop :
	public Lambda::Functor
{
	_Initializer m_Initializer;
	_Condition m_Condition;
	_Increment m_Increment;
	_Body m_Body;

	for_loop(_Initializer &&a_rrInitializer, _Condition &&a_rrCondition, _Increment &&a_rrIncrement, _Body &&a_rrBody)
		:
	m_Initializer((_Initializer&&)a_rrInitializer),
		m_Condition((_Condition&&)a_rrCondition),
		m_Increment((_Increment&&)a_rrIncrement),
		m_Body((_Body&&)a_rrBody) {}

	template<typename ..._Arguments>
	inline void operator () (_Arguments &&...rrArguments) {
		for (
			m_Initializer((_Arguments&&)rrArguments...);
			m_Condition((_Arguments&&)rrArguments...);
			m_Increment((_Arguments&&)rrArguments...)
		) {
			m_Body((_Arguments&&)rrArguments...);
		}
	}
};

#endif
