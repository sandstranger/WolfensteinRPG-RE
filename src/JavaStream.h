#ifndef __JAVASTREAM_H__
#define __JAVASTREAM_H__

#define LT_RESOURCE 5
#define LT_FILE		6
#define LT_SOUND_RESOURCE	7 // [GEC]
static constexpr const char* dir = "WolfensteinRPG.app";

class Applet;

// ------------------
// InputStream Class
// ------------------

class InputStream
{
private:

public:
	static constexpr int LOADTYPE_RESOURCE = 5;
	static constexpr int LOADTYPE_FILE = 6;

	int field_0x0;
	int field_0x4;
	uint8_t* data;
	int cursor;
	FILE* file;
	int fileSize;
	int field_0x28;

	// Constructor
	InputStream();
	// Destructor
	~InputStream();
	void dispose();	// what IS.~InputStream() was for: the object lives on (and is destroyed once, at its scope's end)

	bool startup();
	bool loadResource(const char* fileName);
	bool loadFile(const char* fileName, int loadType);
	void close();
	uint8_t byteAt(int i);	// the byte i past the cursor, or 0 past the data's end
	int readInt();
	int readShort();
	bool readBoolean();
	uint8_t readByte();
	uint8_t readUnsignedByte();
	int readSignedByte();
	void read(uint8_t* dest, int off, int size);
};

// -------------------
// OutputStream Class
// -------------------

class OutputStream
{
private:

public:

	bool isOpen;
	FILE* file;
	uint8_t* buffer;
	int written;
	uint8_t* writeBuff;
	int field_0x24_;
	int fileSize;
	int flushCount;
	bool noWrite;
	Applet* App;

	// Constructor
	OutputStream();
	// Destructor
	~OutputStream();
	void dispose();	// as InputStream's

	int openFile(const char* fileName, int openMode);
	void close();
	int flush();
	void writeInt(int i);
	void writeShort(int16_t i);
	void writeByte(uint8_t i);
	void writeBoolean(bool b);
	void write(uint8_t* buff, int off, int size);
};

#endif