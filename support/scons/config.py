#
# Copyright (C) 2015 Niek Linnenbank
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import os
import os.path
import shutil
import datetime
import platform
from SCons.Script import *
from autoconf import *

class ConfDict(dict):

    def __init__(self):
        self.locked = False
        self.cmdPrefix = ""

    def lock(self, val, prefix = ""):
        self.locked = val
        self.cmdPrefix = prefix

    def __setitem__(self, name, value):
        """
        This function overrides variable assignment
        Ensures variables from the command line are taken first
        instead of from the .conf files.
        """
        global cmd_items

        if self.locked and (self.cmdPrefix + name) in cmd_items:
            return
        else:
            return dict.__setitem__(self, name, value)

local_dict = ConfDict()
cmd_items  = None

def initialize(target, host, params):
    """
    Initialize configuration subsystem.
    This will create a build.conf / build.host.conf if already existing.
    """
    global cmd_items, local_dict
    cmd_items = params

    if not os.path.exists('build.conf'):
	shutil.copyfile('config/' + params.get('ARCH', 'intel') + '/'
				  + params.get('SYSTEM', 'pc') + '/'
			          + params.get('CONF', 'build') + '.conf', 'build.conf')

    if not os.path.exists('build.host.conf'):
	shutil.copyfile('config/host/build.conf', 'build.host.conf')

    # Apply commandline arguments for use in build.conf's
    for key in params:
	if not key.startswith('HOST:'):
	    set_value(local_dict, key, params[key])

    local_dict.lock(True)
    apply_file('build.conf', target)

    local_dict = ConfDict()
    local_dict.lock(False, "HOST:")

    for key in params:
	if key.startswith('HOST:'):
	    set_value(local_dict, key[5:], params[key])

    local_dict.lock(True, "HOST:")
    apply_file('build.host.conf', host)

    # Apply default variables
    set_default_variables(target)
    set_default_variables(host)

def escape(obj):
    return str(obj).replace('"', '\\"').strip()

def write_header(env, filename = None):

    if not filename:
        if env['ARCH'] != 'host':
            path='config/'+env['ARCH']+'/'+env['SYSTEM']+'/System.h'

            try:
                os.makedirs(env['BUILDROOT'] + '/include')
                os.symlink('.', env['BUILDROOT'] + '/include/FreeNOS')
                shutil.copy(path, env['BUILDROOT'] + '/include/System.h')
            except Exception as e:
                pass

            try:
                os.makedirs('build/host/include')
                os.symlink('.', 'build/host/include/FreeNOS')
                shutil.copy(path, 'build/host/include/System.h')
            except:
                pass

        filename = env['BUILDROOT'] + '/include/Config.h'

    out = open(filename, "w")
    name, ext = os.path.splitext(filename)
    name = name.replace('/', '_')
    
    out.write('#ifndef __' + name.upper().replace('-', '_') + '_H\n')
    out.write('#define __' + name.upper().replace('-', '_') + '_H\n\n')
    out.write('#define VERSION_GET_CODE(a,b,c) (((a) << 16) + ((b) << 8) + (c))\n')
    out.write('#define DATETIME  __DATE__ " " __TIME__\n')
    out.write('#define COPYRIGHT "Copyright (C) ' + escape(datetime.datetime.today().year) + ' Niek Linnenbank\\r\\n" \\\n' + \
                                '"This is free software; see the source for copying conditions.  There is NO\\r\\n" \\\n' + \
                                '"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\\r\\n"\n')
    out.write('#define BANNER "FreeNOS " RELEASE " [" ARCH "/" SYSTEM "] (" BUILDUSER "@" BUILDHOST ") (" COMPILER_VERSION ") " DATETIME "\\r\\n" \n');

    for item in env.items():
	if type(item[1]) is str:
	    out.write("#define " + item[0].upper() + ' "' + escape(item[1]) + '"\n')
	elif type(item[1]) is int:
	    out.write("#define " + item[0].upper() + ' ' + str(item[1]) + '\n')
	elif type(item[1]) is list:
	    out.write("#define " + item[0].upper() + ' "')
	    for subitem in item[1]:
		if type(subitem) is str:
		    out.write(escape(subitem) + ' ')
	    out.write('"\n')

    out.write('#endif\n\n')
    out.close()

