typedef struct {
        size_t (*const funcu)(char *str, int sigval);
        void (*const funcc)(int button);
        const int interval;
        const int signal;
        char curtext[BLOCKLENGTH + DELIMITERLENGTH];
        char prvtext[BLOCKLENGTH];
        size_t length;
} Block;
