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
        bStart = 0;
        bLen = strlen(b);
        requiredLength += bLen;

        if (bLen > 0 && b[0] == '/') {
            requiredLength--;
            bStart = 1;
            bLen--;
        }
    }

    char* joinedPath = malloc(sizeof(char*) * requiredLength + 1);
    joinedPath[0] = '\0';
    strncat(joinedPath, a, aLen);
    if (b != NULL) {
        strncat(joinedPath, "/", 1);
        strncat(joinedPath, &b[bStart], bLen);
    }

    return joinedPath;
}