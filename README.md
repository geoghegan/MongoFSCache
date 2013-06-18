MongoFS
=======

This program will tell you, roughly (and I mean roughly) how much of the Linux Filesystem Cache is being used by a single MongoDB database.

It's output is in bytes and can only be run on a single database at a time, specified by the DB.* in the notes below

## Compilation
gcc -o MongoFS MongoFS.c -lm

## Running 
./MongoFS /path/to/database/directory/DB.*

## Licence
Apache 2.0, as per it's big brother, [linux-ftools](https://code.google.com/p/linux-ftools/), of which this is a fork
