#!/usr/bin/env python
import sys

def main():
  args = sys.argv[1:]
  if len(args) != 1:
    print 'Usage: splitCharsToNewLines.py fileName'
    exit

  f = open(args[0])
  for line in f:
    for char in line:
      print char

  return

if __name__ == '__main__':
  main()
