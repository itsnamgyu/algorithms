import os
import subprocess
import glob
import parse


class cd:
    """
    Context manager for changing the current working directory.
    From https://stackoverflow.com/a/13197763.
    """
    def __init__(self, newPath):
        self.newPath = os.path.expanduser(newPath)

    def __enter__(self):
        self.savedPath = os.getcwd()
        os.chdir(self.newPath)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.savedPath)


def get_tokens(path):
    tokens = []
    with open(path) as f:
        tokens.append(f.readline())
        for _ in range(6):
            tokens.append(float(f.readline()))
    return tuple(tokens)

with cd('..'):
    subprocess.call('make generate'.split())
    subprocess.call('make')

in_paths = glob.glob('input*')
in_paths.sort()

with open('results.csv', 'w') as f:
    f.write('file,n2,nlogn,n,\n')
    for in_path in in_paths:
        f.write('in_path,')
        for index in [1, 2, 3]:
            subprocess.call(['../mss20161662', in_path, str(index)])
            result_path = 'result_{}'.format(in_path)
            result = get_tokens(result_path)
            print('[{}, {}] {}'.format(in_path, index, result[6]))
            f.write(str(result[6]) + ',')
        os.remove(result_path)
        f.write('\n')
