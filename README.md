
Simple app that expects its input to be compatible with CGI. Specifically it expects an environment variable (QUERY_STRING) for example:
```
env QUERY_STRING="EventName=Yo&PointName=Ho&LoopId=10&ActiveChannel=7&ActivePower=67&ActiveTime=20181005-235500"
```

Example test from command line
```bash
env QUERY_STRING="EventName=Yo&PointName=Ho&LoopId=10&ActiveChannel=7&ActivePower=67&ActiveTime=20181005-235500" cmake-build-debug/danscgi
```

Produces http output. Strip off the first 2 lines to get just the html.