#pragma once


class TextFileReader
{
public:
	TextFileReader(const BasicString& filePath);
	~TextFileReader();

	void readText(BasicString& outText);

	int fileLength();

private:
	std::ifstream mFile;
};