# $\textrm{Workflow Techniques}$

- [$\textrm{Workflow Techniques}$](#textrmworkflow-techniques)
- [Configuration](#configuration)
  - [Config Files](#config-files)
  - [Attributes](#attributes)
- [Submodules](#submodules)
  - [Basic Setup](#basic-setup)
  - [Adding Submodule for the First Time](#adding-submodule-for-the-first-time)
  - [Adding Submodule from Clone](#adding-submodule-from-clone)
  - [Switching Between Submodule Branches](#switching-between-submodule-branches)
  - [Getting all Submodules in the Repo](#getting-all-submodules-in-the-repo)
  - [Practices](#practices)
- [Git Workflow](#git-workflow)
  - [Git Hooks Intro](#git-hooks-intro)
  - [Client Side Git Hooks](#client-side-git-hooks)
  - [Server Side Git Hooks](#server-side-git-hooks)
  - [Custom Git Commands](#custom-git-commands)
- [Bisect](#bisect)

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

# Submodules
## Basic Setup
- purpose: share git code across different projects
- a construct within git to enable a separate git folder to be considered *within* your own repo
  - *ex.* ``external/submoduleA``, ``external/submoduleA/submoduleA1``
- ``git submodule add <submodule><repo>``
  - ``submodule`` is a url
  - ``repo`` is the local path to store the submodule, within the project
  - the ``submodule add`` command modifies a blob called ``.gitmodules`` directly under repo, containing information about all submodules
- temporary removal of submodule
  - ``git submodule deinit <repo>``
- permanent removal of submodule
  - ``git submodule deinit <repo>``
  - ``git rm repo``
  - ``commit``
- submodules track to a specific commit, not any branch, meaning that other people updating the submodule repo would not directly impact your local repo
- note that ``git`` tracks submodules in the main repo, since the ``.gitmodules`` file is located there

## Adding Submodule for the First Time
- make directory for the external submodule
- use ``git submodule add``
  - this pulls things to local
- use ``git config --global status.submoduleSummary true`` to show full message
  - this will show things like "submodule changes to be committed"
- ``git add`` and ``git commit``

## Adding Submodule from Clone
- ``git submodule init`` to register the submodule at the designated directory
- ``git submodule update`` actually pulls the content of the submodule into the directory

## Switching Between Submodule Branches
- navigate to the submodule directory and do ``git fetch``, ``git checkout <branch>``
- navigate back to the main repo directory, and do ``git add`` ``git commit``

## Getting all Submodules in the Repo
- ``git clone --recursive <submodule> <repo>`` from ``repo/..``
  - this clones all the submodules of submodule into
- **when updating from peer**:
  1. ``git pull``
  2. ``git submodule sync --recursive``
     * this synchronizes changes
  3. ``git submodule update --init --recursive``
     * ``init`` takes care of new submodules that are added within the repo
     * ``recursive`` takes care of embedded submodules within submodules
     * this command is usually added to the pose-merge hook
- **when pushing to peer**:
  1. make changes in submodule
  2. ``git add``, ``git commit``, ``git push`` from the submodule directory
  3. then, push from the repo directory
  - to automate this, there is a config that automatically pushes all submodule changes before the main repo change
    - ``git push --recurse-submodules
  - note that if the main repo is pushed first, the 

## Practices
- avoid touching submodules in feature branches

# Git Workflow
## Git Hooks Intro
- *def.*: a predefined script that is run in reaction to certain events
  - client and server sides
- *proc.* from client to server push:
  - pre-commit: chooses to commit but before the commit message is actually generated
  - prepare-commit-msg: automatically generated commit message
  - commit-msg: the user-entered commit message
  - post-commit: commit already made
  - PUSH (not an actual hook)
  - pre-receive: handle what comes into the server
  - update: handles how the server gets updated
  - post-receive: after things have been updated
- **client side git hooks**: designed to improve workflow, not to enforce policies
  - often leveraged to run pre-commit checks
    - *ex.* linting, testing, verify no ``TODO``, cleaning up files
  - note that the client themself can edit those hooks, so do not put anything formal in there
- **server side git hooks**: enforcing code policies, signing, and identity

## Client Side Git Hooks
- navigate to ``.git/hooks``
  - remove ``.sample`` from hook sample file to let it take effect
  - each blob under ``hooks`` is essentially a bash script for git to run
  - ``set -e`` will block commits when things below it fail, such as ``npm run lint`` or ``npm test``
  - note that these hooks are all under local ``git``, which means it will not be shared
- to share local git hooks,
  - ``git config --local core.hooksPath <hook path>``
  - this will create a new directory at ``hook path``
  - move all local hooks to ``hook path``
  - ``git add``, etc. to stage and share changes

## Server Side Git Hooks
- test server side hooks by cloning a local directory and pushing there
  - ``remote/`` $\to$ ``local/``
- in ``remote/``:
  - go to ``.git/hooks`` and edit hooks associated with server-side events
- in ``local/``:
  - change something and push that change to ``remote/``
  - here, the hooks should activate

## Custom Git Commands
- create directory outside any repo to store custom git commands (*ex.* ``git-scripts``)
  - add ``git-scripts`` to PATH that Git Bash (or other shell) watches 
- inside the directory, create blob called ``git-...`` (*ex.* ``git-testscript``)
  - *ex.* submodule pull command
  - create blob ``git-subpull``
  - add commands in bash format
- go to an actual repo, and run ``git subpull``

# Bisect
- differentiate good and bad commits within the commit history
- ``git bisect start`` to start the process
- ``git besect reset`` to end the process
- ``git bisect run <command>`` to bisect the commit history based on the exit code of ``command``
  - *ex.* ``git bisect run npm test``
- it is also possible to do manual ``bisect``ing by labeling a commit
  - ``git bisect good``, ``git bisect bad``
  - based on the result, ``git bisect`` navigates to another commit in the history in a binary-search-like manner
- note that to kickstart the ``git besect`` process, you must first specify a beginning and ending
  - the beginning must be good, and is specified by ``git bisect good <commit ref>``
  - the ending must be bad, and is specified by ``git besect bad <commit ref>``
- tests outside the local repo will not be discarded when ``git bisect`` switches between commits
  - *ex.* ``git bisect run ../tests/test_feature_xyz.js``