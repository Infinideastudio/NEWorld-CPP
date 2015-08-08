#!/usr/bin/env python3
import unittest_conf
import os
import shutil


def main():
    # 清空缓存目录
    print('(info) Prepare temp folder')
    try:
        shutil.rmtree(unittest_conf.UNITTEST_TEMP_FOLDER)
    except:
        print('(warning) No origin temp folder found')

    os.mkdir(unittest_conf.UNITTEST_TEMP_FOLDER)

    # 编译
    print('(info) Compiling unittest...')
    if os.system('clang++ -std=c++1z {} {} -o exec.out'.format(
        unittest_conf.UNITTEST_MAIN,
        unittest_conf.UNITTEST_LINK_LIBRARY,
    )) == 0:
        print('(info) Running unittest...')

        print('{:=^120}'.format('START UNITTEST'))

        os.system('./exec.out')

        print('{:=^120}'.format('UNITTEST STOPPED'))

    else:
        print('(error) Cannot compile unittest')

    print('(info) Unittest exited')

if __name__ == '__main__':
    main()
