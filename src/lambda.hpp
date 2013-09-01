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


	struct Identity :
		public Functor
	{
		template<typename _Type>
		inline _Type &&operator () (_Type &&rrArgument) {
			return Pass(rrArgument);
		}
	};


	template<typename _Left, typename _Right, bool _fLeftBound = IsBound<_Left>::s_f, bool _fRightBound = IsBound<_Right>::s_f>
	struct LeftShift : public Functor {};

	template<typename _Left, typename _Right>
	struct LeftShift<_Left, _Right, false, false> :
		public Functor
	{
		_Left m_Left;
		_Right m_Right;

		LeftShift(_Left &&a_rrLeft, _Right &&a_rrRight)
			:
		m_Left(Pass(a_rrLeft)),
			m_Right(Pass(a_rrRight)) {}

		template<typename _Argument>
		inline auto operator () (_Argument&&) -> decltype(m_Left << m_Right) {
			return m_Left << m_Right;
		}
	};

	template<typename _Left, typename _Right>
	struct LeftShift<_Left, _Right, true, false> :
		public Functor
	{
		_Left m_Left;
		_Right m_Right;

		LeftShift(_Left &&a_rrLeft, _Right &&a_rrRight)
			:
		m_Left(Pass(a_rrLeft)),
			m_Right(Pass(a_rrRight)) {}

		template<typename _Argument>
		inline auto operator () (_Argument &&rrArgument) -> decltype(m_Left << m_Right(Pass(rrArgument))) {
			return m_Left << m_Right(Pass(rrArgument));
		}
	};

	template<typename _Left, typename _Right>
	struct LeftShift<_Left, _Right, false, true> :
		public Functor
	{
		_Left m_Left;
		_Right m_Right;

		LeftShift(_Left &&a_rrLeft, _Right &&a_rrRight)
			:
		m_Left(Pass(a_rrLeft)),
			m_Right(Pass(a_rrRight)) {}

		template<typename _Argument>
		inline auto operator () (_Argument &&rrArgument) -> decltype(m_Left(Pass(rrArgument)) << m_Right) {
			return m_Left(Pass(rrArgument)) << m_Right;
		}
	};

	template<typename _Left, typename _Right>
	struct LeftShift<_Left, _Right, true, true> :
		public Functor
	{
		_Left m_Left;
		_Right m_Right;

		LeftShift(_Left &&a_rrLeft, _Right &&a_rrRight)
			:
		m_Left(Pass(a_rrLeft)),
			m_Right(Pass(a_rrRight)) {}

		template<typename _Argument>
		inline auto operator () (_Argument &&rrArgument) -> decltype(m_Left(Pass(rrArgument)) << m_Right(Pass(rrArgument))) {
			return m_Left(Pass(rrArgument)) << m_Right(Pass(rrArgument));
		}
	};
}


Lambda::Identity _1;


template<typename _Left, typename _Right>
Lambda::LeftShift<_Left, _Right> operator << (_Left &&rrLeft, _Right &&rrRight) {
	return Lambda::LeftShift<_Left, _Right>((_Left&&)rrLeft, (_Right&&)rrRight);
}
