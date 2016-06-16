// Logging system
//===================================================
// basic support for logging debug information to a file. the first one that
// gets created, if any, will be used for library error messages too.

class ThpLogger {
protected:
    friend void internalLog(const char *c, ...);
    static ThpLogger *internalLogger;
    FILE *logFile;
public:
    static void setInternalLogger(ThpLogger *l);
    ThpLogger(); // default to stdout
    ThpLogger(FILE *fp);
    ThpLogger(const char *filename);
    ThpLogger(const char *filename, bool overwrite);
    ~ThpLogger();

    void print(const char *fmt, ...);
    void print(std::string str);
    void flush();
};

