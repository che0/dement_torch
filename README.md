dement_torch
============

dement_torch is a really primitive HTTP connection/request testing tool

Screenshot:

```
$ THREADS=2 REQUESTS=4 SLEEP_MS=100 TIMEOUT_SEC=30 CONNECT_TIMEOUT_SEC=5 \
    URL=https://localhost:42/foo.jpg SKIP_200=0 ./dement_torch
{"asctime":"2022-06-13 12:01:44,147","message":"Starting with THREADS=2 REQUESTS=4 SLEEP_MS=100 URL=https://localhost:42/foo.jpg TIMEOUT_SEC=30 CONNECT_TIMEOUT_SEC=5 SKIP_200=0 STATUS_FIELD=status_code"}
{"asctime":"2022-06-13 12:01:44,156","status_code":200,"downloaded_bytes":82447,"connect_time_ms":225,"total_time_ms":6}
{"asctime":"2022-06-13 12:01:44,260","status_code":200,"downloaded_bytes":81953,"connect_time_ms":229,"total_time_ms":6}
{"asctime":"2022-06-13 12:01:44,339","status_code":200,"downloaded_bytes":82442,"connect_time_ms":190,"total_time_ms":5}
{"asctime":"2022-06-13 12:01:44,446","status_code":200,"downloaded_bytes":81863,"connect_time_ms":110,"total_time_ms":5}
{"asctime":"2022-06-13 12:01:44,607","status_code":599,"message":"Failed to connect to localhost port 42 after 0 ms: Connection refused"}
{"asctime":"2022-06-13 12:01:44,610","status_code":599,"message":"Failed to connect to localhost port 42 after 0 ms: Connection refused"}
{"asctime":"2022-06-13 12:01:44,649","status_code":599,"message":"Failed to connect to localhost port 42 after 0 ms: Connection refused"}
{"asctime":"2022-06-13 12:01:44,656","status_code":599,"message":"Failed to connect to localhost port 42 after 0 ms: Connection refused"}
{"asctime":"2022-06-13 12:03:44,941","message":"Run complete."}
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
* STATUS_FIELD -- name of JSON field with status code (optional, default is "status_code")
