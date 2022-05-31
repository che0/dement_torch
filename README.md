dement_torch
============

dement_torch is a really primitive HTTP connection/request testing tool

Screenshot:

```
$ THREADS=20 REQUESTS=200 SLEEP_MS=100 TIMEOUT_SEC=30 CONNECT_TIMEOUT_SEC=5 \
    URL=https://localhost:42/foo.jpg SKIP_200=0 ./dement_torch
2022-05-31 21:13:12 status 200, downloaded 4651 bytes, connect 81 ms, total 11 ms
2022-05-31 21:13:12 status 200, downloaded 4651 bytes, connect 84 ms, total 12 ms
2022-05-31 21:13:12 status 200, downloaded 4651 bytes, connect 87 ms, total 12 ms
2022-05-31 21:13:12 status 200, downloaded 4651 bytes, connect 89 ms, total 11 ms
2022-05-31 21:13:12 status 200, downloaded 4651 bytes, connect 89 ms, total 12 ms
2022-05-31 21:13:12 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:12 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:13 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:13 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:13 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:13 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:13 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:13 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:13 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:14 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:14 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:14 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:14 FAIL: Failed to connect to localhost port 42 after 0 ms: Connection refused
2022-05-31 21:13:14 Run complete.
```

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
