

#include <memory.h>
#include <string.h>
#include "Scanner.h"




// string handling, wide character


wchar_t* coco_string_create(const wchar_t* value) {
	return coco_string_create(value, 0);
}

wchar_t* coco_string_create(const wchar_t *value, int startIndex) {
	int valueLen = 0;
	int len = 0;

	if (value) {
		valueLen = wcslen(value);
		len = valueLen - startIndex;
	}

	return coco_string_create(value, startIndex, len);
}

wchar_t* coco_string_create(const wchar_t *value, int startIndex, int length) {
	int len = 0;
	wchar_t* data;

	if (value) { len = length; }
	data = new wchar_t[len + 1];
	wcsncpy(data, &(value[startIndex]), len);
	data[len] = 0;

	return data;
}

wchar_t* coco_string_create_upper(const wchar_t* data) {
	if (!data) { return NULL; }

	int dataLen = 0;
	if (data) { dataLen = wcslen(data); }

	wchar_t *newData = new wchar_t[dataLen + 1];

	for (int i = 0; i <= dataLen; i++) {
		if ((L'a' <= data[i]) && (data[i] <= L'z')) {
			newData[i] = data[i] + (L'A' - L'a');
		}
		else { newData[i] = data[i]; }
	}

	newData[dataLen] = L'\0';
	return newData;
}

wchar_t* coco_string_create_lower(const wchar_t* data) {
	if (!data) { return NULL; }
	int dataLen = wcslen(data);
	return coco_string_create_lower(data, 0, dataLen);
}

wchar_t* coco_string_create_lower(const wchar_t* data, int startIndex, int dataLen) {
	if (!data) { return NULL; }

	wchar_t* newData = new wchar_t[dataLen + 1];

	for (int i = 0; i <= dataLen; i++) {
		wchar_t ch = data[startIndex + i];
		if ((L'A' <= ch) && (ch <= L'Z')) {
			newData[i] = ch - (L'A' - L'a');
		}
		else { newData[i] = ch; }
	}
	newData[dataLen] = L'\0';
	return newData;
}

wchar_t* coco_string_create_append(const wchar_t* data1, const wchar_t* data2) {
	wchar_t* data;
	int data1Len = 0;
	int data2Len = 0;

	if (data1) { data1Len = wcslen(data1); }
	if (data2) {data2Len = wcslen(data2); }

	data = new wchar_t[data1Len + data2Len + 1];

	if (data1) { wcscpy(data, data1); }
	if (data2) { wcscpy(data + data1Len, data2); }

	data[data1Len + data2Len] = 0;

	return data;
}

wchar_t* coco_string_create_append(const wchar_t *target, const wchar_t appendix) {
	int targetLen = coco_string_length(target);
	wchar_t* data = new wchar_t[targetLen + 2];
	wcsncpy(data, target, targetLen);
	data[targetLen] = appendix;
	data[targetLen + 1] = 0;
	return data;
}

void coco_string_delete(wchar_t* &data) {
	delete [] data;
	data = NULL;
}

int coco_string_length(const wchar_t* data) {
	if (data) { return wcslen(data); }
	return 0;
}

bool coco_string_endswith(const wchar_t* data, const wchar_t *end) {
	int dataLen = wcslen(data);
	int endLen = wcslen(end);
	return (endLen <= dataLen) && (wcscmp(data + dataLen - endLen, end) == 0);
}

int coco_string_indexof(const wchar_t* data, const wchar_t value) {
	const wchar_t* chr = wcschr(data, value);

	if (chr) { return (chr-data); }
	return -1;
}

int coco_string_lastindexof(const wchar_t* data, const wchar_t value) {
	const wchar_t* chr = wcsrchr(data, value);

	if (chr) { return (chr-data); }
	return -1;
}

void coco_string_merge(wchar_t* &target, const wchar_t* appendix) {
	if (!appendix) { return; }
	wchar_t* data = coco_string_create_append(target, appendix);
	delete [] target;
	target = data;
}

bool coco_string_equal(const wchar_t* data1, const wchar_t* data2) {
	return wcscmp( data1, data2 ) == 0;
}

int coco_string_compareto(const wchar_t* data1, const wchar_t* data2) {
	return wcscmp(data1, data2);
}

