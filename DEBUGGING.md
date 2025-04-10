# Debugging

## Inconsistent Request Body Parsing

### Non-Working Case

```
[REQUEST] Received 202 bytes of data
[REQUEST] Received request: POST /path HTTP/1.1
Host: 127.0.0.1:8203
User-Agent: python-requests/2.22.0
Accept-Encoding: gzip, deflate
Accept: */*
Connection: keep-alive
Content-Type: application/json
Content-Length: 54


[REQUEST] Parsing request line: POST /path HTTP/1.1
[REQUEST] Method: POST
[REQUEST] Path: /path
[REQUEST] HTTP Version: HTTP/1.1
[REQUEST] Processing POST request body
[REQUEST] Starting request body parsing
[REQUEST] Raw request (hex): 50 4F 53 54 20 2F 70 61 74 68 20 48 54 54 50 2F 31 2E 31 0D 0A 48 6F 73 74 3A 20 31 32 37 2E 30 2E 30 2E 31 3A 38 32 30 33 0D 0A 55 73 65 72 2D 41 67 65 6E 74 3A 20 70 79 74 68 6F 6E 2D 72 65 71 75 65 73 74 73 2F 32 2E 32 32 2E 30 0D 0A 41 63 63 65 70 74 2D 45 6E 63 6F 64 69 6E 67 3A 20 67 7A 69 70 2C 20 64 65 66 6C 61 74 65 0D 0A 41 63 63 65 70 74 3A 20 2A 2F 2A 0D 0A 43 6F 6E 6E 65 63 74 69 6F 6E 3A 20 6B 65 65 70 2D 61 6C 69 76 65 0D 0A 43 6F 6E 74 65 6E 74 2D 54 79 70 65 3A 20 61 70 70 6C 69 63 61 74 69 6F 6E 2F 6A 73 6F 6E 0D 0A 43 6F 6E 74 65 6E 74 2D 4C 65 6E 67 74 68 3A 20 35 34 0D 0A 0D 0A
[REQUEST] Raw request (ascii): POST /path HTTP/1.1..Host: 127.0.0.1:8203..User-Agent: python-requests/2.22.0..Accept-Encoding: gzip, deflate..Accept: */*..Connection: keep-alive..Content-Type: application/json..Content-Length: 54....
[REQUEST] Header end length: 4
[REQUEST] Calculated body length: 0
[REQUEST] No body found - empty body after headers
[REQUEST] No request body found
[REQUEST] Handling POST request
[RESPONSE] Creating HTTP response
[RESPONSE] Status: 200, Content-Type: text/plain
[RESPONSE] Body length: 25
[RESPONSE] Sending HTTP response
[RESPONSE] Status: 200 OK
[RESPONSE] Status Line: HTTP/1.1 200 OK

[RESPONSE] Content-Type: text/plain
[RESPONSE] Content-Length: 25
[RESPONSE] Sending response body
[RESPONSE] Response sent successfully
[RESPONSE] Freeing response
```

### Working Case

```
[REQUEST] Received 256 bytes of data
[REQUEST] Received request: POST /path HTTP/1.1
Host: 127.0.0.1:8203
User-Agent: python-requests/2.22.0
Accept-Encoding: gzip, deflate
Accept: */*
Connection: keep-alive
Content-Type: application/json
Content-Length: 54

{"number": "12524", "type": "issue", "action": "show"}
[REQUEST] Parsing request line: POST /path HTTP/1.1
[REQUEST] Method: POST
[REQUEST] Path: /path
[REQUEST] HTTP Version: HTTP/1.1
[REQUEST] Processing POST request body
[REQUEST] Starting request body parsing
[REQUEST] Raw request (hex): 50 4F 53 54 20 2F 70 61 74 68 20 48 54 54 50 2F 31 2E 31 0D 0A 48 6F 73 74 3A 20 31 32 37 2E 30 2E 30 2E 31 3A 38 32 30 33 0D 0A 55 73 65 72 2D 41 67 65 6E 74 3A 20 70 79 74 68 6F 6E 2D 72 65 71 75 65 73 74 73 2F 32 2E 32 32 2E 30 0D 0A 41 63 63 65 70 74 2D 45 6E 63 6F 64 69 6E 67 3A 20 67 7A 69 70 2C 20 64 65 66 6C 61 74 65 0D 0A 41 63 63 65 70 74 3A 20 2A 2F 2A 0D 0A 43 6F 6E 6E 65 63 74 69 6F 6E 3A 20 6B 65 65 70 2D 61 6C 69 76 65 0D 0A 43 6F 6E 74 65 6E 74 2D 54 79 70 65 3A 20 61 70 70 6C 69 63 61 74 69 6F 6E 2F 6A 73 6F 6E 0D 0A 43 6F 6E 74 65 6E 74 2D 4C 65 6E 67 74 68 3A 20 35 34 0D 0A 0D 0A 7B 22 6E 75 6D 62 65 72 22 3A 20 22 31 32 35 32 34 22 2C 20 22 74 79 70 65 22 3A 20 22 69 73 73 75 65 22 2C 20 22 61 63 74 69 6F 6E 22 3A 20 22 73 68 6F 77 22 7D
[REQUEST] Raw request (ascii): POST /path HTTP/1.1..Host: 127.0.0.1:8203..User-Agent: python-requests/2.22.0..Accept-Encoding: gzip, deflate..Accept: */*..Connection: keep-alive..Content-Type: application/json..Content-Length: 54....{"number": "12524", "type": "issue", "action": "show"}
[REQUEST] Header end length: 4
[REQUEST] Calculated body length: 54
[REQUEST] Parsed body (hex): 7B 22 6E 75 6D 62 65 72 22 3A 20 22 31 32 35 32 34 22 2C 20 22 74 79 70 65 22 3A 20 22 69 73 73 75 65 22 2C 20 22 61 63 74 69 6F 6E 22 3A 20 22 73 68 6F 77 22 7D
[REQUEST] Parsed body (ascii): {"number": "12524", "type": "issue", "action": "show"}
[REQUEST] Successfully parsed request body of length 54
[REQUEST] Request body length: 54
[REQUEST] Request body content: {"number": "12524", "type": "issue", "action": "show"}
[REQUEST] Handling POST request
[RESPONSE] Creating HTTP response
[RESPONSE] Status: 200, Content-Type: text/plain
[RESPONSE] Body length: 87
[RESPONSE] Sending HTTP response
[RESPONSE] Status: 200 OK
[RESPONSE] Status Line: HTTP/1.1 200 OK

[RESPONSE] Content-Type: text/plain
[RESPONSE] Content-Length: 87
[RESPONSE] Sending response body
[RESPONSE] Response sent successfully
[RESPONSE] Freeing response
```

### Notes

In the non-working case, the request is being cut off - we're receiving 202 bytes but the body is missing, while in the working case we receive 256 bytes with the complete body.

The key differences:
* Non-working: Received `202 bytes` of data
* Working: Received `256 bytes` of data

The issue is that we're not reading the complete request.

Let's fix this by modifying the socket reading in main.c to ensure we read the entire request based on the Content-Length header:
