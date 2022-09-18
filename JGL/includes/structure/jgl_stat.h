#pragma once

#include "jgl_basic_types.h"

namespace jgl
{
	template <typename TType = jgl::Int>
	class TStat
	{
	protected:
		TType _value;
		TType _max_value;
		jgl::Bool _clamped;
		jgl::Float _percent;

		void _calc_percent()
		{
			_percent = static_cast<jgl::Float>(_value) / static_cast<jgl::Float>(_max_value);
			if (_percent > 1)
				_percent = 1;
			if (_percent < 0)
				_percent = 0;
		}

	public:
		jgl::Size_t message_size = sizeof(TType) * 2;

		TStat() : _value(), _max_value(), _clamped(true) {}

		TStat(TType p_value) : _value(p_value), _max_value(p_value), _clamped(true) {}

		TStat(TType p_value, TType p_max_value) : _value(p_value), _max_value(p_max_value), _clamped(true) {}

		TType value() const { return (_value); }
		TType max_value() const { return (_max_value); }
		jgl::Float percent() const { return (_percent); }

		void set_clamped(jgl::Bool p_clamped)
		{
			_clamped = p_clamped;
		}
		jgl::Bool clamped() { return (_clamped); }

		void set_value(TType p_value)
		{
			_value = p_value;
			if (_clamped == true && _value > _max_value)
				_value = _max_value;
			_calc_percent();
		}
		void set_max_value(TType p_value)
		{
			_max_value = p_value;
			if (_clamped == true && _value > _max_value)
				_value = _max_value;
			_calc_percent();
		}
		void add_value(TType p_value)
		{
			_value += p_value;
			if (_clamped == true && _value > _max_value)
				_value = _max_value;
			_calc_percent();
		}
		void add_max_value(TType p_value)
		{
			_max_value += p_value;
			add_value(p_value);
		}
		void remove_value(TType p_value)
		{
			add_value(-p_value);
		}

		void reset()
		{
			set_value(_max_value);
		}


		jgl::Bool operator < (const TType& p_other) const
		{
			return (_value < p_other);
		}
		jgl::Bool operator <= (const TType& p_other) const
		{
			return (_value <= p_other);
		}
		jgl::Bool operator > (const TType& p_other) const
		{
			return (_value > p_other);
		}
		jgl::Bool operator >= (const TType& p_other) const
		{
			return (_value >= p_other);
		}
		jgl::Bool operator == (const TType& p_other) const
		{
			return (_value == p_other);
		}
		jgl::Bool operator != (const TType& p_other) const
		{
			return (_value != p_other);
		}

		friend std::ostream& operator<<(std::ostream& os, const TStat<TType>& p_stat)
		{
			os << p_stat._value << " / " << p_stat._max_value;
			return os;
		}

		template <typename TType = jgl::Int>
		friend woe::Message& operator << (woe::Message& os, const TStat<TType>& p_data)
		{
			os << p_data._value;
			os << p_data._max_value;
			return os;
		}

		template <typename TType = jgl::Int>
		friend woe::Message& operator >> (woe::Message& os, TStat<TType>& p_data)
		{
			os >> p_data._value;
			os >> p_data._max_value;
			p_data._calc_percent();
			return os;
		}
	};

	using Stat = TStat<jgl::Int>;
	using FStat = TStat<jgl::Float>;
}