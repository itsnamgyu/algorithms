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


def get_tokens(out_path):
    tokens = []
    with open(out_path) as f:
        tokens.append(f.readline())
        for _ in range(6):
            tokens.append(float(f.readline()))
    return tuple(tokens)


def tokens_match(a, b):
    for index in [0, 2, 3, 4, 5]:
        if a[index] != b[index]:
            return False
    else:
        return True


with cd('..'):
    subprocess.call('make')

in_format = 'in{:d}'
out_format = 'out{:d}'

in_paths = glob.glob('in*')
for in_path in in_paths:
    out_path = out_format.format(
        *parse.parse(in_format, in_path))
    answer = get_tokens(out_path);
    for index in [1, 2, 3]:
        subprocess.call(['../mss20161662', in_path, str(index)])
        result_path = 'result_{}'.format(in_path)
        result = get_tokens(result_path)
        if tokens_match(answer, result) and result[1] == index:
            print('[{}, {}] Success'.format(in_path, index))
        else:
            print('[{}, {}] Fail'.format(in_path, index))
            print('Answer: {}'.format(answer))
            print('Result: {}'.format(result))
    os.remove(result_path)
