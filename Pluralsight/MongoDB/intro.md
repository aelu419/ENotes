# $\textrm{Introduction to MongoDB}$

- [$\textrm{Introduction to MongoDB}$](#textrmintroduction-to-mongodb)
- [Introduction](#introduction)
  - [Components](#components)
  - [Replica Sets](#replica-sets)
  - [Mongo Shell](#mongo-shell)
  - [Saving Data](#saving-data)
- [Finding Documents](#finding-documents)
  - [Query Conditions](#query-conditions)
- [Indexing](#indexing)
  - [Types of Indexes](#types-of-indexes)
  - [Manipulating Indexes](#manipulating-indexes)

# Introduction
## Components
- ``mongod``: mongo daemon
  - starting a service: ``mongod -f <config file url> --install``, ``net start mongodb``
  - stopping a service: ``net stop monbodb``
- ``mongo``: shell
  - connecting to service: under default settings, plain ``mongo`` will connect and launch shell

## Replica Sets
- purpose is to protect databases from crashes
- *def.* **primary**: the only replica in a replica set that accepts writes
- *def.* **secondary**: one of the replicas in a replica set that is read-only
  - replicas have their data replicated *from* primary
  - this is handled automatically and does not guarantee instant sync.
    - however, there is eventual sync.
- *def.* **arbiter**: an *empty* replica used solely to eliminate confusion in server succession
  - upon primary failure, the servers elect a secondary to become the new primary
  - the election uses *simple majority* vote
- *proc.* setting up a minimal, 3-set
  - ideally, all 3 replicas are on different machines
  - however, to run in the same machine, use different ports
  - for each:
    ```bash
    mkdir <dir>
    cd <dir>
    start "<name>" mongod --dbpath <db path> --port <port> --replSet "<rs name>"
    ```
  - to connect: ``mongo --port <port>``
  - to configure replica set
    - in ``mongo``: write *JS* to define a *JSON* object ``config``
    ```json
    "_id" : "demo",
    "members" : [
        { "_id": <id>, "host": "localhost: <portal>", "priority": <priority> },
        // ...
    ]
    ```
    - do ``rs.initiate(<config var name>)``
  - to use replica set
    - write: ``db.<dbname>.save({obj})`` saves to ``foo`` folder of the primary replica
    - read:
      - from primary replica, do ``db.<dbname>.find(<cond>)``
      - from secondary replica, do ``db.setSlaveOk()`` first
        - this just means that this particular db allows reading of secondaries
    - check current replica
      - ``db.getMongo()`` $\to$ ``... <port> ...``, which identifies the replica
      - to check overall health and identities of replicas, ``rs.status()``

## Mongo Shell
- interactively get insight from the server, often to check server status from outside the app
- shell is a javascript interpreter
  - can run scripts in shell, or type script within shell
- shell modes
  - ``mongo --eval <command>``: run command and then return to normal console
  - ``mongo --shell <command>``: run command interactively, returning to Mongo shell instead of console
  - ``mongo <script file>``: run script and then return to normal console
- editor
  - ``edit <func name>`` launches a configured editor to edit the internals
- load scripts into shell
  - automatically imported when ran: use ``rc`` file
    - to cancel, do ``mongo --norc`` to ignore the ``rc`` file 
  - ``load('<script>')``

## Saving Data
- no schema restrictions, document based
- *def.* **storage engine**
  - server is memory-mapped to the disk as a byte array
  - *def.* **BSON**: format for object storage
    - fast conversion to C datatypes
- *proc.* **saving**
  - document must have ``_id`` field
    - ``_id`` can be assigned with many data types, as long as not array
    - when ``_id`` is missing from the ``save`` command, Mongo assigns a generated id based on the other fields
      - the id contains a timestamp
    - ``<id>.getTimeStamp()`` retrieves the ISO date for the insertion date of the object
    - saving with the same ``_id`` will overwrite the old save
      - instead, using ``insert`` will throw ``duplicate key error``
  - documents are saved in *collections*
    - however, schema is not fixed per document
      - concise, support for complex scenarios
  - ``db.<collection name>.save({bson obj})``
  - if the collection name does not match, create a new collection and save it there
- *proc.* **updating**
  - concurrency management issue
    - $x \leftarrow x+1$
    - however, multiple writes like this could be done at the same time frame, instead of $+2$, it writes $+1$ twice
    - the solution is to use update instead of save
  - ``db.<collection>.update(<query>, <update>, <options>)``
    - ``options`` takes care of when ``query`` returns nothing, whether the update should save certain new documents in the collection
    - only one update concurrent
    - ``update``: ``{operator: {field, val}}``
      - *ex.* ``$inc`` operator for increment
      - field-wise: ``set, unset, rename, ...``
      - array: ``push``: add, ``pull``: remove all, ``pop``: remove first(-1) or last(1), ``addToSet``: add with no duplicate
    - multiple update: update but add parameter ``{multi: true}``
  - for strictly single-edit, use ``findAndModify`` instead of ``update``

# Finding Documents
- ``find(<cond>, <projection>)``, both params optional
  - ``projection`` will return *part of* the found document, to make sure it doesn't take too much data
## Query Conditions
- equal: ``{field:val}``
  - comparing to ``null`` will return all documents that either have the field as ``null`` or does not have that field
  - to avoid including documents not including a field, do ``$exists``
- comparison: ``{field:{<comp1>:val1},{<comp2>:val2}, ...}}``
  - ``$lt``, ``$lte``, ``$gt``, ``$gte``
- negation: ``{field:{$not:_}}``
- selection: ``{field:{$in{}:[..]}}``
  - by default, ``$in`` is OR
  - use ``$all`` to implement AND
- cascaded field: ``{"field.subfield":val}``

- *proc.*
  - query cursor will return a batch of documents per time over the cursor
    - basically an iterator
    - ``size()``, ``hasNext()``, ``forEach(<func>)``
    - for sorting: ``cursor.sort(field:order)``, where -1 is descending and 1 is ascending
  - ``limit(<max>)``
  - server-side paging: ``.skip(<max>).limit(<page_max>)``
  - find singular: ``.findOne(<query>)``

# Indexing
- normally, ``find`` goes through all items in the collection
- the solution is to use **index**
  - holds the pointer to  items with specific value at specific field
- documents could also be sorted by index
## Types of Indexes
  - "regular" b-tree index
  - geo (geographical)
  - text (for text queries and fields)
  - hashed (for sharding): spread documents evenly across shards
  - ttl (time to live): expiration date -> auto delete

## Manipulating Indexes
- create index
  - ``.ensureIndex(<keys>, <options>)``
  - *ex.*
    ```javascript
    db.animals.ensureIndex({name:1}) // this means to build index according to field "name", and have it in ascending order (1)
    db.system.indexes.find({ns:'test.animals'}, {key:1}) // this means to sort by key
    ```
  - how to check if an index is used
    - do ``.explain()`` after ``.find()``
    - check if ``cursor`` is ``basic``, if so then index is not used
  - to profile
    - check ``nscanned`` under ``.explain()``, the less the better since it means the data base is extracting less elements for its query
  - index queries can also be subject to comparisons
  - all collections have an immovable ``_id`` index
    - the ``name`` of this index is ``_id_``
  - deleting an unuseful index: ``.dropIndex(<name of index>)``
  - arbitrarily indexing embedded fields
    - *ex.* ``.ensureIndex({"info.color":1})``
  - array index
    - each element of the array will be indexed, and it is possible to get index by single element
      - *ex.* ``.ensureIndex({tag:1})``, ``.find({tag:"a"})``
  - unique index:
    - cannot insert duplicate index
  - sparse index:
    - ``{sparse:true}`` as additional parameter to ``.ensureIndex()``
    - only stores entries for documents *with* that field
    - when sorting by sparse index, omit documents without that index
  - compound index: multiple indices ANDed together, satisfied from left to right
  - sort direction:
    - left to right if multiple tag, each could be ascending or descending
  - if index is omitted during query, Mongo will optimize by using the index
    - for example, if ``.find()`` has ``{_id:0, <name of index>:1}``, it will directly return the pointed elements of the index without checking their ``id``
    - this is called a *covering index*
  - deadweight indexes
    - mongo does not check before ensuring an index
    - even if none of the documents have that index as field, mongo will wait for it to be and then generate index
  - compound index may violate the 128 character limit Mongo has for index
    - or, 31 levels of embedding