#pragma once

namespace block
{
	class ISteppable
	{
	public:
		bool ignore;
		virtual void onStep() = 0;
	};
}