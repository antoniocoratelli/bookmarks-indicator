# bookmarks-indicator

Bookmarks indicator for Ubuntu, written in Python.

![example][animation]

> Think of it as a handy way to quickly open the files you use regularly,
> without needing to open a new file manager window, or manually hunt it
> down in the Unity Dash.
>
> _[OMG! Ubuntu!][omgubuntu] Post_

## Build and Install

The following instructions are for *Debian 9*, some changes might be 
required to install dependencies under different *Debian* versions or 
under *Ubuntu*.

```bash
sudo apt install build-essential cmake libgtk-3-dev libgtkmm-3.0-dev
git clone https://github.com/antoniocoratelli/bookmarks-indicator.git
mkdir -p bookmarks-indicator/build
cd bookmarks-indicator/build
cmake .. && make && sudo make install 
```

## Usage and Configuration

- Create a file named `nano $HOME/.bookmarks-indicator.cfg` 
- Populate it with all the folders and files you want to bookmark, one per line;
  environment variables like `$HOME` are expanded;
- Use `---` or `--- Some Header` where you want to place a menu separator;

Example of a configuration file:

```
$HOME/Documents
$HOME/Downloads
--- Media
$HOME/Music
$HOME/Pictures
$HOME/Videos
/media/user/DRIVE
---
$HOME/Documents/TodoList.txt
```

Run `bookmarks-indicator --help`:

```
bookmarks-indicator

A handy tool to quickly open the files you use regularly.


Usage: bookmarks-indicator [OPTIONS]

Options:
  -h,--help    Print this help message and exit
  -c TEXT      Configuration file path, default: '$HOME/.bookmarks-indicator.cfg'
  -o TEXT      Command to open regular files, default: 'xdg-open'
  -f TEXT      Command to open folders in explorer, default: 'xdg-open'
  -t TEXT      Command to open folders in terminal, default: 'gnome-terminal'
  -i TEXT      Icon of the indicator, can be a path or a gtk icon identifier, default: 'bookmark-new'
```

## Disclaimer

This indicator was developed under Debian 9 using XFCE desktop environment.
If you have troubles running the indicator on your system please open an issue and
we'll debug together.

```
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
```

[animation]: https://raw.githubusercontent.com/antoniocoratelli/bookmarks-indicator/master/res/animation.gif
[omgubuntu]: http://www.omgubuntu.co.uk/2016/09/simple-bookmarks-indicator-ubuntu-desktop
