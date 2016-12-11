#include "File.h"
#include <fstream>
using namespace std;

File::File(const char* filename) : mSize(0), mData(0) {
	ifstream in(filename, ifstream::binary);
	if (in) {
		in.seekg(0, ifstream::end);
		mSize = static_cast< int >(in.tellg());
		in.seekg(0, ifstream::beg);
		mData = new char[mSize];
		in.read(mData, mSize);
	}
}

File::~File() {
	delete[] mData;
	mData = 0;
}

int File::size() const {
	return mSize;
}

const char* File::data() const {
	return mData;
}
