A simple relational Database. 

It's a locally stored DB in the style of SQLite but uses multiple files (similar to e.g. Postgresql). 

1. Dir-structure


1. Dir-structure
The Dir-structure looks something like this: 

|DB_init.db<br />
|<br />
|--- DB-name<br />
&nbsp;&nbsp;&nbsp;&nbsp;|<br />
&nbsp;&nbsp;&nbsp;&nbsp;|--- table1.bin<br />
&nbsp;&nbsp;&nbsp;&nbsp;|--- table2.bin<br />
&nbsp;&nbsp;&nbsp;&nbsp;|--- index1.bin<br />
&nbsp;&nbsp;&nbsp;&nbsp;|--- index2.bin<br />


