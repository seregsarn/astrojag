#include "astrojag_internal.h"

namespace astrojag {

Logger *Logger::internalLogger = nullptr;

void internalLog(const char *fmt, ...) {
	va_list s;
	static char buf[4096];
	if (Logger::internalLogger == nullptr) return;

	va_start(s, fmt);
	vsnprintf(buf, 4096, fmt, s);
	va_end(s);
	Logger::internalLogger->print("astrojag: %s", buf);
}

void Logger::setInternalLogger(Logger *l) {
    internalLogger = l;
}

}; /* namespace astrojag */
//================================================================

Logger::Logger() : Logger(stdout) { }
Logger::Logger(FILE *fp) {
    int fd = fileno(fp);
    int fd2 = dup(fd);
    logFile = fdopen(fd2, "a+");
    if (internalLogger == nullptr) internalLogger = this;
}
Logger::Logger(const char *filename) : Logger(filename, true) {}
Logger::Logger(const char *filename, bool overwrite) {
    logFile = fopen(filename, overwrite ? "w" : "a");
    if (internalLogger == nullptr) internalLogger = this;
}
Logger::~Logger() {
    fclose(logFile);
    if (this == internalLogger) internalLogger = nullptr;
}

void Logger::print(const char *fmt, ...) {
	va_list s;
	static char buf[4096];

	va_start(s, fmt);
	vsnprintf(buf, 4096, fmt, s);
	va_end(s);

	fprintf(logFile, "%s", buf);
}

void Logger::print(string str) {
    print("%s", str.c_str());
}

void Logger::flush() {
    fflush(logFile);
}