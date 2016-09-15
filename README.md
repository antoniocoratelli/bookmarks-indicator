# bookmarks-indicator
Bookmarks indicator for Ubuntu, written in Python.
Just put in `config` file all the folders you want to bookmark (one per line).
You can use `---` or `--- Some Header` where you want to place a menu separator.
Environment variables like `$HOME` are expanded.

![example](https://raw.githubusercontent.com/antoniocoratelli/bookmarks-indicator/master/res/animation.gif)

## requirements

This indicator requires the following modules:
- `os`
- `sys`
- `tempfile`
- `subprocess`
- `gtk`
- `appindicator`
- `argparse`

## usage
```
usage: bookmarks-indicator.py [-h] [-i COLOR] [-c CONFIG] [-l LABEL]
                              [-o OPENER]

optional arguments:
  -h, --help  show this help message and exit
  -i COLOR    icon color (default: '#010101')
  -c CONFIG   config file path (default: '<script-path>/config')
  -l LABEL    indicator label (default: '')
  -o OPENER   file opener (default: 'xdg-open')
  -t SHELL    shell (default: 'gnome-terminal')

Copyright (c) 2016, Antonio Coratelli.
Released under BSD 3-Clause License. See 'LICENSE' file.

Icon extracted from "WPZOOM Developer Icon Set" by WPZOOM.
http://www.wpzoom.com/wpzoom/new-freebie-wpzoom-developer-icon-set-154-free-icons
```

## support

If you like this application, you can [share it](https://www.addtoany.com/share/#url=github.com/antoniocoratelli/bookmarks-indicator), [buy me a coffe](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=GFD8AU9YJB36S), or just say thanks adding a [ star](https://github.com/antoniocoratelli/bookmarks-indicator/stargazers) :)
