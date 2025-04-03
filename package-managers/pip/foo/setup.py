
from setuptools import setup, find_packages

def do_setup():
    setup(
        name='foo',
        description='the foo package',
        license='MIT',
        version='1.0',
        install_requires=[
            'bar==1.0',
        ],
    )

if __name__ == "__main__":
    do_setup()
