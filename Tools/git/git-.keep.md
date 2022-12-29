简介

本文主要介绍在git中, .gitkeep的作用。

作用

git无法追踪一个空的文件夹，当用户需要追踪(track)一个空的文件夹的时候，按照惯例，大家会把一个称为.gitkeep的文件放在这些文件夹里。

例程

就个人而言，一般需要.gitkeep地方，是希望完成以下功能:

使git忽略一个文件夹下的所有文件，并保留该文件夹
```sh
# .gitignore 

# ignore all files in lib/
lib/*
# except for .gitkeep
!.gitkeep
# ignore TODO file in root directory,not subdir/TODO
/TODO
```
当.gitignore采用上面的写法时，git会忽略lib文件夹下除了.gitkeep外的所有文件。

