dement_torch
============

dement_torch is a really primitive HTTP connection/request testing tool

Dependencies:

* libcurl

Build:

```
make
```

Usage:

```
THREADS=20 REQUESTS=200 SLEEP_MS=1000 TIMEOUT_SEC=30 CONNECT_TIMEOUT_SEC=5 \
    URL=https://localhost:42/foo.jpg SKIP_200=1 ./dement_torch
```

Env variables:

* THREADS -- number of threads to run
* REQUESTS -- requests to send (per thread)
* SLEEP_MS -- random sleep up to N ms after every request
* TIMEOUT_SEC -- request timeout in seconds
* CONNECT_TIMEOUT_SEC -- connect timeout in seconds
* URL -- well, the URL
* SKIP_200 -- 0|1, if printout should skip responses with status 200
