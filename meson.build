project(
    'top2bottom',
    'cpp',
    default_options : ['cpp_std=c++14'],
    version : '1.0'
    )

top2bottom_core = static_library(
    'top2bottom-core',
    files('top2bottom.cpp')
    )

top2bottom = executable(
    'top2bottom',
    files('main.cpp'),
    link_with: top2bottom_core,
    install: true
    )

top2bottom_tests = executable(
    'top2bottom-tests',
    files('catch_main.cpp', 'top2bottom_test.cpp'),
    link_with: top2bottom_core,
    )

test('top2bottom tests', top2bottom_tests)
