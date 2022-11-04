# Configuration
`TerminalCalculator`'s behavior and appearance can be configured using dedicated configuration files.

## File locations
### Unix
On Unix machines this file can either be `/home/{username}/.tcconfig` or `/home/{username}/.config/tc/config.yml`. The first location of the two is prioritized over the latter, resulting in the latter being ignored if the first one already is used.

### Windows
On Windows machines this file can either be `{homedrive}:\User\{username}\.tcconfig` or `{homedrive}:\User\{username}\.config\tc\config.yml`. The first of the two locations is prioritized over the latter, resulting in the latter being ignored if the first one already is used.

## Format
`TerminalCalculator`'s configuration files are composed in `YAML`.
A configuration file may look as follows:
```yaml
general:
    show_version: true

colors:
    result: 1
    command: 6
```

## Options

### General
Within the `general` node, following values can be changed:
|Name|Description|Default|
|----|-----------|-------|
|`show_version`|Show version number in title|`true`|

### Colors
`TerminalCalculator` uses `ncurses`' color definitions.
| | | | | | | | | |
|-|-|-|-|-|-|-|-|-|
|__Color__|Black|Blue|Green|Cyan|Red|Magenta|Yellow|White|
|__Value__|0|1|2|3|4|5|6|7|

Within the `colors` node, following values can be changed:
|Name|Description|Default|
|----|-----------|-------|
|`result`|Color of results|Green(1)|
|`command`|Color of commands|Yellow(6)|
