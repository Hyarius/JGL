#pragma once

#include "Structure/jgl_data_contener.h"

namespace jgl
{
	class Originator
	{
	private:

	public:
		virtual const jgl::Data_contener save() = 0;
		virtual void restore(const jgl::Data_contener p_snapshot) = 0;
	};

	template<typename Originator_type>
	class Caretaker
	{
	private:
		jgl::Array < jgl::Data_contener > _snapshots;

	public:
		Caretaker()
		{
			static_assert(std::is_base_of<Originator, Originator_type>::value, "Caretaker can only allow Originator enherenced object");
		}

		void save(Originator_type& p_originator)
		{
			_snapshots.push_back(p_originator.save());
		}

		const jgl::Data_contener pop_front()
		{
			if (_snapshots.size() == 0)
				return (jgl::Data_contener());

			const jgl::Data_contener result = _snapshots.last();
			_snapshots.pop_back();
			return (result);
		}

	};
}