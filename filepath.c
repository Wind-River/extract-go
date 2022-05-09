#include <string.h>
#include <stdlib.h>

char* join(const char *a, const char *b) {
    int aLen = strlen(a);
    int requiredLength = aLen;
    if (aLen > 0 && a[aLen-1] == '/') {
        requiredLength--;
        aLen--;
    }

    int bStart = 0;
    int bLen = 0;
    if (b != NULL) {
        bLen = strlen(b);
        requiredLength += bLen;

        if (bLen > 0 && b[0] == '/') {
            bStart = 1;
            bLen--;
            requiredLength--;
        }
    }

    char* joinedPath = malloc(sizeof(char*) * requiredLength + 1);
    joinedPath[0] = '\0';
    strncat(joinedPath, a, aLen);
    if (bLen > 0) {
        strncat(joinedPath, "/", 1);
        strncat(joinedPath, &b[bStart], bLen);
    }

    return joinedPath;
}