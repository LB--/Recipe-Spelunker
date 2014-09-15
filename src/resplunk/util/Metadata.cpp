#include "resplunk/util/Metadata.hpp"

namespace resplunk
{
	namespace util
	{
		struct Listener final
		: private event::Processor<Metadata::SerializeEvent>
		{
		private:
			virtual void process(Metadata::SerializeEvent &e) const noexcept override final
			{
				auto &data = e.data_for(e.instance());
				data["m"] = ObjectValue{*t};
			}
		} listener;
	}
}
