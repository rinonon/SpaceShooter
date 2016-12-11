#pragma once

class File {
public:
	File(const char* filename);
	~File();
	int size() const;
	const char* data() const;
private:
	int mSize;
	char* mData;
};

