import os
import shlex
import subprocess
import shutil

build_dir = os.path.abspath("../cmake_cache_0001")
if os.path.isdir(build_dir):
    shutil.rmtree(build_dir)
    pass
if not os.path.isdir(build_dir):
    os.makedirs(build_dir)
old_dir_abs_path = os.path.abspath(os.curdir).replace('\\', '/')
cmake_cmd = "cmake -G 'NMake Makefiles' {0}".format(old_dir_abs_path)
vstoll_cmd = "D:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/vcvarsall.bat x64"
print(cmake_cmd)
os.chdir(build_dir)
ret = True
ret = ret and 0 == subprocess.call(shlex.split("{0} & {1}".format(vstoll_cmd, cmake_cmd)))
ret = ret and 0 == subprocess.call(shlex.split("{0} & {1}".format(vstoll_cmd, "nmake")))
ret and shutil.copy("main.exe", old_dir_abs_path)
ret and shutil.copy("main.pdb", old_dir_abs_path)
os.chdir(old_dir_abs_path)
shutil.rmtree(build_dir)