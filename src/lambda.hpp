#include <type_traits>


namespace Lambda {
	using namespace std;


	struct Functor {};


	template<typename _Type>
	struct IsFree {
		static bool constexpr s_f = is_base_of<
			Functor,
			typename remove_cv<
				typename remove_reference<_Type>::type
				>::type
			>::value;
	};

	template<typename _Type>
	struct IsBound {
		static bool constexpr s_f = !is_base_of<
			Functor,
			typename remove_cv<
				typename remove_reference<_Type>::type
				>::type
			>::value;
	};


	template<typename _Type, bool _fFree = IsFree<_Type>::s_f>
	struct FunctorTraits;

	template<typename _Type>
	struct FunctorTraits<_Type, true> {};


	template<typename _Left, typename _Right, bool _fLeftFree = IsFree<_Left>::s_f, bool _fRightFree = IsFree<_Right>::s_f>
	struct FunctorTraits2;

	template<typename _Left, typename _Right>
	struct FunctorTraits2<_Left, _Right, true, false> {
		template<typename _FunctorClass>
		static inline _FunctorClass Build(_Left &&rrLeft, _Right &&rrRight) {
			return _FunctorClass((_Left&&)rrLeft, (_Right&&)rrRight);
		}
	};

	template<typename _Left, typename _Right>
	struct FunctorTraits2<_Left, _Right, false, true> {
		template<typename _FunctorClass>
		static inline _FunctorClass Build(_Left &&rrLeft, _Right &&rrRight) {
			return _FunctorClass((_Left&&)rrLeft, (_Right&&)rrRight);
		}
	};

	template<typename _Left, typename _Right>
	struct FunctorTraits2<_Left, _Right, true, true> {
		template<typename _FunctorClass>
		static inline _FunctorClass Build(_Left &&rrLeft, _Right &&rrRight) {
			return _FunctorClass((_Left&&)rrLeft, (_Right&&)rrRight);
		}
	};


	struct Null :
		public Functor
	{
		template<typename... _Arguments>
		inline void operator () (_Arguments&&...) {}
	};


	template<unsigned int _i>
	struct Bind :
		public Functor
	{
		template<typename _First, typename... _Other>
		static inline auto Apply(_First&&, _Other&&... rrOther) -> decltype(Bind<_i - 1>::Apply(rrOther...)) {
			return Bind<_i - 1>::Apply(rrOther...);
		}

		template<typename... _Arguments>
		inline auto operator () (_Arguments&&... rrArguments) -> decltype(Bind<_i>::Apply(rrArguments...)) {
			return Bind<_i>::Apply(rrArguments...);
		}
	};

