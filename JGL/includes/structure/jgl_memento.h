#pragma once

#include "Structure/jgl_data_contener.h"

namespace jgl
{
	class Originator
	{
	private:
		class Caretaker
		{
		private:
			jgl::Array < jgl::Data_contener > _snapshots;

		public:
			Caretaker()
			{

			}

			void push_back(jgl::Data_contener p_snapshot)
			{
				_snapshots.push_back(p_snapshot);
			}

			const jgl::Data_contener&& pop_back()
			{
				if (_snapshots.size() == 0)
					return (jgl::Data_contener());

				const jgl::Data_contener result = _snapshots.last();
				_snapshots.pop_back();
				return (std::move(result));
			}
		};

		Caretaker _caretaker;

		virtual const jgl::Data_contener _on_save() = 0;
		virtual void _on_restore(const jgl::Data_contener&& p_param) = 0;

	public:
		void save()
		{
			_caretaker.push_back(_on_save());
		}
		void restore()
		{
			_on_restore(_caretaker.pop_back());
		}
	};
}