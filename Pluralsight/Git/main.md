- [Introduction to Git](#introduction-to-git)
  - [Hashing](#hashing)
  - [Commit](#commit)
  - [Annotated Tag](#annotated-tag)
  - [Git as File System](#git-as-file-system)
- [Branching](#branching)
  - [Merging Branches](#merging-branches)
  - [Internals of Merging](#internals-of-merging)
  - [Other Types of Merging](#other-types-of-merging)
  - [Switch vs. Checkout](#switch-vs-checkout)
- [Rebasing](#rebasing)
  - [Simple Introduction](#simple-introduction)
  - [Implementation](#implementation)
  - [Merges vs. Rebases](#merges-vs-rebases)
  - [Elaboration of Tags](#elaboration-of-tags)
- [Distributed Version Control](#distributed-version-control)
  - [Local vs. Remote](#local-vs-remote)

# Introduction to Git
## Hashing
- hash: $obj \to SHA$
  - *ex. (unix)* ``echo "Apple Pie" | git hash-object --stdin``
    - sends the literal to git's hash
    - will print hash (SHA1) associated with the object
  - hash collision: practically not possible
- ``Git`` is a persistent hash map
  - ``$proj/.git/objects/[str]/[postfix = *]`` $\to$ binaries
    - here, the common prefix of many hashes, ``str``, becomes the folder name
    - the unique postfixes of hashes, ``postfix``, becomes the file name
      - *ex.** ``obj1:239918...`` ``obj2:23781a...``
        - they will be located in ``.git/objects/23``
        - their file name would be respectively ``9918...`` and ``781a..``
    - in ``git `` terminology, these binaries are called ``blob``s
    - 
  - ``git init`` is used to initialize the framework of ``.git/*`` within the project
## Commit
- a **commit** is a piece of text that git stores, containing hashes to ``tree``, ``author``, ``committee``, etc.
  - here, the ``tree`` points to the root directory, which in turn points to all its contents
  - for a subsequent commit, it has a *parent* that is the previous commit
  - the new commit is an entirely new tree node
    - if anything is changed, then a new hash is created and linked to the new tree
    - if not, then the hash remains the same and the new commit's tree points to the old hash
  - *ex.*
    - commit 1: $[Head1] \to ./ \to T1 \to A, B, C$, where $T1$ is the hash for the tree
    - we delete the file with hash $A$, and edit $B$ so it becomes $D$
    - commit 2: $[Head2] \to ./ \to T2 \to C, D$
    - at this time, ``objects`` contain all $A, B, C, D$
  - note that:
    - memory is saved due to storing everything at most once
      - further optimizations are used to compress and reduce saved information
    - the process is sped up by terminating hash-saving upon non-changing hash
      - *ex.* for linked hashes $F1 \to F2 \to F3 \to F4 \to A$, as long as none of them changed, ``git`` is going to point to $F1$ and just stop there
      - Otherwise, if $A$ changed, then the $F_i$ hashes would also change
## Annotated Tag
- tags with metadata
  - contains data and points to another ``commit``

## Git as File System
- basically a virtual file system
- similar to the linux file system, since it's written by the same guy
- a "content tracker system"

# Branching
- ``.git/ref/heads/[branch]`` blob
  - a branch is a reference to a commit
  - main/master branch is not really significant, since it's just another blob
- multiple branches could point to the same commit
- ``git branch`` lists all the branches
  - the current branch is marked ``*``
  - the relative address to the ``HEAD`` commit's blob is stored in the blob ``./git/HEAD``
  - *ex.* ``HEAD``: ``"ref: refs/heads/main"``
  - i.e. ``HEAD`` is a reference to a branch
  - to switch between branches, do ``git switch <branch>``: "move ``HEAD`` and update working directory to the content of it"

## Merging Branches
- upon conflict, ``git`` keeps track of a list of conflicts
- to resolve the conflict, the user needs to stage changes for *all* the aforementioned listed files
  - ``git`` will not watch over the actual files in the working directories and scan for things like ``>>>>> HEAD``
- a merge commit has two parents
- ``HEAD`` as well as the current branch now references the new commit

## Internals of Merging
-  ``commit-commit`` links are used to track history
-  ``content-content`` links are used to track content
-  ``git`` features focus on either, but usually not both
   -  features generally don't care about the working directory

## Other Types of Merging 
- fast-forwarding: guaranteed no conflict in-between, uses this method to optimize
- checking out commit: ``git checkout <hash of commit>``
  - here, ``HEAD`` points towards that commit directly, instead of any particular branch
  - this situation is called a *detached head*
- *ex.* start from ``main``, check out the current commit $C1$ (detaches head)
  - commit some changes $C2$, now, ``HEAD`` points to $C2$ while ``main`` to $C1$
  - do ``git switch main`` which points ``HEAD`` back to ``main``
  - now:
    - unless the hash for $C2$ is recorded, $C2$ becomes unreferenciable
    - $C2$ is now prone to git's garbage collection
## Switch vs. Checkout
- over time, checkout accumulated too many features
- ``switch`` is meant to be one aspect of ``checkout``, namely to point ``HEAD`` towards a *preexisting* branch
- creation of new branches is retained in ``checkout -b``

# Rebasing

## Simple Introduction
- ``git``'s "signature feature", as it is one of the few version control systems that support it
- *ex.* imagine the following commit tree
  - $C \to \{D1, E1\}$
  - $D1 \to D2 \to D3$
  - $E1 \to E2 \to E3$
  - ``HEAD`` $\to$ ``alt`` $\to E3$; ``main`` $\to D3$
  - this is essentially two diverging branches off of $C$
  - here, doing ``git rebase main`` creates the following tree
    - $C \to D1 \to D2 \to D3 \to E1 \to E2 \to E3$
    - ``HEAD`` $\to$ ``alt`` $\to$ $E3$
    - ``main`` $\to D3$
    - basically, it
        1. finds the first common ancestor $A$ ($A \equiv C$) between two ``commit``s
        2. detaches the current history w.r.t. $A$ and places it after the target ``commit``
- ``rebase`` also uses fast forwarding if the current ``commit`` is in the ancestry of the target ``commit``, and that they do not have conflicts

## Implementation
- there is no actual moving, since ``object`` database contents are immutable
- ``Git`` actual creates clones of the ``commit``s that differ only in their parentage
  - however, this difference is significant enough to guarantee different hashes, which allows them to be associated with unique blocks in the database
  - the old ``commit``s are left behind, just with references lost
    - if before the rebase, another branch is created to track the current ``commit``, then after the rebase these ``commit``s will still be referenced and thus not deleted
    - it not, old ``commit``s are prone to garbage collection

## Merges vs. Rebases
- **Merges**
  - never "lies": it preserves all the parentage of commits
  - however, this is also misleading without a GUI, since the history is more of a graph than a log; this creates possible confusion and misunderstanding (or non-understanding)
- **Rebases**
  - leaves linear history, more streamlined and "clean"
  - this tidied up history is not necessary correct, since the order of commits are not necesarily in the order in which they are made
  - may tamper with other git functions
- prefer merging unless there is a reason against it

## Elaboration of Tags
- creating regular (aka. lightweight) tag: ``git tag <tag_name>``
  - $Tag \to C$
- ceating annotated tag: ``git tag <tag_name> -a -m "msg"``
  - $Tag \to Obj\{msg, ...\} \to C$
- tags are stored in ``.git/refs/tags/<tag_name>``
  - each tag blob points towards a commit
- lightweight tags are basically just branches, but stay in place when committing ``HEAD``
  - their literal contents follow the exact same schema
- *tags do not move with rebase*

# Distributed Version Control
- ``git clone <url>`` copies the working directory as well as the ``./git`` folder
  - then, it checks out one of the branches
- ``git`` does not care about which version is more priviledged, a local copy is the same value with a cloud copy
  - all values are by convention, not design

## Local vs. Remote
- configuration in ``.git/config``
- the default remote is ``origin``, containing a ``url`` and a ``fetch`` describing which commit to fetch from
- ``git branch --all`` tracks remote branches
  - ``remotes/[remote name]/<branch name>``
  - will automatically update upon connection to the remote
  - tracked in ``./git/packed_refs`` blob
- ``git show-ref <branch>`` shows the commits that each branch points to
- 