#pragma once
namespace PirateSimulator
{

//=============================================================================
// INTERFACE IManager
//=============================================================================
class IManager
{
public:
	virtual ~IManager() = default;

public:
	using IdType = const char*;
	static IdType NullId;
};

} // namespace engine