int coco_string_hash(const wchar_t *data) {
	int h = 0;
	if (!data) { return 0; }
	while (*data != 0) {
		h = (h * 7) ^ *data;
		++data;
	}
	if (h < 0) { h = -h; }
	return h;
}

// string handling, ascii character

wchar_t* coco_string_create(const char* value) {
	int len = 0;
	if (value) { len = strlen(value); }
	wchar_t* data = new wchar_t[len + 1];
	for (int i = 0; i < len; ++i) { data[i] = (wchar_t) value[i]; }
	data[len] = 0;
	return data;
}

char* coco_string_create_char(const wchar_t *value) {
	int len = coco_string_length(value);
	char *res = new char[len + 1];
	for (int i = 0; i < len; ++i) { res[i] = (char) value[i]; }
	res[len] = 0;
	return res;
}

void coco_string_delete(char* &data) {
	delete [] data;
	data = NULL;
}


Token::Token() {
	kind = 0;
	pos  = 0;
	col  = 0;
	line = 0;
	val  = NULL;
	next = NULL;
}

Token::~Token() {
	coco_string_delete(val);
}

Buffer::Buffer(FILE* s, bool isUserStream) {
// ensure binary read on windows
#if _MSC_VER >= 1300
	_setmode(_fileno(s), _O_BINARY);
#endif
	stream = s; this->isUserStream = isUserStream;
	if (CanSeek()) {
		fseek(s, 0, SEEK_END);
		fileLen = ftell(s);
		fseek(s, 0, SEEK_SET);
		bufLen = (fileLen < COCO_MAX_BUFFER_LENGTH) ? fileLen : COCO_MAX_BUFFER_LENGTH;
		bufStart = INT_MAX; // nothing in the buffer so far
	} else {
		fileLen = bufLen = bufStart = 0;
	}
	bufCapacity = (bufLen>0) ? bufLen : COCO_MIN_BUFFER_LENGTH;
	buf = new unsigned char[bufCapacity];	
	if (fileLen > 0) SetPos(0);          // setup  buffer to position 0 (start)
	else bufPos = 0; // index 0 is already after the file, thus Pos = 0 is invalid
	if (bufLen == fileLen && CanSeek()) Close();
}

Buffer::Buffer(Buffer *b) {
	buf = b->buf;
	bufCapacity = b->bufCapacity;
	b->buf = NULL;
	bufStart = b->bufStart;
	bufLen = b->bufLen;
	fileLen = b->fileLen;
	bufPos = b->bufPos;
	stream = b->stream;
	b->stream = NULL;
	isUserStream = b->isUserStream;
}

Buffer::Buffer(const unsigned char* buf, int len) {
	this->buf = new unsigned char[len];
	memcpy(this->buf, buf, len*sizeof(unsigned char));
	bufStart = 0;
	bufCapacity = bufLen = len;
	fileLen = len;
	bufPos = 0;
	stream = NULL;
}

Buffer::~Buffer() {
	Close(); 
	if (buf != NULL) {
		delete [] buf;
		buf = NULL;
	}
}

void Buffer::Close() {
	if (!isUserStream && stream != NULL) {
		fclose(stream);
		stream = NULL;
	}
}

int Buffer::Read() {
	if (bufPos < bufLen) {
		return buf[bufPos++];
	} else if (GetPos() < fileLen) {
		SetPos(GetPos()); // shift buffer start to Pos
		return buf[bufPos++];
	} else if ((stream != NULL) && !CanSeek() && (ReadNextStreamChunk() > 0)) {
		return buf[bufPos++];
	} else {
		return EoF;
	}
}

int Buffer::Peek() {
	int curPos = GetPos();
	int ch = Read();
	SetPos(curPos);
	return ch;
}

// beg .. begin, zero-based, inclusive, in byte
// end .. end, zero-based, exclusive, in byte
wchar_t* Buffer::GetString(int beg, int end) {
	int len = 0;
	wchar_t *buf = new wchar_t[end - beg];
	int oldPos = GetPos();
	SetPos(beg);
	while (GetPos() < end) buf[len++] = (wchar_t) Read();
	SetPos(oldPos);
	wchar_t *res = coco_string_create(buf, 0, len);
	coco_string_delete(buf);
	return res;
}

int Buffer::GetPos() {
	return bufPos + bufStart;
}

