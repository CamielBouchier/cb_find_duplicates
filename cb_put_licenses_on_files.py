# -*- coding: utf-8 -*-

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#
# $BeginLicense$
#
# (C) 2015-2021 by Camiel Bouchier (camiel@bouchier.be)
#
# This file is part of cb_find_duplicates.
# All rights reserved.
# You are granted a non-exclusive and non-transferable license to use this
# software for personal or internal business purposes.
#
# THIS SOFTWARE IS PROVIDED "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL Camiel Bouchier BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# $EndLicense$
#
#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

import sys

if not 'mingw64' in sys.executable:
    print("This script should run the mingw64 python.")
    sys.exit(1)

if sys.version_info[0] != 3 or sys.version_info[1] < 6:
    print("This script requires Python version 3.6.")
    sys.exit(1)

import logging
logger = logging.getLogger(__name__)

import os
import re

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

program_name = "cb_put_licenses_on_files"

license_txt = [
    '',
    '(C) 2015-2021 by Camiel Bouchier (camiel@bouchier.be)',
    '',
    'This file is part of cb_find_duplicates.',
    'All rights reserved.',
    ''
    'You are granted a non-exclusive and non-transferable license to use this',
    'software for personal or internal business purposes.',
    '',
    'THIS SOFTWARE IS PROVIDED "AS IS" AND',
    'ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED',
    'WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE',
    'DISCLAIMED. IN NO EVENT SHALL Camiel Bouchier BE LIABLE FOR ANY',
    'DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES',
    '(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;',
    'LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND',
    'ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT',
    '(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS',
    'SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.',
    '']


#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

def generate_files(root, extensions_to_include=[], regexes_to_exclude=[]) :

    for path, dirs, files in os.walk(root) :
        for the_file in files :

            relative_name = os.path.join(path, the_file).replace('\\', '/')

            if extensions_to_include :
                (filename, extension) = os.path.splitext(the_file)
                if extension not in extensions_to_include :
                    continue

            # logger.info(f"including \'{relative_name}\'")

            excluded_due_to_regex = False
            for regex in regexes_to_exclude :
                if re.match(regex, relative_name) :
                    # logger.info(f"excluding \'{relative_name}\' due to \'{regex}\'")
                    excluded_due_to_regex = True
                    break
            if excluded_due_to_regex :
                continue

            yield os.path.abspath(relative_name).replace('\\', '/')

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

def handle_file(filename) :

    logger.info(f"Handling file {filename}")

    with open(filename, encoding="utf-8") as f :
        file_lines = f.readlines()

    has_begin_license = False
    has_end_license = False

    for line in file_lines :
        if not has_begin_license and re.match(r'.{0,5}\$BeginLicense\$', line) :
            has_begin_license = True
            begin_license_line = line
        if has_begin_license and re.match(r'.{0,5}\$EndLicense\$', line) :
            has_end_license = True
            end_license_line = line
            break

    if not has_begin_license :
        logger.info("No 'BeginLicense' found in '{}'".format(filename))
        return

    if has_begin_license and not has_end_license :
        logger.info("No 'EndLicense' found in '{}'".format(filename))
        return

    with open(filename, "w", encoding="utf-8", newline='\n') as f :
        skipping = False
        for line in file_lines :
            if not skipping :
                f.write(line.rstrip()+'\n') # Implicit trailing blanks removal.
            if line == end_license_line :
                f.write(line)
                skipping = False
            if line == begin_license_line :
                skipping = True
                for X in license_txt :
                    f.write(begin_license_line.replace('$BeginLicense$', X).rstrip()+'\n')

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

def install_logger() :

    root_logger = logging.getLogger()
    root_logger.setLevel(logging.DEBUG)

    console_handler = logging.StreamHandler()
    console_handler.setLevel(logging.INFO)
    console_format = "%(filename)32s:%(lineno)5s : %(message)s"
    console_formatter = logging.Formatter(console_format)
    console_handler.setFormatter(console_formatter)

    root_logger.addHandler(console_handler)

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

if __name__ == '__main__':

    install_logger()
    logger.info("Starting {}".format(program_name))

    dir_todo = '.'
    extensions_todo = [
        '',
        '.ac',
        '.bat',
        '.cpp',
        '.h',
        '.in',
        '.lua',
        '.m4',
        '.nsi',
        '.pro',
        '.py',
        '.spec',
        '.txt',
        ]
    regex_exclude = [
        r'.*/.git.*',
        r'.*/autom4te.cache/.*',
        r'.*/cb_find_duplicates-cppcheck-build-dir.cache/.*',
        r'.*/debug/.*',
        r'.*/dist_windows/.*',
        r'.*/moc/.*',
        r'.*/objects_debug/.*',
        r'.*/objects_release/.*',
        r'.*/release/.*',
        r'.*/resources/.*',
        ]

    for the_file in generate_files(dir_todo, extensions_todo, regex_exclude) :
        handle_file(f"{the_file}")

    logger.info("Ending {}".format(program_name))

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

# vim: syntax=python ts=4 sw=4 sts=4 sr et columns=100 lines=45
