#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>

#include "main.h"


void renderStatus(struct Settings_t* desired, struct Settings_t* actual){

    printf("<html>\n"
                   "<head><title>Active Tag Configuration</title></head>\n"
                   "<body>\n");

    // print a header
    printf("<h4>Active Tag Configuration</h4>\n");

    // render errors from previous form submission
    if(desired!=NULL){
        if(desired->error.activeTime
           || desired->error.activePower
           || desired->error.activeChannel
           || desired->error.loopId
           || desired->error.pointName
           || desired->error.eventName
                ){
            printf("<ul>\n");
            int ctr = 0;
            while(desired->error.messages[ctr] != NULL){
                printf("<li style=\"color:red;\">%s</li>\n",desired->error.messages[ctr]);
                ++ctr;
            }
            printf("</ul>\n");
        }

    }

    // render actual state form
    printf("<form method=\"get\">\n"
               EVENTNAME ":<br />\n"
               "<input type=\"text\" name=\"" EVENTNAME "\"     value=\"%s\"></br>\n"
               POINTNAME ":<br />\n"
               "<input type=\"text\" name=\"" POINTNAME "\"     value=\"%s\"></br>\n"
               LOOPID ":<br />\n"
               "<input type=\"text\" name=\"" LOOPID "\"        value=\"%i\"></br>\n"
               ACTIVECHANNEL  ":<br />\n"
               "<input type=\"text\" name=\"" ACTIVECHANNEL "\" value=\"%i\"></br>\n"
               ACTIVEPOWER  ":<br />\n"
               "<input type=\"text\" name=\"" ACTIVEPOWER "\"   value=\"%i\"></br>\n"
               ACTIVETIME ": %s <br />\n"
               "<input type=\"text\" name=\"" ACTIVETIME "\"    value=\"\"></br>\n"
               "<input type=\"submit\" name=\"submit\" value=\"submit\"></br>\n"
               "</form>\n",
           actual->values.eventName,
           actual->values.pointName,
           actual->values.loopId,
           actual->values.activeChannel,
           actual->values.activePower,
           actual->values.activeTime
    );

    printf("</body>\n"
                   "</html>\n");
}



int main(int argc, char **argv, char **envp) {

    // print out HTTP headers
    printf("Content-type: text/html\n"); // required as part of HTTP protocol
    printf("\n");

    struct Settings_t desired;
    char *queryString = getenv("QUERY_STRING");
    SettingsFromQueryString(&desired, queryString);

    if (desired.found.activeTime || desired.found.activeChannel || desired.found.activePower || desired.found.loopId || desired.found.eventName || desired.found.pointName){
        // they are trying to change settings
        // so do that and then render the results
        SettingsApply(&desired);
    }

    struct Settings_t actual;
    SettingsFromActual(&actual);

    renderStatus(&desired, &actual);
}