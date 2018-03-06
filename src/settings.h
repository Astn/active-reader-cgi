//
// Created by astn on 3/6/18.
//

#ifndef DANSCGI_SETTINGS_H
#define DANSCGI_SETTINGS_H

#include <stdbool.h>

#define EVENTNAME "EventName"
#define POINTNAME "PointName"
#define LOOPID "LoopId"
#define ACTIVECHANNEL "ActiveChannel"
#define ACTIVEPOWER "ActivePower"
#define ACTIVETIME "ActiveTime"
#define szStrName 100
#define szTime 15
#define MSG_PARSEERROR_EVENTNAME EVENTNAME " must have a length more than 2 and less than 100"
#define MSG_PARSEERROR_POINTNAME POINTNAME " must have a length more than 2 and less than 100"
#define MSG_PARSEERROR_LOOPID LOOPID " was out of range. Expected: 0..7"
#define MSG_PARSEERROR_ACTIVECHANNEL ACTIVECHANNEL " was out of range. Expected: 0..7"
#define MSG_PARSEERROR_ACTIVEPOWER ACTIVEPOWER " was out of range. Expected: 0..100"
#define MSG_PARSEERROR_ACTIVETIME ACTIVETIME " was not in the expected format. Expected: YYYMMDD-HHMMSS"
#define szErrorMessages 6
// parameters we handle:
/*
EventName=????????????????  (ASCII)
Point Name=???????????????? (ASCII)
LoopID=0..7
ActiveChannel=0..7
ActivePower=0..100
ActiveTime=YYYYMMDD-HHMMSS
*/
struct SettingsValues_t{
    int loopId ;
    int activeChannel;
    int activePower;
    char eventName[szStrName];
    char pointName[szStrName];
    char activeTime[szTime];
};
struct SettingsFound_t{
    bool loopId;
    bool activeChannel;
    bool activePower;
    bool eventName;
    bool pointName;
    bool activeTime;
};
struct SettingsParsed_t{
    bool loopId;
    bool activeChannel;
    bool activePower;
    bool eventName;
    bool pointName;
    bool activeTime;
};
struct SettingsError_t{
    bool loopId;
    bool activeChannel;
    bool activePower;
    bool eventName;
    bool pointName;
    bool activeTime;
    char* messages[szErrorMessages];
};
struct Settings_t{
    struct SettingsValues_t values;
    struct SettingsFound_t found;
    struct SettingsParsed_t parsed;
    struct SettingsError_t error;
};


int parsearg(const char *pch, const char *argEventName, char *eventName, const size_t szeventName);
bool inRange(int in, int min, int max);
int match(const char *string, char *pattern);
bool isValidActiveTime(char* activeTime);
void SettingsError_AddMessage(struct SettingsError_t * e, char* msg);
void SettingsParsePair(struct Settings_t *s, char* pch);
void SettingsFromQueryString(struct Settings_t *s, char *queryString);
void SettingsFromActual(struct Settings_t *s);
void SettingsApply(struct Settings_t *s);


#endif //DANSCGI_SETTINGS_H
