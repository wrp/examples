
from setuptools import setup, find_packages

def do_setup():
    setup(
        name='bar',
        description='the bar package',
        license='MIT',
        version='3.1',
        packages=find_packages(exclude=['tests*']),
        include_package_data=True,
    )

if __name__ == "__main__":
    do_setup()
