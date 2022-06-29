import requests
import json
import urllib3


use_logout_all = True
urllib3.disable_warnings()
url = "https://127.0.0.1:8000/"
# url = "http://127.0.0.1:8888/"

# Step1 - Login with basic authentication to /login
session = requests.Session()
session.auth = ('user1', 'user1')
response = session.post(url + 'login/', verify=False)
if response.status_code != 200:
    print(response.text)
    exit()

data = json.loads(response.text)
token = data['token']
print('Token:', token)

# Step2 - Query
headers = {
    'Accept': 'application/json',
    'Authorization': f'Token {token}',
}
params = {'license-plate-number': 6987}
response = requests.get(url, params=params, headers=headers, verify=False)
if response.status_code != 200:
    print(response.text)
    exit()

data = json.loads(response.text)
print('Data:', data)

if not use_logout_all:
    # Step3-a - Logout
    response = requests.post(url + 'logout/', headers=headers, verify=False)
    if response.status_code != 204:
        print(response.text)
        exit()
    print('Logout Success')

else:
    # Step3-b - Logout all
    response = requests.post(url + 'logoutall/', headers=headers, verify=False)
    if response.status_code != 204:
        print(response.text)
        exit()
    print('Logout All Success')
