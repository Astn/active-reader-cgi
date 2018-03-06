#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>

int parsearg(const char *pch, const char *argEventName, char *eventName, const size_t szeventName) {
    if (strncmp(pch, argEventName, strlen(argEventName)) == 0 && strlen(pch) > strlen(argEventName) + 1) {
        strncpy(eventName, pch + strlen(argEventName) + 1, szeventName);
        return 1;
    } else {
        return 0;
    }
}

bool inRange(int in, int min, int max){
    if(in >=min && in<=max)return true;
    else return false;
}

int match(const char *string, char *pattern)
{
    int    status;
    regex_t    re;

    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);      /* report error */
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return(0);      /* report error */
    }
    return(1);
}

// ActiveTime=YYYYMMDD-HHMMSS
bool isValidActiveTime(char* activeTime){
    // YYYYMMDD-HHMMSS
    char* pattern = "^[0-9]{4}[01][0-9][0-3][0-9]-[012][0-9][0-6][0-9][0-6][0-9]$";
    if(match(activeTime, pattern) ==1){
        return true;
    }    else {
        return false;
    }

}


int main(int argc, char **argv, char **envp) {

    // print out HTTP headers
    printf("Content-type: text/html\n"); // required as part of HTTP protocol
    printf("\n");

    // print out HTTP content
    // print all variables
    printf("<html>\n"
                   "<head><title>Active Tag Configuration</title></head>\n"
                   "<body>\n");

    printf("ENVIRONMENT\n");
    printf("<pre>");
    int envArgCount = 0;
    while (envp[envArgCount] != NULL) {
        printf("%s\n", envp[envArgCount++]);
    }

    printf("</pre>\n");

    printf("InputArgs\n");


    printf("<pre>\n");
    for (int i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }
    printf("</pre>\n");


    printf("Parsing http query string");

    // parameters we handle:
    /*
    EventName=????????????????  (ASCII)
    Point Name=???????????????? (ASCII)
    LoopID=0..7
    ActiveChannel=0..7
    ActivePower=0..100
    ActiveTime=YYYYMMDD-HHMMSS
    */

    char eventName[100];
    char pointName[100];
    int loopId = 0;
    int activeChannel = 0;
    int activePower = 0;
    char activeTime[50];

    const char *EVENTNAME = "EventName";
    const char *POINTNAME = "PointName";
    const char *LOOPID = "LoopId";
    const char *ACTIVECHANNEL = "ActiveChannel";
    const char *ACTIVEPOWER = "ActivePower";
    const char *ACTIVETIME = "ActiveTime";

    printf("<pre>");
    char *queryString = getenv("QUERY_STRING");
    if (queryString != NULL) {
        printf("queryString contents: \n");
        // as tokens
        char *pch = strtok(queryString, "&");
        while (pch != NULL) {
            printf("Found: %s\n", pch);
            char argValue[100];
            if (parsearg(pch, EVENTNAME, argValue, sizeof(argValue)) == 1) {
                printf("Captured %s as %s\n", EVENTNAME, argValue);
                strncpy(eventName, argValue, sizeof(eventName));
            }
            if (parsearg(pch, POINTNAME, argValue, sizeof(argValue)) == 1) {
                printf("Captured %s as %s\n", POINTNAME, argValue);
                strncpy(pointName, argValue, sizeof(pointName));
            }
            if (parsearg(pch, LOOPID, argValue, sizeof(argValue)) == 1) {
                int tmp = atoi(argValue);
                if(inRange(tmp,0,7)){
                    printf("Captured %s as %s\n", LOOPID, argValue);
                    loopId = tmp;
                }else{
                    printf("<span style=\"color:red;\">%s was out of range. Expected: 0..7</span>\n",LOOPID);
                }
            }
            if (parsearg(pch, ACTIVECHANNEL, argValue, sizeof(argValue)) == 1) {
                int tmp = atoi(argValue);
                if(inRange(tmp,0,7)){
                    printf("Captured %s as %s\n", ACTIVECHANNEL, argValue);
                    activeChannel = tmp;
                }else{
                    printf("<span style=\"color:red;\">%s was out of range. Expected: 0..7</span>\n",ACTIVECHANNEL);
                }
            }
            if (parsearg(pch, ACTIVEPOWER, argValue, sizeof(argValue)) == 1) {
                int tmp = atoi(argValue);
                if(inRange(tmp,0,100)){
                    printf("Captured %s as %s\n", ACTIVEPOWER, argValue);
                    activePower = tmp;
                }else{
                    printf("<span style=\"color:red;\">%s was out of range. Expected: 0..100</span>\n",ACTIVEPOWER);
                }
            }
            if (parsearg(pch, ACTIVETIME, argValue, sizeof(argValue)) == 1) {
                if(isValidActiveTime(argValue)){
                    printf("Captured %s as %s\n", ACTIVETIME, argValue);
                    strncpy(activeTime, argValue, sizeof(activeTime));
                }else{
                    printf("<span style=\"color:red;\">%s was not in a valid format. Expected: YYYYMMDD-HHMMSS</span>\n",ACTIVETIME);
                }
            }

            pch = strtok(NULL, "&");
        }
    }
    printf("</pre>");

    printf("</body>\n"
           "</html>\n");

}