	template<>
	struct Bind<0> :
		public Functor
	{
		template<typename _Type, typename... _Other>
		static inline _Type &&Apply(_Type &&rr, _Other&&...) {
			return rr;
		}

		template<typename _Type, typename... _Other>
		inline _Type &&operator () (_Type &&rr, _Other&&...) {
			return rr;
		}
	};


#define LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(Operator, Name) \
	template<typename _Operand, bool _fBound = IsBound<_Operand>::s_f> \
	struct Name; \
	template<typename _Operand> \
	struct Name<_Operand, false> : \
		public Functor \
	{ \
		_Operand m_Operand; \
		Name(_Operand &&a_rrOperand) \
			: \
		m_Operand((_Operand&&)a_rrOperand) {} \
		template<typename... _Arguments> \
		inline auto operator () (_Arguments&&... Arguments) -> decltype(Operator(m_Operand(Arguments...))) { \
			return Operator(m_Operand(Arguments...)); \
		} \
	}; \
	template<typename _Operand> \
	struct Name<_Operand, true> : \
		public Functor \
	{ \
		_Operand m_Operand; \
		Name(_Operand &&a_rrOperand) \
			: \
		m_Operand((_Operand&&)a_rrOperand) {} \
		template<typename... _Arguments> \
		inline auto operator () (_Arguments&&...) -> decltype(Operator m_Operand) { \
			return Operator m_Operand; \
		} \
	};

#define LAMBDA_POSTFIX_UNARY_FUNCTOR_CLASS(Operator, Name) \
	template<typename _Operand, bool _fBound = IsBound<_Operand>::s_f> \
	struct Name; \
	template<typename _Operand> \
	struct Name<_Operand, false> : \
		public Functor \
	{ \
		_Operand m_Operand; \
		Name(_Operand &&a_rrOperand) \
			: \
		m_Operand((_Operand&&)a_rrOperand) {} \
		template<typename... _Arguments> \
		inline auto operator () (_Arguments&&... Arguments) -> decltype((m_Operand(Arguments...)) Operator) { \
			return (m_Operand(Arguments...)) Operator; \
		} \
	}; \
	template<typename _Operand> \
	struct Name<_Operand, true> : \
		public Functor \
	{ \
		_Operand m_Operand; \
		Name(_Operand &&a_rrOperand) \
			: \
		m_Operand((_Operand&&)a_rrOperand) {} \
		template<typename... _Arguments> \
		inline auto operator () (_Arguments&&...) -> decltype(m_Operand Operator) { \
			return m_Operand Operator; \
		} \
	};

LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(*, Indirection)
LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(+, UnaryPlus)
LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(-, UnaryMinus)
LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(!, LogicalNot)
LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(~, BitwiseNot)
LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(++, PreIncrement)
LAMBDA_PREFIX_UNARY_FUNCTOR_CLASS(--, PreDecrement)
LAMBDA_POSTFIX_UNARY_FUNCTOR_CLASS(++, PostIncrement)
LAMBDA_POSTFIX_UNARY_FUNCTOR_CLASS(--, PostDecrement)


#define LAMBDA_BINARY_FUNCTOR_CLASS(Operator, Name) \
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
		m_Left((_Left&&)a_rrLeft), \
			m_Right((_Right&&)a_rrRight) {} \
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
		m_Left((_Left&&)a_rrLeft), \
			m_Right((_Right&&)a_rrRight) {} \
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
		m_Left((_Left&&)a_rrLeft), \
			m_Right((_Right&&)a_rrRight) {} \
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
		m_Left((_Left&&)a_rrLeft), \
			m_Right((_Right&&)a_rrRight) {} \
		template<typename... _Arguments> \
		inline auto operator () (_Arguments&&...) -> decltype(m_Left Operator m_Right) { \
			return m_Left Operator m_Right; \
		} \
	};

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
LAMBDA_BINARY_FUNCTOR_CLASS(=, Assignment)
LAMBDA_BINARY_FUNCTOR_CLASS(==, Equals)
LAMBDA_BINARY_FUNCTOR_CLASS(!=, NotEqual)
LAMBDA_BINARY_FUNCTOR_CLASS(<, LessThan)
LAMBDA_BINARY_FUNCTOR_CLASS(>, GreaterThan)
LAMBDA_BINARY_FUNCTOR_CLASS(<=, LessThanOrEqualTo)
LAMBDA_BINARY_FUNCTOR_CLASS(>=, GreaterThanOrEqualTo)


	template<typename _Type>
	struct Delay :
		public Functor
	{
		_Type m_;

		Delay(_Type &&a_rr)
			:
		m_((_Type&&)a_rr) {}

		template<typename... _Arguments>
		inline _Type operator () (_Arguments&&...) {
			return (_Type&&)m_;
		}
	};


	template<typename _Condition, typename _Then>
	struct IfThen :
		public Functor
	{
		_Condition m_Condition;
		_Then m_Then;

		IfThen(_Condition &&a_rrCondition, _Then &&a_rrThen)
			:
		m_Condition((_Condition&&)a_rrCondition),
			m_Then((_Then&&)a_rrThen) {}

		template<typename... _Arguments>
		inline void operator () (_Arguments&&... rrArguments) {
			if (m_Condition(rrArguments...)) {
				m_Then(rrArguments...);
			}
		}
	};


	template<typename _Condition, typename _Then, typename _Else>
	struct IfThenElse :
		public Functor
	{
		_Condition m_Condition;
		_Then m_Then;
		_Else m_Else;

		IfThenElse(_Condition &&a_rrCondition, _Then &&a_rrThen, _Else &&a_rrElse)
			:
		m_Condition((_Condition&&)a_rrCondition),
			m_Then((_Then&&)a_rrThen),
			m_Else((_Else&&)a_rrElse) {}

		template<typename... _Arguments>
		inline void operator () (_Arguments&&... rrArguments) {
			if (m_Condition(rrArguments...)) {
				m_Then(rrArguments...);
			} else {
				m_Else(rrArguments...);
			}
		}
	};


	template<typename _Condition>
	struct If :
		public Functor
	{
		_Condition m_Condition;

		If(_Condition &&a_rrCondition)
			:
		m_Condition((_Condition&&)a_rrCondition) {}

		template<typename _Then>
		struct then_ :
			public Functor
		{
			_Condition m_Condition;
			_Then m_Then;

			then_(_Condition &&a_rrCondition, _Then &&a_rrThen)
				:
			m_Condition((_Condition&&)a_rrCondition),
				m_Then((_Then&&)a_rrThen) {}

			template<typename... _Arguments>
			inline void operator () (_Arguments&&... rrArguments) {
				if (m_Condition(rrArguments...)) {
					m_Then(rrArguments...);
				}
			}

			// TODO else
		};

		template<typename _Then>
		inline then_<_Then> operator [] (_Then &&rrThen) && {
			return then_<_Then>(move(m_Condition), (_Then&&)rrThen);
		}
	};


	template<typename _Condition, typename _Body = Null>
	struct While :
		public Functor
	{
		_Condition m_Condition;
		_Body m_Body;

		While(_Condition &&a_rrCondition, _Body &&a_rrBody)
			:
		m_Condition((_Condition&&)a_rrCondition),
			m_Body((_Body&&)a_rrBody) {}

		template<typename... _Arguments>
		inline void operator () (_Arguments&&... Arguments) {
			while (m_Condition(Arguments...)) {
				m_Body(Arguments...);
			}
		}
	};


	template<typename _Condition, typename _Body = Null>
	struct DoWhile :
		public Functor
	{
		_Condition m_Condition;
		_Body m_Body;

		DoWhile(_Condition &&a_rrCondition, _Body &&a_rrBody)
			:
		m_Condition((_Condition&&)a_rrCondition),
			m_Body((_Body&&)a_rrBody) {}

		template<typename... _Arguments>
		inline void operator () (_Arguments&&... Arguments) {
			do {
				m_Body(Arguments...);
			} while (m_Condition(Arguments...));
		}
	};
}


