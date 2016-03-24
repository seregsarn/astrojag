// logging interface.

class ThpLogger {
protected:
    static ThpLogger *internalLogger;
    FILE *logFile;
public:
    static void setInternalLogger(ThpLogger *l);
    ThpLogger();
    ThpLogger(FILE *fp);
    ThpLogger(char *filename);
    ThpLogger(char *filename, bool overwrite);
    ~ThpLogger();

    void print(const char *fmt, ...);
    void print(string str);
};