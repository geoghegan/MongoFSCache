MongoFS
=======

This program will tell you, roughly (and I mean roughly) how much of the Linux Filesystem Cache is being used by a single MongoDB.

It's output is in bytes and can only be run on a single collection, specified by the COLLECTION.* in the notes below

## Compilation
gcc -o MongoFS MongoFS.c -lm

## Running 
./MongoFS /path/to/database/directory/COLLECTION.*

