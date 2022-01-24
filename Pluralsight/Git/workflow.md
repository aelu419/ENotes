# $\textrm{Workflow Techniques}$

- [$\textrm{Workflow Techniques}$](#textrmworkflow-techniques)
- [Configuration](#configuration)
  - [Config Files](#config-files)
  - [Attributes](#attributes)

# Configuration
## Config Files
- for repository configs $C_i$ at local level
  - ex. ``.git/config``
    - ``--local``
- for user account level $C_u$
  - ex. ``/Users/alice/.gitconfig``
    - ``--user``
- for system level (per git installation) $C$
  - ex. ``/usr/local/etc/gitconfig``
    - ``--global``
- ``git config --<level> <field> <value>``
- possible config files
  - identity
  - file editor
  - merge tools
  - ...
- ``git config --list`` prints all configs
- ``git config --<level> --unset/edit/remove-section`` can be used to edit configs
- environment variables can also affect git behavior, although not commonly used
- ``git difftool``, ``git mergetool`` will launch respective tools in the config
- set aliases for common commands

## Attributes
- specific functionality per file type
  - specify line ending
  - specify LFS application
  - specify binary files
    - ex. not doing the whole ``diff`` on binary files since it outputs nonsense
  - remove from export (``export-ignore``)
- smudge filter:
  - when checking out, replace placeholder with text
  - *ex.* stub to API key
- clean filter
  - when staging, replace text with placeholder
  - *ex.* API key (secret) to stub
- These filters make sure the staged edits are stable