Lambda::Null _0;
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


#define LAMBDA_PREFIX_UNARY_OPERATOR(Operator, FunctorClass) \
	template<typename _Operand, typename _Traits = Lambda::FunctorTraits<_Operand>> \
	inline Lambda::FunctorClass<_Operand> operator Operator (_Operand &&rrOperand) { \
		return Lambda::FunctorClass<_Operand>((_Operand&&)rrOperand); \
	}

LAMBDA_PREFIX_UNARY_OPERATOR(*, Indirection)
LAMBDA_PREFIX_UNARY_OPERATOR(+, UnaryPlus)
LAMBDA_PREFIX_UNARY_OPERATOR(-, UnaryMinus)
LAMBDA_PREFIX_UNARY_OPERATOR(!, LogicalNot)
LAMBDA_PREFIX_UNARY_OPERATOR(~, BitwiseNot)
LAMBDA_PREFIX_UNARY_OPERATOR(++, PreIncrement)
LAMBDA_PREFIX_UNARY_OPERATOR(--, PreDecrement)


#define LAMBDA_BINARY_OPERATOR(Operator, FunctorClass) \
	template<typename _Left, typename _Right, typename _Traits = Lambda::FunctorTraits2<_Left, _Right>> \
	inline Lambda::FunctorClass<_Left, _Right> operator Operator (_Left &&rrLeft, _Right &&rrRight) { \
		return _Traits::template Build<Lambda::FunctorClass<_Left, _Right>>((_Left&&)rrLeft, (_Right&&)rrRight); \
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
//LAMBDA_BINARY_OPERATOR(=, Assignment)
LAMBDA_BINARY_OPERATOR(==, Equals)
LAMBDA_BINARY_OPERATOR(!=, NotEqual)
LAMBDA_BINARY_OPERATOR(<, LessThan)
LAMBDA_BINARY_OPERATOR(>, GreaterThan)
LAMBDA_BINARY_OPERATOR(<=, LessThanOrEqualTo)
LAMBDA_BINARY_OPERATOR(>=, GreaterThanOrEqualTo)


template<typename _Type>
Lambda::Delay<_Type> constant(_Type &&rr) {
	return Lambda::Delay<_Type>((_Type&&)rr);
}

template<typename _Type>
Lambda::Delay<_Type> var(_Type &&rr) {
	return Lambda::Delay<_Type>((_Type&&)rr);
}

template<typename _Condition, typename _Then>
Lambda::IfThen<_Condition, _Then> if_then(_Condition &&rrCondition, _Then &&rrThen) {
	return Lambda::IfThen<_Condition, _Then>((_Condition&&)rrCondition, (_Then&&)rrThen);
}

template<typename _Condition, typename _Then, typename _Else>
Lambda::IfThenElse<_Condition, _Then, _Else> if_then_else(_Condition &&rrCondition, _Then &&rrThen, _Else &&rrElse) {
	return Lambda::IfThenElse<_Condition, _Then, _Else>((_Condition&&)rrCondition, (_Then&&)rrThen, (_Else&&)rrElse);
}

template<typename _Condition>
Lambda::If<_Condition> if_(_Condition &&rrCondition) {
	return Lambda::If<_Condition>((_Condition&&)rrCondition);
}

template<typename _Condition, typename _Body>
Lambda::While<_Condition, _Body> while_loop(_Condition &&rrCondition, _Body &&rrBody = _0) {
	return Lambda::While<_Condition, _Body>((_Condition&&)rrCondition, (_Body&&)rrBody);
}

template<typename _Condition, typename _Body>
Lambda::DoWhile<_Condition, _Body> do_while_loop(_Condition &&rrCondition, _Body &&rrBody = _0) {
	return Lambda::DoWhile<_Condition, _Body>((_Condition&&)rrCondition, (_Body&&)rrBody);
}
