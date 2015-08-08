#!/usr/bin/env python3
import os
import sys
import proj_conf


def decide_compiler(config):
    """确定使用的编译器
    如果设置中使用的是clang，则使用设置中的编译器
    否则使用默认的clang++
    """

    if 'clang'in config:
        return config
    else:
        return 'clang++'


def gen_source_parameters(sources):
    """生成原代码文件参数
    因为SOURCES是定义为一个list
    """

    # 每个源文件用空格隔开
    return ' '.join(sources)


def gen_include_parameters(includes, systenIncludes):
    """生成头文件目录参数
    包含普通头文件目录和系统级头文件目录
    """

    include_parameters = ''

    include_parameters += ' '.join(map('-I{}'.format, includes))
    # for e in includes:
    #     include_parameters += '-I' + e

    # 在中间加入一个空格隔开
    include_parameters += ' '

    include_parameters += ' '.join(map('-isystem {}'.format, systenIncludes))
    # for e in systenIncludes:
    #     include_parameters += '-isystem' + ' ' + e

    return include_parameters


def gen_cppcheck_stds(stds):
    """生成cppcheck的std参数
    格式：--std=std1 --std=std2 ...
    """

    # std_parameters = ''

    # for e in stds:
    #     std_parameters += '--std={} '.format(e)

    # return std_parameters
    return ' '.join(map('--std={}'.format, stds))


def gen_cppcheck_enables(enables):
    """生成cppcheck的enable参数
    格式：enable1, enable2, ...
    """

    # 每个enable参数用逗号隔开
    return '--enable={}'.format(','.join(enables))


def clang_syntax_check():
    """使用clang -fsyntax-only进行语法检查
    该函数自己会有文字输出
    """

    if not proj_conf.CHECK_USE_CLANG_SYNTAX:
        print('(info) Clang syntax ignored')
        return 0

    # 编译器，源文件，C++标准，头文件目录
    command = '{} -fsyntax-only {} -std={} {}'
    compiler = decide_compiler(proj_conf.COMPILER)
    sources = gen_source_parameters(proj_conf.SOURCES)
    includes = gen_include_parameters(
        proj_conf.COMPILER_INCLUDES_DIRECTORY,
        proj_conf.COMPILER_SYSTEM_INCLUDES_DIRECTORY
    )

    # print('=' * 20 + 'BEGIN clang syntax check' + '=' * 20)
    print('{:=^100}'.format('BEGIN clang syntax check'))

    result = os.system(command.format(
        compiler,
        sources,
        proj_conf.CXX_VERSION,
        includes
    ))

    # print('=' * 21 + 'END clang syntax check' + '=' * 21)
    print('{:=^100}'.format('END clang syntax check'))

    print('(info) Command clang -fsyntax-only returned {}'.format(result))

    # 添加一个额外的行来分隔各部分的输出
    print('')

    return result


def clang_static_analyze():
    """使用clang --analyze进行静态检查
    该函数会有文字输出
    """

    if not proj_conf.CHECK_USE_CLANG_ANALYZE:
        print('(info) Clang static analyze ignored')
        return 0

    # 编译器，源文件，C++标准，头文件目录
    command = '{} --analyze {} -std={} {}'
    compiler = decide_compiler(proj_conf.COMPILER)
    sources = gen_source_parameters(proj_conf.SOURCES)
    includes = gen_include_parameters(
        proj_conf.COMPILER_INCLUDES_DIRECTORY,
        proj_conf.COMPILER_SYSTEM_INCLUDES_DIRECTORY
    )

    # print('=' * 20 + 'BEGIN clang static analyze' + '=' * 20)
    print('{:=^100}'.format('BEGIN clang static analyze'))

    result = os.system(command.format(
        compiler,
        sources,
        proj_conf.CXX_VERSION,
        includes
    ))

    # print('=' * 21 + 'END clang static analyze' + '=' * 21)
    print('{:=^100}'.format('END clang static analyze'))

    print('(info) Command clang --analyze returned {}'.format(result))

    # 添加一个额外的行来分隔各部分的输出
    print('')

    return result


def cppcheck_static_check():
    """使用cppcheck进行静态检查
    该函数会有文字输出
    """

    if not proj_conf.CHECK_USE_CPPCHECK:
        print('(info) Cppcheck static check ignored')
        return 0

    # C++标准，检查项（enable），源文件，头文件目录
    command = 'cppcheck {} {} {} {}'
    stds = gen_cppcheck_stds(proj_conf.CPPCHECK_STD)
    enables = gen_cppcheck_enables(proj_conf.CPPCHECK_ENABLE)
    sources = gen_source_parameters(proj_conf.SOURCES)
    includes = gen_include_parameters(
        proj_conf.COMPILER_INCLUDES_DIRECTORY,
        proj_conf.COMPILER_SYSTEM_INCLUDES_DIRECTORY
    )

    # print('=' * 20 + 'BEGIN cppcheck static check' + '=' * 20)
    print('{:=^100}'.format('BEGIN cppcheck static check'))

    result = os.system(command.format(
        stds,
        enables,
        sources,
        includes
    ))

    # print('=' * 21 + 'END cppcheck static check' + '=' * 21)
    print('{:=^100}'.format('END cppcehck static check'))

    print('(info) Command cppcheck returned {}'.format(result))

    # 添加一个额外的行来分隔各部分的输出
    print('')

    return result


def main(argc, argv):
    """主函数
    如果语法检查未通过，则不进行静态检查
    避免不必要的麻烦
    """

    print('(info) Start checking...')

    if clang_syntax_check() == 0:
        clang_static_analyze()
        cppcheck_static_check()
    else:
        print('(error) Clang syntax check failed')

    print('(info) Exited')


if __name__ == '__main__':
    main(len(sys.argv), sys.argv)
