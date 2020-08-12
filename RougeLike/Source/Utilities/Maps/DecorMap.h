#pragma once

enum class DecorType : Uint32;


class DecorMap : public DataMap<DecorType, StringMap>
{
public:
	void fill(const XMLNode& node) override;
};