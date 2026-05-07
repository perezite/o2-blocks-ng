#pragma once
namespace o2
{
	class SdlContext
	{
		// void ensureSubsystemInitialized(int ...);
		static int _instanceCount;
	public:
		SdlContext();
		~SdlContext();
	};
}