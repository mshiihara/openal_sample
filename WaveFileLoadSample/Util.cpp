#include "Util.h"

long GetFileSize(FILE * fp) {
	long fileSize = -1;
	if (!fp) { return fileSize; }

	long pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, pos, SEEK_SET);

	return fileSize;
}
