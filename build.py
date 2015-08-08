#!/usr/bin/env python3

#
# Copyright 2015 riteme
#

import sys
import os
import proj_conf


def ParseParameters():
    """解析传入的参数
    会修改设置
    """

    for param in range(1, len(sys.argv)):
        if sys.argv[param] == '-debug':  # 调试模式
            proj_conf.DEBUG_MODE = True
        elif '-O' in sys.argv[param]:  # 设置优化
            proj_conf.OPTIMIZE_LEVEL = sys.argv[param][2:]
        # 使用clang
        elif sys.argv[param] == '-clang' or sys.argv[param] == '-clang++':
            proj_conf.COMPILER = 'clang++'
        elif sys.argv[param] == '-gcc' or sys.argv[param] == '-g++':  # 使用GCC
            proj_conf.COMPILER = 'g++'
        elif sys.argv[param] == '--help' or sys.argv[param] == '-h':  # 显示帮助
            print(
                """Usage:
                build.py [-debug] [-O0, 1, 2, 2.5, 3] [-clang] [-gcc] [-h]
                """
            )
            exit()
        else:
            print('Unknown parameter: {}'.format(sys.argv[param]))
            exit()


if __name__ != '__main__':
    # 警告使用者不应将此当做模块使用
    raise RuntimeWarning("It's not a module to load!")
else:
    # 执行准备脚本
    if proj_conf.PREPARE_SHELL != '':
        print('(info) Performing preparation shell...')
        result = os.system('sh ' + proj_conf.PREPARE_SHELL)
        if result != 0:
            print(
                '(error) Preparation shell failed with return code {}'.format(
                    result
                )
            )
            exit()

    # 如果有参数则进行解析
    if len(sys.argv) > 1:
        ParseParameters()

    command = ''

    # 展示版本号
    print('(info) Program version: V{}'.format(proj_conf.VERSION))

    # 确定编译器
    print('(info) Compiler: {}'.format(proj_conf.COMPILER))
    command += proj_conf.COMPILER + ' '

    # 确定C++版本
    print('(info) C++ version: {}'.format(proj_conf.CXX_VERSION))
    command += '-std=' + proj_conf.CXX_VERSION + ' '

    # 确定STL库（libstdc++或libc++）
    if 'clang' in proj_conf.COMPILER:
        print('(info) STL library: {}'.format(proj_conf.CXX_STDLIB))
        command += '-stdlib=' + proj_conf.CXX_STDLIB + ' '
    elif 'gcc' in proj_conf.COMPILER or 'g++' in proj_conf.COMPILER:
        print("(warning) STL library setting ignored")

    # 加入源码路径
    for source in proj_conf.SOURCES:
        print('(info) Add source: {}'.format(source))
        command += source + ' '

    # 添加警告参数
    print('(info) Add warning parameters...')
    for warning in proj_conf.COMPILER_WARNINGS:
        command += '-W' + warning + ' '
    for ignoredWarning in proj_conf.COMPILER_IGNORE_WARNINGS:
        command += '-Wno-' + ignoredWarning + ' '

    # 添加链接库参数
    print('(info) Add link parameters...')
    for library in proj_conf.COMPILER_LIBRARYS:
        command += '-l' + library + ' '
    for libraryDirectory in proj_conf.COMPILER_LIBRARYS_DIRECTORY:
        command += '-L' + libraryDirectory + ' '

    # 添加头文件参数
    print('(info) Add include parameters...')
    for include in proj_conf.COMPILER_INCLUDES_DIRECTORY:
        command += '-I' + include + ' '
    for systemInclude in proj_conf.COMPILER_SYSTEM_INCLUDES_DIRECTORY:
        command += '-isystem' + ' ' + systemInclude + ' '

    # 添加文件输出参数
    print('(info) Add other parameters...')
    output = proj_conf.OUTPUT_LOCATION
    if proj_conf.OUTPUT_WITH_VERSION:
        if proj_conf.OUTPUT_MODE == 'shared':
            # libXXX.so.version
            output += '.' + proj_conf.VERSION
        elif proj_conf.OUTPUT_MODE == 'executable':
            indexDot = os.path.basename(output).rfind('.')
            if indexDot == -1:
                # filename.version
                output += '.' + proj_conf.VERSION
            else:
                # filename.version.extension
                output = '{}.{}.{}'.format(
                    output[:len(os.path.dirname(output)) + indexDot + 1],
                    proj_conf.VERSION,
                    output[len(os.path.dirname(output)) + indexDot + 2:]
                )

    command += '-o ' + output + ' '

    # 添加动态库参数
    if proj_conf.OUTPUT_MODE == 'shared':
        command += '-fPIC -shared '

    # 调试和优化参数
    if proj_conf.DEBUG_MODE:
        command += '-g '
    else:  # 如果是调试模式则不开启优化，因为优化可能会打乱代码顺序
        command += {
            '0': '-O0',
            '1': '-O1',
            '2': '-O2',
            '2.5': '-Os',
            '3': '-O3'
        }.get(proj_conf.OPTIMIZE_LEVEL, '-O0')  # 默认为无优化

    command += ' '

    # 加入其余的参数
    command += ' '.join(proj_conf.COMPILER_OTHER_PARAMETERS)

    # 打印最终结果
    print('(info) Final shell: {}'.format(command))

    # 执行命令，开始编译
    print('(info) Going to compile...')
    # print('==================================================BEGIN=====')
    print('{:=^100}'.format('BEGIN'))

    result = os.system(command)

    # print('===================================================END======')
    print('{:=^100}'.format('END'))

    # 报告错误
    if result != 0:
        print('(error) Compilion failed with return code {}'.format(result))

    # 链接程序到latest
    if proj_conf.OUTPUT_LINK_TO_LATEST and result == 0:

        # 如果已存在，则先删除，防止发生错误
        if os.path.exists(proj_conf.OUTPUT_LOCATION):
            os.remove(proj_conf.OUTPUT_LOCATION)

        result = os.system(
            'ln {} {}'.format(
                output,
                proj_conf.OUTPUT_LOCATION
            )
        )

        # 检查返回值
        if result != 0:
            print(
                '(error) Output link failed with return code {}'.format(
                    result
                )
            )
            exit()

    # 执行清理脚本
    if proj_conf.CLEANUP_SHELL != '':
        print('(info) Performing clean-up shell...')
        result = os.system('sh ' + proj_conf.CLEANUP_SHELL)
        if result != 0:
            print(
                '(error) Clean-up shell failed with return code {}'.format(
                    result
                )
            )
            exit()

    # 报告结束
    print('(info) Compilion end')
