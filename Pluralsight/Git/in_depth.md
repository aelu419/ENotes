# $\textrm{Mastering Git}$

- [$\textrm{Mastering Git}$](#textrmmastering-git)
- [Introduction](#introduction)
- [Basic Workflow](#basic-workflow)
- [Advanced](#advanced)
  - [Stash](#stash)
  - [Merge](#merge)
  - [Paths](#paths)
  - [Hunks](#hunks)
  - [Switch](#switch)
  - [Restore](#restore)
  - [Git as Toolbox](#git-as-toolbox)
- [History](#history)
  - [Referencing commits](#referencing-commits)
  - [Fixing Past Commits](#fixing-past-commits)
- [Rewriting History on Large Scale](#rewriting-history-on-large-scale)
  - [Hard](#hard)
  - [Soft](#soft)
- [Setting Up Workflow](#setting-up-workflow)
  - [Distribution Model](#distribution-model)
  - [Branching Model](#branching-model)
  - [Constraints](#constraints)
  - [Gitflow](#gitflow)
  - [Customize Workflow](#customize-workflow)

# Introduction
- Working Area
  - local files
  - use ``git status`` or ``git diff`` to check difference against index
- Index
  - the index is a blob within ``.git``, but better visualized as a separate region
  - the in-between layer, "staging area"
  - commit towards the repository
  - use ``git diff --cached`` to check difference with repository
- Repository
  - the backup/deployment

# Basic Workflow
- ``git add`` from work area to index
  - note that ``git add <file>`` for a deleted file tells ``git`` that file has been deleted
- ``git commit`` from index to repository
- ``git checkout`` from repository to working area and index, while also pointing HEAD towards destination
- ``git rm`` deletes from repository AND index
- ``git rm --cached`` deletes from index
- ``git add <old_name>`` + ``git add <new name>`` accomplishes renaming (as well as directory change)
  - git figures out renaming and migrating based on content
- ``git mv <old> <new>`` is short for
  - ``mv <old> <new> && git add <new> && git rm <old>``
- ``git reset`` is *specifically* for moving a branch, while other commands move the branch but also does something else
  - it moves a branch to a specific commit
  - if HEAD *happens* to point to the moved branch, it will also move
  - ``--hard`` also resets work area and index
  - ``--mixed`` copies data to index, but not the work area
  - ``--soft`` don't touch work area or index
  - *case.* HEAD reset (i.e. ``git reset HEAD``) will un-stage all staged changes since the last commit
    - ``--hard`` HEAD reset will also clear the work area

# Advanced
## Stash
- ``git stash`` moves all the changes to *tracked* files that are not already in the index area, and stores then in the stash area; it also checks out the current commit
  - this is an abbreviation of ``git stash save``
  - ``--include-untracked`` will include *untracked* files as well
  - the check out will clear the stashed changes from the current repository
  - see the stash area as a clip board for the project, which can be indexed
- ``git stash list`` lists the stashes
- ``git stash apply`` applies the stashed changes to the work area and index

## Merge
- when a merge conflict happens, ``git`` stays in a merge
  - this means it create merge-related files under ``.git``, namely ``MERGE_HEAD``, ``MERGE_MODE``, ``MERGE_MSG``, etc.
  - these files are all temporary

## Paths
- ``git reset <branch> <path to file>``
  - for hard reset, do not do ``--hard``, do ``git checkout <branch> <path to file>``

## Hunks
- sub-file level changes
- ``git`` lists "hunks" from a file
  - there are options to split a hunk to even smaller hunks
  - use ``git add --patch <file>``, git will prompt about possible hunks as well as a list of commands
- ``--patch`` is also available in ``checkout``, ``stash``, ``reset``, etc.

## Switch
- one aspect of ``checkout``
- ``git switch <branch>`` points HEAD to the new branch

## Restore
- ``git restore <file>`` $\equiv$ hard reset (i.e. checkout)
- ``git restore --staged <file>`` $\equiv$ mixed reset

## Git as Toolbox
- each command is functionally simple, but is open to multi-use
- this is inspired by the Linux styling

# History
- formatting the log: ``git log --graph --decorate --oneline``
## Referencing commits
- ``<commit>^`` parent of the commit
- ``<commit>~n`` the n'th *generation* parent of the commit
  - ``<commit>~m`` the n'th parent (choosing from multi-parented commits)
## Fixing Past Commits
- ``git commit --amend`` will add newly staged changes to the last commit, as well as interactively change the commit message
  - in actuality, the commit is copied and then amended. the old commit is no longer referenced
- ``git rebase --interactive <commit>`` edits everything from the current commit up to the specified commit (exclusive)
  - ``pick`` = let commit be the same
  - ``squash`` = combine with previous commit
  - ``reword`` = edit commit msg
  - ``fixup`` = ``squash`` but discard msg
  - ``drop`` = remove commit
  - ``exec`` = run the following line in shell
  - note that these commands constitute a program that will be ran once the rebasing process begins
  - note that in actuality, ``pick``s create clones of the original commits
- ``git reflog`` tracks the movement of a pointer, such as ``HEAD``
  - ``reflog`` is completly local, meaning that the history is lost when pulling/cloning from another end
  - all hashes remain in ``reflog`` until gc

# Rewriting History on Large Scale
- change across multiple commits
  - *ex.* removing the same file across different commits
## Hard
- ``git filter-repo``
  - ``--path <file> --invert-paths`` removes file from all commits
  - note that everyone else need to clone again since their histories are not matched
## Soft
- ``git revert <commit>`` creates changes that are the exact opposite with the alterations made on commit
  - the old commit is still in, meaning that the rest of the team does not need to redo their local histories
- be careful when reverting merges since it creates confusing structure
  - the diagrams still show the merge happening, and it becomes confusing when you merge again after the revert

# Setting Up Workflow
- ``git`` is too free for team coordination, so certain agreements need to be set up
  1. Distribution Model: how many repositories to use and who has the permission to what
  2. Branching Model: which branches are used and how should they be treated
  3. Constraints: merge or rebase, when to push, commit message, etc.
- this topic is more social than technical

## Distribution Model
- **p2p**: everyone can see everyone else's repo, and none is authoritative
  - hard to decide whose right and who to release
- **centralized**: a *origin* everyone pulls and pushes to, where the *origin* does not have a physical working area (it is just a git repo)
  - here, peers do not have access to each other's changes
  - **PR**: an authorized user who reviews changes by contributors and decide how to edit *origin* accordingly
    - the maintainer usually has visibility (either direct or indirect) to other contributors
    - on Github, this is done through pull requests, which became the standardized method in the field
  - **Multi-level PR (Dictator & Lieutanants**: sub-projects take PR's from contributors; main project takes PR's from sub-projects, etc.
    - the naming comes from the Linux project that uses this model

## Branching Model
- *def.* **Stable Branch**: the tip of the branch must be working
- *def.* **Unstable Branch**: the tip of the branch might not be working
- *integration/main/master* branch is the central branch that people branch out from
  - in most projects, it is a goal to make sure that the *integration* branch is "mostly" stable
  - CICD is used for this purpose
- *release* branch (opt) aside from integration
  - *integration* merges into *release* every period, after more extensive tests are run
- *feature/topic* branches branching out from *integration* and merging back in after implmentation
- ``cherry-pick``: copying one specific commit
  - *hotfix* branch is a alternative way to share data between diverged branches

## Constraints
- a catch-all phrase for everything not falling into distribition and branching
- *ex.*
  - "always merge"/"always rebase"
  - "Only X can do Y on branch Z" (as additional enforcement to the actual Github permissions)
  - "Don't push on red"
    - using hooks to check and block accidental pushes
  - "Squash to single commit before merge"

## Gitflow
- centralized data model, but p2p when necessary (i.e. peers exchanging data)
- stable *main*, unstable *developer* (integration)
- stable *release* branch
- no rebasing
- the standardizations allow for automations that enforce and execute macros like "create hotfix"
- however, Gitflow may not be optimal
  - ex. frequent integration vs. feature branches and all-at-once merges

## Customize Workflow
- do not just "sit down and design"
- start small and "grow"
  - ex. specify distsribution, branching (simple), and constraints (minimal)
- when the scale goes out of control, or if issues occur, add/remove new rules *as response*
- complex rules give rise to bureaucracy; simple rules leave space for intelligent behavior
