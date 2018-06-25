#include <string.h>
#include "strcut.h"


char *strcut(char *string, int fromIndex, int toIndex) {
    if (strlen(string) == 0) return string;
    char *result = strdup(string);
    int stringLength = strlen(string);
    if (toIndex > stringLength + 1) toIndex = strlen(string) + 1;
    int length = toIndex - fromIndex;
    if (length < 0) return "";
    if (length == 0) result[0] = string[fromIndex];
    for (int i = fromIndex; i < toIndex; ++i) {
        result[i - fromIndex] = string[i];
    }
    result[length] = '\0';
    return result;
}