
from flask import Flask
from redis import Redis
import sys

app = Flask(__name__)
redis = Redis(host='redis', port=6379)

@app.route('/')
def hello():
    count = redis.incr('hits')
    name = 'Banana' if len(sys.argv) < 2 else ','.join(sys.argv[1:])
    return 'Hello, ' + name + '!  I have been seen ' + str(count) + ' times.\n'

if __name__ == "__main__":
    app.run(host="0.0.0.0", debug=True)
