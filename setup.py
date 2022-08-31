from setuptools import setup, find_packages, Extension

setup(
    name='mykmeanssp',
    version='0.1.0',
    author="Daniel Jaffe & Ron Blachar",
    description="k-means algorithm, implemented by ex1",
    install_requires=['invoke'],
    packages=find_packages(where='.'),
    license='GPL-2',
    classifiers=[
        'Development Status :: 3 - Aplha',
        'License :: OSI Approved :: GNU General Public License  v2 (GPLv2)',
        'Natural Language :: English',
        'Programming Language :: Python :: 3 :: Only',
        'Programming Language :: Python ::  Implementation :: CPython',
    ],
    ext_modules=[
        Extension(
            'mykmeanssp',
            ['eigengapHeuristic.c','errors.c','graphRepresentation.c','matrix.c','rotationMatrix.c','spkmeans.c','spkmeansmodule.c','userInput.c'],
        ),
    ]
)
