// Logging system
//===================================================
// basic support for logging debug information to a file. the first one that
// gets created, if any, will be used for library error messages too.

class Logger {
protected:
    friend void internalLog(const char *c, ...);
    static Logger *internalLogger;
    FILE *logFile;
public:
    static void setInternalLogger(Logger *l);
    Logger(); // default to stdout
    Logger(FILE *fp);
    Logger(const char *filename);
    Logger(const char *filename, bool overwrite);
    ~Logger();

    void print(const char *fmt, ...);
    void print(std::string str);
    void flush();
};

