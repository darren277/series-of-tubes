import requests

PORT = 8203

def test_get():
    r = requests.get('http://127.0.0.1:{PORT}/test'.format(PORT=PORT))
    print(r.status_code)
    print(r.text)

def test_query():
    r = requests.get('http://127.0.0.1:{PORT}/path/script.cgi?field1=value1&field2=value2'.format(PORT=PORT))
    print(r.status_code)
    print(r.text)

def test_post_with_json_payload():
    r = requests.post('http://127.0.0.1:{PORT}/path'.format(PORT=PORT), json={'number': '12524', 'type': 'issue', 'action': 'show'})
    print(r.status_code)
    print(r.text)

def test_post_with_xml_payload():
    r = requests.post('http://127.0.0.1:{PORT}/path'.format(PORT=PORT), data="<?xml version='1.0' encoding='utf-8'?>\n<a>\u0431</a>")
    print(r.status_code)
    print(r.text)


def test_simple_service():
    response = requests.get("http://127.0.0.1:8123/test")
    print("Simple Service Response:", response.text)

def test_nginx_service():
    response = requests.get("http://localhost:8124/test")
    print("Nginx Service Response:", response.text)

def test_python_service():
    data = {'key': 'value'}
    response = requests.post("http://localhost:8125/test", json=data)
    print("Python Service Response:", response.json())

if __name__ == "__main__":
    test_simple_service()
    #test_nginx_service()
    #test_python_service()

#test_get()
#test_query()
#test_post_with_json_payload()

