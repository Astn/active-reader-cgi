//
// Created by astn on 3/6/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include <errno.h>

#include "settings.h"

int parsearg(const char *pch, const char *argEventName, char *eventName, const size_t szeventName) {
    if (strncmp(pch, argEventName, strlen(argEventName)) == 0 && strlen(pch) > strlen(argEventName) + 1) {
        strncpy(eventName, pch + strlen(argEventName) + 1, szeventName);
        return 1;
    } else {
        return 0;
    }
}

bool inRange(int in, int min, int max) {
    if(in >=min && in<=max)return true;
    else return false;
}

int match(const char *string, char *pattern) {
    int    status;
    regex_t    re;
    char		buffer[100];

    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);      /* report error */
    }
    if ((status = regexec(&re, string, (size_t) 0, NULL, 0)) != 0)  {
        regerror(status, &re, buffer, 100);
        printf("<!--\n");
        printf("regcomp() failed with '%s - %d'\n", buffer, errno);
        printf("\nPattern: %s", pattern);
        printf("\nString: %s %i", string, (int)strlen(string));
        printf("-->\n");
    }
    regfree(&re);
    if (status != 0) {
        return(0);      /* report error */
    }
    return(1);
}

bool isValidActiveTime(char *activeTime) {
    // YYYY/MM/DD HH:MM:SS
    char* pattern = "^[0-9]{4}/[01][0-9]/[0-3][0-9] [012][0-9]:[0-6][0-9]:[0-6][0-9]";
    if(match(activeTime, pattern) ==1){
        return true;
    }    else {
        return false;
    }

}


void SettingsError_AddMessage(struct SettingsError_t *e, char *msg) {
    int i = 0;
    while (i < lenErrorMessages) {
        if(e->messages[i] == NULL){
            e->messages[i] = msg;
            break;
        }
        ++i;
    }

}

unsigned long min(unsigned long a, unsigned long b){
    if (a > b){
        return b;
    }
    else {
        return a;
    }

}

void SettingsParsePair(struct Settings_t *s, char *pch) {
    char argValue[100];
    if (parsearg(pch, EVENTNAME, argValue, sizeof(argValue)) == 1) {
        s->found.eventName = true;
        if(strlen(argValue) > 2 && strlen(argValue) < lenStrName){
            s->parsed.eventName = true;
            unsigned long nl = strchr(argValue, '\n') - argValue;
            strncpy(s->values.eventName, argValue, min(lenStrName,nl));
        } else {
            s->error.eventName = true;
            SettingsError_AddMessage(&s->error,MSG_PARSEERROR_EVENTNAME);
        }
    }
    if (parsearg(pch, POINTNAME, argValue, sizeof(argValue)) == 1) {
        s->found.pointName = true;
        if(strlen(argValue) > 2 && strlen(argValue) < lenStrName) {
            s->parsed.pointName = true;
            unsigned long nl = strchr(argValue, '\n') - argValue;
            strncpy(s->values.pointName, argValue, min(lenStrName,nl));
        } else {
            s->error.pointName = true;
            SettingsError_AddMessage(&s->error,MSG_PARSEERROR_POINTNAME);
        }
    }
    if (parsearg(pch, LOOPID, argValue, sizeof(argValue)) == 1) {
        int tmp = atoi(argValue);
        s->found.loopId = true;
        if(inRange(tmp,0,7)){
            s->parsed.loopId = true;
            s->values.loopId = tmp;
        }else{
            s->error.loopId = true;
            SettingsError_AddMessage(&s->error,MSG_PARSEERROR_LOOPID);
        }
    }
    if (parsearg(pch, ACTIVECHANNEL, argValue, sizeof(argValue)) == 1) {
        int tmp = atoi(argValue);
        s->found.activeChannel = true;
        if(inRange(tmp,0,7)){
            s->parsed.activeChannel = true;
            s->values.activeChannel = tmp;
        }else{
            s->error.activeChannel = true;
            SettingsError_AddMessage(&s->error,MSG_PARSEERROR_ACTIVECHANNEL);
        }
    }
    if (parsearg(pch, ACTIVEPOWER, argValue, sizeof(argValue)) == 1) {
        int tmp = atoi(argValue);
        s->found.activePower = true;
        if(inRange(tmp,0,100)){
            s->parsed.activePower = true;
            s->values.activePower = tmp;
        }else{
            s->error.activePower = true;
            SettingsError_AddMessage(&s->error,MSG_PARSEERROR_ACTIVEPOWER);
        }
    }
    if (parsearg(pch, ACTIVETIME, argValue, sizeof(argValue)) == 1) {
        s->found.activeTime = true;
        if(isValidActiveTime(argValue)){
            s->parsed.activeTime = true;
            unsigned long nl = strchr(argValue, '\n') - argValue;
            strncpy(s->values.activeTime, argValue, min(lenTime,nl));
        }else{
            s->error.activeTime = true;
            SettingsError_AddMessage(&s->error,MSG_PARSEERROR_ACTIVETIME);
        }
    }
}
void SettingsInit(struct Settings_t *s){
    for (int i = 0; i < lenErrorMessages; ++i) {
        s->error.messages[i] = NULL;
    }
    s->values.activeChannel=0;
    s->values.activePower=0;
    s->values.loopId=0;
    memset(s->values.eventName,0,lenStrName);
    memset(s->values.pointName,0,lenStrName);
    memset(s->values.activeTime,0,lenTime);


    s->found.loopId=false;
    s->found.activeChannel=false;
    s->found.activePower=false;
    s->found.eventName=false;
    s->found.pointName=false;
    s->found.activeTime=false;

    s->parsed.loopId=false;
    s->parsed.activeChannel=false;
    s->parsed.activePower=false;
    s->parsed.eventName=false;
    s->parsed.pointName=false;
    s->parsed.activeTime=false;

    s->error.loopId=false;
    s->error.activeChannel=false;
    s->error.activePower=false;
    s->error.eventName=false;
    s->error.pointName=false;
    s->error.activeTime=false;

}
void SettingsFromQueryString(struct Settings_t *s, char *queryString) {
    SettingsInit(s);
    if (queryString != NULL) {
        char *pch = strtok(queryString, "&");
        while (pch != NULL) {
            SettingsParsePair(s,pch);
            pch = strtok(NULL, "&");
        }
    }
}

