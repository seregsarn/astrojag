// logging interface.

class ThpLogger {
protected:
    friend void internalLog(const char *c, ...);
    static ThpLogger *internalLogger;
    FILE *logFile;
public:
    static void setInternalLogger(ThpLogger *l);
    ThpLogger();
    ThpLogger(FILE *fp);
    ThpLogger(const char *filename);
    ThpLogger(const char *filename, bool overwrite);
    ~ThpLogger();

    void print(const char *fmt, ...);
    void print(std::string str);
    void flush();
};
