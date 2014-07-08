MongoFSCache
=======

This program will tell you, roughly how much of a Linux machines memory (FS Cache included, hence the name) is being used by a single MongoDB database.

Due to the way MongoDB journalling works, often times data files are in memory but under filesystem cache and not resident memory.

MongoFS' output is unit selectable (pass in -b, -k, -m, or -g, for bytes, KB, MB, or GB respectivly) and can only be run on a single database at a time, specified by the DB.* in the notes below

## Compilation
gcc -o MongoFS MongoFS.c -lm

## Running 
./MongoFS -m /path/to/database/directory/DB.*

## Licence
Apache 2.0, as per it's big brother, [linux-ftools](https://code.google.com/p/linux-ftools/).
