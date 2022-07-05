import urllib3
import requests
import json
import time
import logging
import concurrent.futures
import seaborn as sns
import matplotlib.pyplot as plt
urllib3.disable_warnings()

logger = logging.getLogger()
log_file_name = 'client_multi.log'
file_handler = logging.FileHandler(log_file_name, mode='w')
logger.addHandler(file_handler)
logger.propagate = False
logger.setLevel(logging.INFO)

num_users = 24
num_threads = num_users
num_queries = 100


class Client:
    URL = 'https://localhost:8000/'

    def __init__(self, username, password):
        self.username = username
        self.password = password
        self.token = None
        self.auth_header = {
            'Accept': 'application/json',
        }

    def login(self):
        session = requests.Session()
        session.auth = (self.username, self.password)
        response = session.post(self.URL + 'login/', verify=False)
        if response.status_code != 200:
            msg = f'unable to login - {response.text} ({response.status_code})'
            raise Exception(msg)

        data = json.loads(response.text)
        self.token = data['token']
        self.auth_header['Authorization'] = f'Token {self.token}'

    def query(self, plate_number):
        params = {'license-plate-number': plate_number}
        st = time.time()
        response = requests.get(self.URL, params=params, headers=self.auth_header, verify=False)
        et = time.time()
        latency_ms = (et - st) * 1000
        logger.info(f'query take {latency_ms:07.3f} ms ({response.status_code}) from {self.username}')

    def logout(self):
        response = requests.post(self.URL + 'logout/', headers=self.auth_header, verify=False)
        if response.status_code != 204:
            msg = f'unable to logout - {response.text} ({response.status_code})'
            raise Exception(msg)


def request_queries(user_num):
    try:
        client = Client(f'user{user_num}', f'user{user_num}')
        client.login()
        for i in range(num_queries):
            client.query(i)
        client.logout()
    except Exception as e:
        logger.error(e)


if __name__ == "__main__":
    with concurrent.futures.ThreadPoolExecutor(max_workers=num_threads) as executor:
        future_to_url = {executor.submit(request_queries, i): i for i in range(1, 1 + num_users)}

    with open(log_file_name) as f:
        latencies = [float(line.rstrip().split()[2]) for line in f]

    f, (ax_box, ax_hist) = plt.subplots(2, sharex=True, gridspec_kw={"height_ratios": (.15, .85)})
    sns.boxplot(latencies, ax=ax_box)
    sns.histplot(data=latencies, ax=ax_hist)
    ax_box.set(xlabel='')
    plt.show()
