#!/usr/bin/env python3

import os, sys
from getch import getch

menu1 = '''\

1. Checkout a frobbish.
2. Exonerate the state.
3. Foobar the glyph extension.
4. Ornerate four tabbish.
5. Undo the last 50,000 changes.
6. Go to the store.

Your Choice: '''

OS = sys.platform
OS = OS[0:3]
if OS == ('win'):
    cmd = 'cls'
else:
    cmd = 'clear'
k=-1

while k not in (range(49,55)):
    os.system(cmd)
    print(menu1, end='')
    k = getch()
    print(chr(k))