def set_default_variables(env):

    # Read the current version from the VERSION file.
    version = open('VERSION').read().strip()
    env['VERSION'] = version

    # Calculate a version integer code.
    current      = env['VERSION']
    expr         = re.compile("\.")
    version      = expr.split(current)
    versionCode  = 0
    versionPower = 16

    # Calculate version code.
    for v in version:
	versionCode  += int(v) * pow(2, versionPower)
	versionPower -= 8

    env['VERSIONCODE'] = versionCode

    # Set the full version revision in environments.
    try:
        env['RELEASE'] = env['VERSION'] + '-git(' + os.popen('git rev-parse --short HEAD').read().strip() + ')'
    except:
        env['RELEASE'] = env['VERSION']

    # Append extra version information
    if 'VERSION_EXTRA' in env:
        env['RELEASE'] += env['VERSION_EXTRA']

    # Mark with an asterisk if the user made any changes in the local git checkout (if any)
    try:
        if os.system('git diff|grep -i git 2>&1 > /dev/null') == 0:
            env['RELEASE'] += '*'
    except:
        pass

    # Attempt to retrieve the correct compiler version
    try:
        compiler = os.popen(env['CC'] + " --version | head -n 1").read().strip()
    except:
        compiler = env['CC'] + ' ' + env['CCVERSION']

    env['FREENOS'] = 1
    env['COMPILER_VERSION'] = compiler
    env['BUILDUSER'] = os.environ['USER']
    env['BUILDHOST'] = platform.node()
    env['BUILDOS'] = platform.system() + ' ' + platform.release()
    env['BUILDARCH'] = platform.machine()
    env['BUILDPY'] = "Python " + platform.python_version()
    env['BUILDER'] = "SCons " + SCons.__version__
    env['BUILDURL'] = os.getcwd()
    env['BUILDPATH'] = os.getcwd()

def set_value(env, key, value):
    """
    Apply new value to a environment.
    """
    if value in ('True', 'true'):
        env[key] = True
    elif value in ('False', 'false'):
        env[key] = False
    elif value.startswith('[') and value.endswith(']'):
        env[key] = eval(value)
    else:
        try:
            env[key] = int(value)
        except:
            env[key] = value

def apply_file(conf_file, env):
    """
    Apply a configuration on the given SCons environment
    """
    global local_dict
    result = parse_file(conf_file)

    for item in result:
	if type(result[item]) is list:
	    env[item] = eval_list(result[item], result)
	elif type(result[item]) is str:
	    env[item] = eval_string(result[item], result)
	else:
	    env[item] = result[item]

def parse_file(conf_file):
    """
    Parses a configuration file.
    Returns a dictionary with the parsed values.
    """
    global local_dict

    global_dict = { 'Include' : parse_file }
    config_file = eval_string(conf_file)

    execfile(config_file, global_dict, local_dict)
    return local_dict

def eval_list(lst, replace_dict = None):
    """
    Replace configuration item values in the given list.
    """
    new_lst = []

    for item in lst:
	new_lst.append(eval_string(item, replace_dict))

    return new_lst

def eval_string(string, replace_dict = None):
    """
    Replace configuration item values in the given string or list.
    """
    global local_dict

    if not replace_dict:
	replace_dict = local_dict

    split_string = string.split('}')
    final_string = ''

    for substr in split_string:
	idx = substr.find('${')
	if idx == -1:
	    final_string += substr
	else:
	    final_string += substr[0:idx]
	    var_name = substr[idx+2:]

	    if var_name in replace_dict:
		final_string += str(replace_dict[var_name])

    return final_string
