from setuptools import setup, find_packages


with open('README.rst') as readme:
    long_description = ''.join(readme).strip()


setup(
    name='orphanage',
    version='0.0.0',
    url='https://github.com/tonyseek/python-orphanage',
    author='Jiangge Zhang',
    author_email='tonyseek@gmail.com',
    description='Let orphan processes suicide',
    long_description=long_description,
    packages=find_packages(),
    zip_safe=False,
    license='MIT',
    keywords=['process', 'management', 'orphan'],
    classifiers=[
        'Development Status :: 1 - Planning',
        'Intended Audience :: Developers',
        'Intended Audience :: System Administrators',
        'License :: OSI Approved :: MIT License',
        'Operating System :: POSIX',
        'Operating System :: POSIX :: Linux',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: Implementation :: CPython',
        'Programming Language :: Python :: Implementation :: PyPy',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Utilities',
    ],
    install_requires=[],
    extras_require={},
    platforms=['POSIX', 'Linux'],
)