void Buffer::SetPos(int value) {
	if ((value >= fileLen) && (stream != NULL) && !CanSeek()) {
		// Wanted position is after buffer and the stream
		// is not seek-able e.g. network or console,
		// thus we have to read the stream manually till
		// the wanted position is in sight.
		while ((value >= fileLen) && (ReadNextStreamChunk() > 0));
	}

	if ((value < 0) || (value > fileLen)) {
		wprintf(L"--- buffer out of bounds access, position: %d\n", value);
		exit(1);
	}

	if ((value >= bufStart) && (value < (bufStart + bufLen))) { // already in buffer
		bufPos = value - bufStart;
	} else if (stream != NULL) { // must be swapped in
		fseek(stream, value, SEEK_SET);
		bufLen = fread(buf, sizeof(unsigned char), bufCapacity, stream);
		bufStart = value; bufPos = 0;
	} else {
		bufPos = fileLen - bufStart; // make Pos return fileLen
	}
}

// Read the next chunk of bytes from the stream, increases the buffer
// if needed and updates the fields fileLen and bufLen.
// Returns the number of bytes read.
int Buffer::ReadNextStreamChunk() {
	int free = bufCapacity - bufLen;
	if (free == 0) {
		// in the case of a growing input stream
		// we can neither seek in the stream, nor can we
		// foresee the maximum length, thus we must adapt
		// the buffer size on demand.
		bufCapacity = bufLen * 2;
		unsigned char *newBuf = new unsigned char[bufCapacity];
		memcpy(newBuf, buf, bufLen*sizeof(unsigned char));
		delete [] buf;
		buf = newBuf;
		free = bufLen;
	}
	int read = fread(buf + bufLen, sizeof(unsigned char), free, stream);
	if (read > 0) {
		fileLen = bufLen = (bufLen + read);
		return read;
	}
	// end of stream reached
	return 0;
}

bool Buffer::CanSeek() {
	return (stream != NULL) && (ftell(stream) != -1);
}

int UTF8Buffer::Read() {
	int ch;
	do {
		ch = Buffer::Read();
		// until we find a utf8 start (0xxxxxxx or 11xxxxxx)
	} while ((ch >= 128) && ((ch & 0xC0) != 0xC0) && (ch != EoF));
	if (ch < 128 || ch == EoF) {
		// nothing to do, first 127 chars are the same in ascii and utf8
		// 0xxxxxxx or end of file character
	} else if ((ch & 0xF0) == 0xF0) {
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		int c1 = ch & 0x07; ch = Buffer::Read();
		int c2 = ch & 0x3F; ch = Buffer::Read();
		int c3 = ch & 0x3F; ch = Buffer::Read();
		int c4 = ch & 0x3F;
		ch = (((((c1 << 6) | c2) << 6) | c3) << 6) | c4;
	} else if ((ch & 0xE0) == 0xE0) {
		// 1110xxxx 10xxxxxx 10xxxxxx
		int c1 = ch & 0x0F; ch = Buffer::Read();
		int c2 = ch & 0x3F; ch = Buffer::Read();
		int c3 = ch & 0x3F;
		ch = (((c1 << 6) | c2) << 6) | c3;
	} else if ((ch & 0xC0) == 0xC0) {
		// 110xxxxx 10xxxxxx
		int c1 = ch & 0x1F; ch = Buffer::Read();
		int c2 = ch & 0x3F;
		ch = (c1 << 6) | c2;
	}
	return ch;
}

Scanner::Scanner(const unsigned char* buf, int len) {
	buffer = new Buffer(buf, len);
	Init();
}

Scanner::Scanner(const wchar_t* fileName) {
	FILE* stream;
	char *chFileName = coco_string_create_char(fileName);
	if ((stream = fopen(chFileName, "rb")) == NULL) {
		wprintf(L"--- Cannot open file %ls\n", fileName);
		exit(1);
	}
	coco_string_delete(chFileName);
	buffer = new Buffer(stream, false);
	Init();
}

Scanner::Scanner(FILE* s) {
	buffer = new Buffer(s, true);
	Init();
}

Scanner::~Scanner() {
	char* cur = (char*) firstHeap;

	while(cur != NULL) {
		cur = *(char**) (cur + COCO_HEAP_BLOCK_SIZE);
		free(firstHeap);
		firstHeap = cur;
	}
	delete [] tval;
	delete buffer;
}

