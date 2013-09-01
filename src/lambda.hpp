#include <type_traits>


namespace Lambda {
	using namespace std;


	struct Functor {};


	template<typename _Type>
	struct IsBound {
		static bool constexpr s_f = !is_base_of<Functor, typename remove_cv<typename remove_reference<_Type>::type>::type>::value;
	};

	template<typename _Type>
	inline _Type &&Pass(_Type &&rr) {
		return rr;
	}


	template<unsigned int _i>
	struct Bind :
		public Bind<_i - 1>
	{
		template<typename _First, typename... _Arguments>
		inline auto operator () (_First&&, _Arguments&&... Other) -> decltype(Bind<_i - 1>::operator () (Other...)) {
			return Bind<_i - 1>::operator () (Other...);
		}
	};

	template<>
	struct Bind<0> :
		public Functor
	{
		template<typename _Type, typename... _Other>
		inline _Type &&operator () (_Type &&rr, _Other&&...) {
			return rr;
		}
	};


#define LAMBDA_FUNCTOR_CLASS(Operator, Name) \
	template<typename _Left, typename _Right, bool _fLeftBound = IsBound<_Left>::s_f, bool _fRightBound = IsBound<_Right>::s_f> \
	struct Name; \
	template<typename _Left, typename _Right> \
	struct Name<_Left, _Right, false, false> : \
		public Functor \
	{ \
		_Left m_Left; \
		_Right m_Right; \
		Name(_Left &&a_rrLeft, _Right &&a_rrRight) \
			: \
		m_Left(Pass(a_rrLeft)), \
			m_Right(Pass(a_rrRight)) {} \
		template<typename... _Arguments> \
		inline auto operator () (_Arguments&&... Arguments) -> decltype(m_Left(Arguments...) Operator m_Right(Arguments...)) { \
			return m_Left(Arguments...) Operator m_Right(Arguments...); \
		} \
	}; \
	template<typename _Left, typename _Right> \
	struct Name<_Left, _Right, true, false> : \
		public Functor \
	{ \
		_Left m_Left; \
		_Right m_Right; \
		Name(_Left &&a_rrLeft, _Right &&a_rrRight) \
			: \
		m_Left(Pass(a_rrLeft)), \
			m_Right(Pass(a_rrRight)) {} \
		template<typename... _Arguments> \
		inline auto operator () (_Arguments&&... Arguments) -> decltype(m_Left Operator m_Right(Arguments...)) { \
			return m_Left Operator m_Right(Arguments...); \
		} \
	}; \
	template<typename _Left, typename _Right> \
	struct Name<_Left, _Right, false, true> : \
		public Functor \
	{ \
		_Left m_Left; \
		_Right m_Right; \
		Name(_Left &&a_rrLeft, _Right &&a_rrRight) \
			: \
		m_Left(Pass(a_rrLeft)), \
			m_Right(Pass(a_rrRight)) {} \
		template<typename... _Arguments> \
		inline auto operator () (_Arguments&&... Arguments) -> decltype(m_Left(Arguments...) Operator m_Right) { \
			return m_Left(Arguments...) Operator m_Right; \
		} \
	}; \
	template<typename _Left, typename _Right> \
	struct Name<_Left, _Right, true, true> : \
		public Functor \
	{ \
		_Left m_Left; \
		_Right m_Right; \
		Name(_Left &&a_rrLeft, _Right &&a_rrRight) \
			: \
		m_Left(Pass(a_rrLeft)), \
			m_Right(Pass(a_rrRight)) {} \
		template<typename... _Arguments> \
		inline auto operator () (_Arguments&&...) -> decltype(m_Left Operator m_Right) { \
			return m_Left Operator m_Right; \
		} \
	};

LAMBDA_FUNCTOR_CLASS(+, Plus)
LAMBDA_FUNCTOR_CLASS(+=, CompoundPlus)
LAMBDA_FUNCTOR_CLASS(-, Minus)
LAMBDA_FUNCTOR_CLASS(-=, CompoundMinus)
LAMBDA_FUNCTOR_CLASS(*, Multiply)
LAMBDA_FUNCTOR_CLASS(*=, CompoundMultiply)
LAMBDA_FUNCTOR_CLASS(/, Divide)
LAMBDA_FUNCTOR_CLASS(/=, CompoundDivide)
LAMBDA_FUNCTOR_CLASS(%, Modulus)
LAMBDA_FUNCTOR_CLASS(%=, CompoundModulus)
LAMBDA_FUNCTOR_CLASS(&, BitwiseAnd)
LAMBDA_FUNCTOR_CLASS(|, BitwiseOr)
LAMBDA_FUNCTOR_CLASS(^, BitwiseXor)
LAMBDA_FUNCTOR_CLASS(&=, CompoundAnd)
LAMBDA_FUNCTOR_CLASS(|=, CompoundOr)
LAMBDA_FUNCTOR_CLASS(^=, CompoundXor)
LAMBDA_FUNCTOR_CLASS(&&, LogicalAnd)
LAMBDA_FUNCTOR_CLASS(||, LogicalOr)
LAMBDA_FUNCTOR_CLASS(<<, LeftShift)
LAMBDA_FUNCTOR_CLASS(<<=, CompoundLeftShift)
LAMBDA_FUNCTOR_CLASS(>>, RightShift)
LAMBDA_FUNCTOR_CLASS(>>=, CompoundRightShift)
//LAMBDA_FUNCTOR_CLASS(=, Assignment)
LAMBDA_FUNCTOR_CLASS(==, Equals)
LAMBDA_FUNCTOR_CLASS(!=, NotEqual)
LAMBDA_FUNCTOR_CLASS(<, LessThan)
LAMBDA_FUNCTOR_CLASS(>, GreaterThan)
LAMBDA_FUNCTOR_CLASS(<=, LessThanOrEqualTo)
LAMBDA_FUNCTOR_CLASS(>=, GreaterThanOrEqualTo)

}


