# bookmarks-indicator
Bookmarks indicator for Ubuntu, written in Python.
Just put in `config` file all the folders you want to bookmark (one per line).
Environment variables like `$HOME` are expanded.

## usage
```
usage: bookmarks-indicator.py [-h] [-w] [-c CONFIG] [-l LABEL] [-o OPENER]

optional arguments:
  -h, --help  show this help message and exit
  -w          use white icon (default uses black icon)
  -c CONFIG   config file path (default: '<script-path>/config')
  -l LABEL    indicator label (default: '')
  -o OPENER   file opener (default: 'xdg-open')
```

![example](https://raw.githubusercontent.com/antoniocoratelli/bookmarks-indicator/master/res/animation.gif)
