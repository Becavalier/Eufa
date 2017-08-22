#!/usr/bin/python3

import argparse
import sys


def decode_sections(file):
    try:
        with open(file, 'rb') as raw:
            raw = raw.read()
    except IOError as exc:
        print("[Eufa-tools] Can't open input wasm module file: " + str(exc), file=sys.stderr)
        return


def __main__():
    parser = argparse.ArgumentParser()
    parser.add_argument('--module', type=str, help='The path of module file which need to be analyzed.')
    FLAGS, unparsed = parser.parse_known_args()

    if FLAGS.module is not None:
        decode_sections(FLAGS.module)

if __name__ == '__main__':
    __main__()