void Scanner::Init() {
	EOL    = '\n';
	eofSym = 0;
	maxT = 55;
	noSym = 55;
	int i;
	for (i = 65; i <= 90; ++i) start.set(i, 5);
	for (i = 48; i <= 57; ++i) start.set(i, 95);
	start.set(95, 1);
	start.set(35, 3);
	start.set(36, 6);
	start.set(34, 10);
	start.set(116, 13);
	start.set(102, 16);
	start.set(99, 21);
	start.set(112, 96);
	start.set(114, 41);
	start.set(115, 47);
	start.set(105, 97);
	start.set(100, 55);
	start.set(98, 61);
	start.set(110, 65);
	start.set(118, 68);
	start.set(63, 72);
	start.set(101, 98);
	start.set(58, 99);
	start.set(119, 75);
	start.set(64, 79);
	start.set(45, 120);
	start.set(109, 81);
	start.set(62, 121);
	start.set(60, 122);
	start.set(43, 123);
	start.set(61, 124);
	start.set(47, 125);
	start.set(91, 105);
	start.set(93, 106);
	start.set(59, 107);
	start.set(40, 108);
	start.set(41, 109);
	start.set(123, 110);
	start.set(125, 111);
	start.set(44, 112);
	start.set(94, 113);
	start.set(38, 114);
	start.set(124, 115);
	start.set(126, 116);
	start.set(46, 117);
	start.set(42, 118);
	start.set(37, 119);
		start.set(Buffer::EoF, -1);


	tvalLength = 128;
	tval = new wchar_t[tvalLength]; // text of current token

	// COCO_HEAP_BLOCK_SIZE byte heap + pointer to next heap block
	heap = malloc(COCO_HEAP_BLOCK_SIZE + sizeof(void*));
	firstHeap = heap;
	heapEnd = (void**) (((char*) heap) + COCO_HEAP_BLOCK_SIZE);
	*heapEnd = 0;
	heapTop = heap;
	if (sizeof(Token) > COCO_HEAP_BLOCK_SIZE) {
		wprintf(L"--- Too small COCO_HEAP_BLOCK_SIZE\n");
		exit(1);
	}

	pos = -1; line = 1; col = 0; charPos = -1;
	oldEols = 0;
	NextCh();
	if (ch == 0xEF) { // check optional byte order mark for UTF-8
		NextCh(); int ch1 = ch;
		NextCh(); int ch2 = ch;
		if (ch1 != 0xBB || ch2 != 0xBF) {
			wprintf(L"Illegal byte order mark at start of file");
			exit(1);
		}
		Buffer *oldBuf = buffer;
		buffer = new UTF8Buffer(buffer); col = 0; charPos = -1;
		delete oldBuf; oldBuf = NULL;
		NextCh();
	}


	pt = tokens = CreateToken(); // first token is a dummy
}

void Scanner::NextCh() {
	if (oldEols > 0) { ch = EOL; oldEols--; }
	else {
		pos = buffer->GetPos();
		// buffer reads unicode chars, if UTF8 has been detected
		ch = buffer->Read(); col++; charPos++;
		// replace isolated '\r' by '\n' in order to make
		// eol handling uniform across Windows, Unix and Mac
		if (ch == L'\r' && buffer->Peek() != L'\n') ch = EOL;
		if (ch == EOL) { line++; col = 0; }
	}

}

void Scanner::AddCh() {
	if (tlen >= tvalLength) {
		tvalLength *= 2;
		wchar_t *newBuf = new wchar_t[tvalLength];
		memcpy(newBuf, tval, tlen*sizeof(wchar_t));
		delete [] tval;
		tval = newBuf;
	}
	if (ch != Buffer::EoF) {
		tval[tlen++] = ch;
		NextCh();
	}
}



void Scanner::CreateHeapBlock() {
	void* newHeap;
	char* cur = (char*) firstHeap;

	while(((char*) tokens < cur) || ((char*) tokens > (cur + COCO_HEAP_BLOCK_SIZE))) {
		cur = *((char**) (cur + COCO_HEAP_BLOCK_SIZE));
		free(firstHeap);
		firstHeap = cur;
	}

	// COCO_HEAP_BLOCK_SIZE byte heap + pointer to next heap block
	newHeap = malloc(COCO_HEAP_BLOCK_SIZE + sizeof(void*));
	*heapEnd = newHeap;
	heapEnd = (void**) (((char*) newHeap) + COCO_HEAP_BLOCK_SIZE);
	*heapEnd = 0;
	heap = newHeap;
	heapTop = heap;
}