void SettingsFromActual(struct Settings_t *s) {
    SettingsInit(s);
    int entry = 1;
    char line[200];
    const int lenLogBuffer = 2000;
    char logBuffer[lenLogBuffer];
    int logPos = 0;
    printf("<!--\n"
                   "DEBUG GET CURRENT SETTINGS\n"
                   "> /home/drh/configure\n");

    FILE* output = popen("/home/drh/configure", "r");

    while ( fgets(line, 199, output) != NULL )
    {
        if(strlen(line)+logPos < lenLogBuffer){
            strcat(logBuffer, line);
            logPos += strlen(line);
        }
        SettingsParsePair(s,line);
    }
    pclose(output);
    strcat(logBuffer,"-->\n");
    printf("%s",logBuffer);
}

void SettingsApply(struct Settings_t *s) {
    const char* programPath = "/home/drh/configure";
    char argStr[1200];
    char buffer[200];

    if(s->parsed.loopId){
        sprintf(buffer,LOOPID "=%i ",s->values.loopId);
        strcat(argStr,buffer);
    }
    if(s->parsed.activePower){
        sprintf(buffer,ACTIVEPOWER "=%i ",s->values.activePower);
        strcat(argStr,buffer);
    }
    if(s->parsed.activeChannel){
        sprintf(buffer,ACTIVECHANNEL "=%i ",s->values.activeChannel);
        strcat(argStr,buffer);
    }
    if(s->parsed.eventName){
        sprintf(buffer,EVENTNAME "=%s ",s->values.eventName);
        strcat(argStr,buffer);
    }
    if(s->parsed.pointName){
        sprintf(buffer,POINTNAME "=%s ",s->values.pointName);
        strcat(argStr,buffer);
    }
    if(s->parsed.activeTime){
        sprintf(buffer,ACTIVETIME "=%s ",s->values.activeTime);
        strcat(argStr,buffer);
    }

    char command[1300];
    sprintf(command,"%s -p %s",programPath,argStr);
    char line[200];
    printf("<!--\n"
                   "DEBUG SETTINGS APPLY\n"
                   "> %s\n",command);

    FILE* output = popen(command, "r");

    while ( fgets(line, 199, output) )
    {
        printf("%s", line);
    }
    pclose(output);
    printf("-->\n");
}
