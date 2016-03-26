#include "thplib_internal.h"

ThpLogger *ThpLogger::internalLogger = nullptr;

void internalLog(const char *fmt, ...) {
	va_list s;
	static char buf[4096];
	if (ThpLogger::internalLogger == nullptr) return;

	va_start(s, fmt);
	vsnprintf(buf, 4096, fmt, s);
	va_end(s);
	ThpLogger::internalLogger->print("thplib: %s", buf);
}

void ThpLogger::setInternalLogger(ThpLogger *l) {
    internalLogger = l;
}

//================================================================

ThpLogger::ThpLogger() : ThpLogger(stdout) { }
ThpLogger::ThpLogger(FILE *fp) {
    int fd = fileno(fp);
    int fd2 = dup(fd);
    logFile = fdopen(fd2, "a+");
    if (internalLogger == nullptr) internalLogger = this;
}
ThpLogger::ThpLogger(const char *filename) : ThpLogger(filename, true) {}
ThpLogger::ThpLogger(const char *filename, bool overwrite) {
    logFile = fopen(filename, overwrite ? "w" : "a");
    if (internalLogger == nullptr) internalLogger = this;
}
ThpLogger::~ThpLogger() {
    fclose(logFile);
    if (this == internalLogger) internalLogger = nullptr;
}

void ThpLogger::print(const char *fmt, ...) {
	va_list s;
	static char buf[4096];

	va_start(s, fmt);
	vsnprintf(buf, 4096, fmt, s);
	va_end(s);

	fprintf(logFile, "%s", buf);
}

void ThpLogger::print(string str) {
    print("%s", str.c_str());
}

void ThpLogger::flush() {
    fflush(logFile);
}