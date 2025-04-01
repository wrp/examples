#!/usr/bin/env python

import time
from datetime import datetime
import threading



class status_bar:
    def run(self, message='', interval=.3):
        self.message = message
        self.interval = interval
        a = threading.Thread(target=self.show_status)
        a.daemon = True
        a.start()
        return self

    def show_status(self):
        spinner = '-\|/'
        idx = 0
        while True:
            m = datetime.now().strftime(f'%T {spinner[idx]}{spinner[-idx]}')
            end = f'\033[K\r'
            idx = (idx+1) % 4
            print(f'{m}: ', end='')
            print(self.message, end=end, flush=True)
            time.sleep(self.interval)
