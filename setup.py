from setuptools import Extension, setup

module = Extension("mykmeanspp", sources=['kmeansmodule.c'])
setup(name="mykmeanspp",
      version="1.0",
      description="A Python wrapper for the k-means++ algorithm",
      ext_modules=[module])