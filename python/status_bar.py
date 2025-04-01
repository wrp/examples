#!/usr/bin/env python

import time
from datetime import datetime
import threading



class status_bar:
    def run(self, message='', interval=.3):
        self.message = message
        self.interval = interval
        self.spinner = '-\|/'
        self.idx = 0

        a = threading.Thread(target=self._main_loop)
        a.daemon = True
        a.start()
        return self

    def display(self, message):
        self.message = message

    def _show_message(self):
            c = self.spinner[self.idx] + self.spinner[-self.idx]
            m = datetime.now().strftime(f'%T {c}')
            end = f'\033[K\r'
            self.idx = (self.idx+1) % 4
            print(f'{m}: ', end='')
            print(self.message, end=end, flush=True)

    def _main_loop(self):
        while True:
            self._show_message()
            time.sleep(self.interval)