Lambda::Bind<0> _1;
Lambda::Bind<1> _2;
Lambda::Bind<2> _3;
Lambda::Bind<3> _4;
Lambda::Bind<4> _5;
Lambda::Bind<5> _6;
Lambda::Bind<6> _7;
Lambda::Bind<7> _8;
Lambda::Bind<8> _9;
Lambda::Bind<9> _10;


#define LAMBDA_OPERATOR(Operator, FunctorClass) \
	template<typename _Left, typename _Right> \
	Lambda::FunctorClass<_Left, _Right> operator Operator (_Left &&rrLeft, _Right &&rrRight) { \
		return Lambda::FunctorClass<_Left, _Right>((_Left&&)rrLeft, (_Right&&)rrRight); \
	}

LAMBDA_OPERATOR(+, Plus)
LAMBDA_OPERATOR(+=, CompoundPlus)
LAMBDA_OPERATOR(-, Minus)
LAMBDA_OPERATOR(-=, CompoundMinus)
LAMBDA_OPERATOR(*, Multiply)
LAMBDA_OPERATOR(*=, CompoundMultiply)
LAMBDA_OPERATOR(/, Divide)
LAMBDA_OPERATOR(/=, CompoundDivide)
LAMBDA_OPERATOR(%, Modulus)
LAMBDA_OPERATOR(%=, CompoundModulus)
LAMBDA_OPERATOR(&, BitwiseAnd)
LAMBDA_OPERATOR(|, BitwiseOr)
LAMBDA_OPERATOR(^, BitwiseXor)
LAMBDA_OPERATOR(&=, CompoundAnd)
LAMBDA_OPERATOR(|=, CompoundOr)
LAMBDA_OPERATOR(^=, CompoundXor)
LAMBDA_OPERATOR(&&, LogicalAnd)
LAMBDA_OPERATOR(||, LogicalOr)
LAMBDA_OPERATOR(<<, LeftShift)
LAMBDA_OPERATOR(<<=, CompoundLeftShift)
LAMBDA_OPERATOR(>>, RightShift)
LAMBDA_OPERATOR(>>=, CompoundRightShift)
//LAMBDA_OPERATOR(=, Assignment)
LAMBDA_OPERATOR(==, Equals)
LAMBDA_OPERATOR(!=, NotEqual)
LAMBDA_OPERATOR(<, LessThan)
LAMBDA_OPERATOR(>, GreaterThan)
LAMBDA_OPERATOR(<=, LessThanOrEqualTo)
LAMBDA_OPERATOR(>=, GreaterThanOrEqualTo)
