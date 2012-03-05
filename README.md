# j0llyDmper ?
## Overview of the beast
*j0llydmper* is a windows service that allows you to dump *furtively* and *automaticaly* some contents of USB disks *just plugged* in your computer.
In order to dump potentialy *interesting* files, you can use a rule on the file name or/and on the file size.

![j0llydmper-screen](http://img718.imageshack.us/img718/1846/j0llydmpr2.png)

## Configuration
The configuration file is very simple, it looks like that :

<pre>
[service_configuration]
name="j0llyDmpr"
desc="j0llyDmpr by 0vercl0k (0vercl0k.tuxfamily.org)."

[main_configuration]
patterns=".pwd;.dmpme"
max_size=1000
recurse_max=10
output_dir="C:\DUMPED\"
</pre>

## I wan't to compile it!
You will be able to compile *j0llydmper* with VC++ / C::B, the projects files are on the repo. In addition it's x64 compatible, yeah!


NB: Thanks to http://blog.tenablesecurity.com/2010/12/tenable-network-security-podcast-episode-62.html :).