Token* Scanner::CreateToken() {
	Token *t;
	if (((char*) heapTop + (int) sizeof(Token)) >= (char*) heapEnd) {
		CreateHeapBlock();
	}
	t = (Token*) heapTop;
	heapTop = (void*) ((char*) heapTop + sizeof(Token));
	t->val = NULL;
	t->next = NULL;
	return t;
}

void Scanner::AppendVal(Token *t) {
	int reqMem = (tlen + 1) * sizeof(wchar_t);
	if (((char*) heapTop + reqMem) >= (char*) heapEnd) {
		if (reqMem > COCO_HEAP_BLOCK_SIZE) {
			wprintf(L"--- Too long token value\n");
			exit(1);
		}
		CreateHeapBlock();
	}
	t->val = (wchar_t*) heapTop;
	heapTop = (void*) ((char*) heapTop + reqMem);

	wcsncpy(t->val, tval, tlen);
	t->val[tlen] = L'\0';
}

Token* Scanner::NextToken() {
	while (ch == ' ' ||
			(ch >= 9 && ch <= 10) || ch == 13
	) NextCh();

	int recKind = noSym;
	int recEnd = pos;
	t = CreateToken();
	t->pos = pos; t->col = col; t->line = line; t->charPos = charPos;
	int state = start.state(ch);
	tlen = 0; AddCh();

	switch (state) {
		case -1: { t->kind = eofSym; break; } // NextCh already done
		case 0: {
			case_0:
			if (recKind != noSym) {
				tlen = recEnd - t->pos;
				SetScannerBehindT();
			}
			t->kind = recKind; break;
		} // NextCh already done
		case 1:
			if ((ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z')) {AddCh(); goto case_2;}
			else {goto case_0;}
		case 2:
			case_2:
			recEnd = pos; recKind = 1;
			if ((ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z')) {AddCh(); goto case_2;}
			else {t->kind = 1; break;}
		case 3:
			if ((ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z')) {AddCh(); goto case_4;}
			else {goto case_0;}
		case 4:
			case_4:
			recEnd = pos; recKind = 2;
			if ((ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z')) {AddCh(); goto case_4;}
			else {t->kind = 2; break;}
		case 5:
			case_5:
			recEnd = pos; recKind = 3;
			if ((ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z')) {AddCh(); goto case_5;}
			else {t->kind = 3; break;}
		case 6:
			if ((ch >= L'A' && ch <= L'Z')) {AddCh(); goto case_7;}
			else {goto case_0;}
		case 7:
			case_7:
			recEnd = pos; recKind = 4;
			if ((ch >= L'0' && ch <= L'9') || (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z')) {AddCh(); goto case_7;}
			else {t->kind = 4; break;}
		case 8:
			case_8:
			if ((ch >= L'0' && ch <= L'9')) {AddCh(); goto case_9;}
			else {goto case_0;}
		case 9:
			case_9:
			recEnd = pos; recKind = 6;
			if ((ch >= L'0' && ch <= L'9')) {AddCh(); goto case_9;}
			else {t->kind = 6; break;}
		case 10:
			if (ch <= 8 || (ch >= 11 && ch <= 12) || (ch >= 14 && ch <= L'!') || (ch >= L'#' && ch <= 65535)) {AddCh(); goto case_11;}
			else {goto case_0;}
		case 11:
			case_11:
			if (ch <= 8 || (ch >= 11 && ch <= 12) || (ch >= 14 && ch <= L'!') || (ch >= L'#' && ch <= 65535)) {AddCh(); goto case_11;}
			else if (ch == L'"') {AddCh(); goto case_12;}
			else {goto case_0;}
		case 12:
			case_12:
			{t->kind = 7; break;}
		case 13:
			if (ch == L'r') {AddCh(); goto case_14;}
			else {goto case_0;}
		case 14:
			case_14:
			if (ch == L'u') {AddCh(); goto case_15;}
			else {goto case_0;}
		case 15:
			case_15:
			if (ch == L'e') {AddCh(); goto case_20;}
			else {goto case_0;}
		case 16:
			if (ch == L'a') {AddCh(); goto case_17;}
			else {goto case_0;}
		case 17:
			case_17:
			if (ch == L'l') {AddCh(); goto case_18;}
			else {goto case_0;}
		case 18:
			case_18:
			if (ch == L's') {AddCh(); goto case_19;}
			else {goto case_0;}
		case 19:
			case_19:
			if (ch == L'e') {AddCh(); goto case_20;}
			else {goto case_0;}
		case 20:
			case_20:
			{t->kind = 8; break;}
		case 21:
			if (ch == L'l') {AddCh(); goto case_22;}
			else {goto case_0;}
		case 22:
			case_22:
			if (ch == L'a') {AddCh(); goto case_23;}
			else {goto case_0;}
		case 23:
			case_23:
			if (ch == L's') {AddCh(); goto case_24;}
			else {goto case_0;}
		case 24:
			case_24:
			if (ch == L's') {AddCh(); goto case_25;}
			else {goto case_0;}
		case 25:
			case_25:
			{t->kind = 9; break;}
		case 26:
			case_26:
			if (ch == L'b') {AddCh(); goto case_27;}
			else {goto case_0;}
		case 27:
			case_27:
			if (ch == L'l') {AddCh(); goto case_28;}
			else {goto case_0;}
		case 28:
			case_28:
			if (ch == L'i') {AddCh(); goto case_29;}
			else {goto case_0;}
		case 29:
			case_29:
			if (ch == L'c') {AddCh(); goto case_30;}
			else {goto case_0;}
		case 30:
			case_30:
			{t->kind = 10; break;}
		case 31:
			case_31:
			if (ch == L'v') {AddCh(); goto case_32;}
			else {goto case_0;}
		case 32:
			case_32:
			if (ch == L'a') {AddCh(); goto case_33;}
			else {goto case_0;}
		case 33:
			case_33:
			if (ch == L't') {AddCh(); goto case_34;}
			else {goto case_0;}
		case 34:
			case_34:
			if (ch == L'e') {AddCh(); goto case_35;}
			else {goto case_0;}
		case 35:
			case_35:
			{t->kind = 11; break;}
		case 36:
			case_36:
			if (ch == L't') {AddCh(); goto case_37;}
			else {goto case_0;}
		case 37:
			case_37:
			if (ch == L'e') {AddCh(); goto case_38;}
			else {goto case_0;}
		case 38:
			case_38:
			if (ch == L'c') {AddCh(); goto case_39;}
			else {goto case_0;}
		case 39:
			case_39:
			if (ch == L't') {AddCh(); goto case_40;}
			else {goto case_0;}
		case 40:
			case_40:
			{t->kind = 12; break;}
		case 41:
			if (ch == L'e') {AddCh(); goto case_42;}
			else {goto case_0;}
		case 42:
			case_42:
			if (ch == L't') {AddCh(); goto case_43;}
			else {goto case_0;}
		case 43:
			case_43:
			if (ch == L'u') {AddCh(); goto case_44;}
			else {goto case_0;}
		case 44:
			case_44:
			if (ch == L'r') {AddCh(); goto case_45;}
			else {goto case_0;}
		case 45:
			case_45:
			if (ch == L'n') {AddCh(); goto case_46;}
			else {goto case_0;}
		case 46:
			case_46:
			{t->kind = 13; break;}
		case 47:
			if (ch == L't') {AddCh(); goto case_48;}
			else {goto case_0;}
		case 48:
			case_48:
			if (ch == L'r') {AddCh(); goto case_49;}
			else {goto case_0;}
		case 49:
			case_49:
			if (ch == L'i') {AddCh(); goto case_50;}
			else {goto case_0;}
		case 50:
			case_50:
			if (ch == L'n') {AddCh(); goto case_51;}
			else {goto case_0;}
		case 51:
			case_51:
			if (ch == L'g') {AddCh(); goto case_52;}
			else {goto case_0;}
		case 52:
			case_52:
			{t->kind = 14; break;}
		case 53:
			case_53:
			if (ch == L't') {AddCh(); goto case_54;}
			else {goto case_0;}
		case 54:
			case_54:
			{t->kind = 15; break;}
		case 55:
			if (ch == L'o') {AddCh(); goto case_56;}
			else {goto case_0;}
		case 56:
			case_56:
			if (ch == L'u') {AddCh(); goto case_57;}
			else {goto case_0;}
		case 57:
			case_57:
			if (ch == L'b') {AddCh(); goto case_58;}
			else {goto case_0;}
		case 58:
			case_58:
			if (ch == L'l') {AddCh(); goto case_59;}
			else {goto case_0;}
		case 59:
			case_59:
			if (ch == L'e') {AddCh(); goto case_60;}
			else {goto case_0;}
		case 60:
			case_60:
			{t->kind = 16; break;}
		case 61:
			if (ch == L'o') {AddCh(); goto case_62;}
			else {goto case_0;}
		case 62:
			case_62:
			if (ch == L'o') {AddCh(); goto case_63;}
			else {goto case_0;}
		case 63:
			case_63:
			if (ch == L'l') {AddCh(); goto case_64;}
			else {goto case_0;}
		case 64:
			case_64:
			{t->kind = 17; break;}
		case 65:
			if (ch == L'e') {AddCh(); goto case_66;}
			else {goto case_0;}
		case 66:
			case_66:
			if (ch == L'w') {AddCh(); goto case_67;}
			else {goto case_0;}
		case 67:
			case_67:
			{t->kind = 18; break;}
		case 68:
			if (ch == L'o') {AddCh(); goto case_69;}
			else {goto case_0;}
		case 69:
			case_69:
			if (ch == L'i') {AddCh(); goto case_70;}
			else {goto case_0;}
		case 70:
			case_70:
			if (ch == L'd') {AddCh(); goto case_71;}
			else {goto case_0;}
		case 71:
			case_71:
			{t->kind = 19; break;}
		case 72:
			case_72:
			{t->kind = 20; break;}
		case 73:
			case_73:
			if (ch == L'f') {AddCh(); goto case_74;}
			else {goto case_0;}
		case 74:
			case_74:
			{t->kind = 22; break;}
		case 75:
			if (ch == L'h') {AddCh(); goto case_76;}
			else {goto case_0;}
		case 76:
			case_76:
			if (ch == L'i') {AddCh(); goto case_77;}
			else {goto case_0;}
		case 77:
			case_77:
			if (ch == L'l') {AddCh(); goto case_78;}
			else {goto case_0;}
		case 78:
			case_78:
			if (ch == L'e') {AddCh(); goto case_79;}
			else {goto case_0;}
		case 79:
			case_79:
			{t->kind = 23; break;}
		case 80:
			case_80:
			{t->kind = 24; break;}
		case 81:
			if (ch == L'a') {AddCh(); goto case_82;}
			else {goto case_0;}
		case 82:
			case_82:
			if (ch == L'i') {AddCh(); goto case_83;}
			else {goto case_0;}
		case 83:
			case_83:
			if (ch == L'n') {AddCh(); goto case_84;}
			else {goto case_0;}
		case 84:
			case_84:
			{t->kind = 25; break;}
		case 85:
			case_85:
			{t->kind = 26; break;}
		case 86:
			case_86:
			{t->kind = 27; break;}
		case 87:
			case_87:
			{t->kind = 28; break;}
		case 88:
			case_88:
			{t->kind = 29; break;}
		case 89:
			case_89:
			{t->kind = 30; break;}
		case 90:
			case_90:
			{t->kind = 31; break;}
		case 91:
			case_91:
			{t->kind = 32; break;}
		case 92:
			case_92:
			{t->kind = 33; break;}
		case 93:
			case_93:
			if (ch <= L')' || (ch >= L'+' && ch <= 65535)) {AddCh(); goto case_93;}
			else if (ch == L'*') {AddCh(); goto case_100;}
			else {goto case_0;}
		case 94:
			case_94:
			{t->kind = 56; break;}
		case 95:
			case_95:
			recEnd = pos; recKind = 5;
			if ((ch >= L'0' && ch <= L'9')) {AddCh(); goto case_95;}
			else if (ch == L'.') {AddCh(); goto case_8;}
			else {t->kind = 5; break;}
		case 96:
			if (ch == L'u') {AddCh(); goto case_26;}
			else if (ch == L'r') {AddCh(); goto case_101;}
			else {goto case_0;}
		case 97:
			if (ch == L'n') {AddCh(); goto case_53;}
			else if (ch == L'f') {AddCh(); goto case_72;}
			else {goto case_0;}
		case 98:
			if (ch == L'l') {AddCh(); goto case_102;}
			else {goto case_0;}
		case 99:
			recEnd = pos; recKind = 21;
			if (ch == L'?') {AddCh(); goto case_74;}
			else {t->kind = 21; break;}
		case 100:
			case_100:
			if (ch <= L')' || (ch >= L'+' && ch <= L'.') || (ch >= L'0' && ch <= 65535)) {AddCh(); goto case_93;}
			else if (ch == L'/') {AddCh(); goto case_94;}
			else if (ch == L'*') {AddCh(); goto case_100;}
			else {goto case_0;}
		case 101:
			case_101:
			if (ch == L'i') {AddCh(); goto case_31;}
			else if (ch == L'o') {AddCh(); goto case_36;}
			else {goto case_0;}
		case 102:
			case_102:
			if (ch == L's') {AddCh(); goto case_103;}
			else {goto case_0;}
		case 103:
			case_103:
			if (ch == L'e') {AddCh(); goto case_104;}
			else {goto case_0;}
		case 104:
			case_104:
			recEnd = pos; recKind = 21;
			if (ch == L'i') {AddCh(); goto case_73;}
			else {t->kind = 21; break;}
		case 105:
			{t->kind = 34; break;}
		case 106:
			{t->kind = 35; break;}
		case 107:
			{t->kind = 36; break;}
		case 108:
			{t->kind = 38; break;}
		case 109:
			{t->kind = 39; break;}
		case 110:
			{t->kind = 40; break;}
		case 111:
			{t->kind = 41; break;}
		case 112:
			{t->kind = 42; break;}
		case 113:
			{t->kind = 43; break;}
		case 114:
			{t->kind = 46; break;}
		case 115:
			{t->kind = 47; break;}
		case 116:
			{t->kind = 50; break;}
		case 117:
			{t->kind = 51; break;}
		case 118:
			{t->kind = 52; break;}
		case 119:
			{t->kind = 54; break;}
		case 120:
			recEnd = pos; recKind = 45;
			if (ch == L'>') {AddCh(); goto case_80;}
			else if (ch == L'-') {AddCh(); goto case_88;}
			else {t->kind = 45; break;}
		case 121:
			recEnd = pos; recKind = 49;
			if (ch == L'>') {AddCh(); goto case_85;}
			else if (ch == L'=') {AddCh(); goto case_89;}
			else {t->kind = 49; break;}
		case 122:
			recEnd = pos; recKind = 48;
			if (ch == L'<') {AddCh(); goto case_86;}
			else if (ch == L'=') {AddCh(); goto case_90;}
			else if (ch == L'>') {AddCh(); goto case_91;}
			else {t->kind = 48; break;}
		case 123:
			recEnd = pos; recKind = 44;
			if (ch == L'+') {AddCh(); goto case_87;}
			else {t->kind = 44; break;}
		case 124:
			recEnd = pos; recKind = 37;
			if (ch == L'=') {AddCh(); goto case_92;}
			else {t->kind = 37; break;}
		case 125:
			recEnd = pos; recKind = 53;
			if (ch == L'*') {AddCh(); goto case_93;}
			else {t->kind = 53; break;}

	}
	AppendVal(t);
	return t;
}

void Scanner::SetScannerBehindT() {
	buffer->SetPos(t->pos);
	NextCh();
	line = t->line; col = t->col; charPos = t->charPos;
	for (int i = 0; i < tlen; i++) NextCh();
}

// get the next token (possibly a token already seen during peeking)
Token* Scanner::Scan() {
	if (tokens->next == NULL) {
		return pt = tokens = NextToken();
	} else {
		pt = tokens = tokens->next;
		return tokens;
	}
}

// peek for the next token, ignore pragmas
Token* Scanner::Peek() {
	do {
		if (pt->next == NULL) {
			pt->next = NextToken();
		}
		pt = pt->next;
	} while (pt->kind > maxT); // skip pragmas

	return pt;
}

// make sure that peeking starts at the current scan position
void Scanner::ResetPeek() {
	pt = tokens;
}


