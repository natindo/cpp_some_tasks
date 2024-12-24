from setuptools import setup, Extension

cjson_module = Extension(
    'cjson',
    sources=['cjson.c'],
    extra_compile_args=[],
    extra_link_args=[],
)

setup(
    name='cjson',
    version='0.0.1',
    description='Example C-based JSON parser and dumper',
    ext_modules=[cjson_module],
)
