# bookmarks-indicator

Bookmarks indicator for Ubuntu, written in Python.

![example][animation]

> Think of it as a handy way to quickly open the files you use regularly,
> without needing to open a new file manager window, or manually hunt it
> down in the Unity Dash.
>
> _[OMG! Ubuntu!][omgubuntu] Post_

## Brief Guide

- Put in `config` all the folders and files you want to bookmark, one per line;
  environment variables like `$HOME` are expanded;
- Use `---` or `--- Some Header` where you want to place a menu separator;
- Open a terminal in the folder containing `bookmarks-indicator.py`;
- Run `python bookmarks-indicator.py`.

Example of a `config` file:

    $HOME/Documents
    $HOME/Downloads
    --- Media
    $HOME/Music
    $HOME/Pictures
    $HOME/Videos
    ---
    $HOME/Documents/TodoList.txt

## Requirements

This indicator requires the following python modules: `os`, `sys`, `tempfile`,
`subprocess`, `gtk`, `appindicator`, `argparse`.
You can install the missing ones using `pip` ([link][pip]).

## Advanced Usage

    usage: bookmarks-indicator.py [OPTIONS]

    optional arguments:
      -h, --help  show this help message and exit
      -i COLOR    icon color (default: '#010101')
      -c CONFIG   config file path (default: '<script-path>/config')
      -l LABEL    indicator label (default: '')
      -o OPENER   file opener (default: 'xdg-open')
      -t SHELL    shell (default: 'gnome-terminal')
      -x EXT      execute -- instead of open in editor -- files with chosen
                  extension (default: disabled) for example '-x ".sh" -x ".py"'
                  will execute bash and python scripts (ensure they have file
                  execution permission)

    Copyright (c) 2016, Antonio Coratelli.
    Released under BSD 3-Clause License. See 'LICENSE' file.

    Icon extracted from "WPZOOM Developer Icon Set" by WPZOOM.
    http://www.wpzoom.com/wpzoom/new-freebie-wpzoom-developer-icon-set-154-free-icons

## Support

If you like this application, you can [share it][support_share],
[buy me a coffe][support_paypal], or just say thanks adding a
[star][support_star] :)


[animation]: https://raw.githubusercontent.com/antoniocoratelli/bookmarks-indicator/master/res/animation.gif
[omgubuntu]: http://www.omgubuntu.co.uk/2016/09/simple-bookmarks-indicator-ubuntu-desktop
[pip]: https://wiki.python.org/moin/CheeseShopTutorial#Installing_Distributions

[support_share]:  https://www.addtoany.com/share/#url=github.com/antoniocoratelli/bookmarks-indicator
[support_star]:   https://github.com/antoniocoratelli/bookmarks-indicator/stargazers
[support_paypal]: https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=GFD8AU9YJB36S
