# Mangine

A search engine to find relevant linux/package commands for the input purpose. Leverages an embedding model to provide more accurate results than standard word searches utilized by manpages searches.

### How it Works

When Mangine is installed it will go through your whole system, and scan every manpage through its contents into a processing pipeline. This pipeline will take each manpage, chunk it into parts, and store it in a database. This allows the user to run a query which will score our commands based on relevence.

### TODO
- [ ] Switch from Embedding Model to Traditional BM25 Search
- [ ] Turn into a webserver or leverage piping (probably the latter, why open a port???)
- [ ] Why is everything a class? Fix that
- [ ] Integrate BM25 from Scratch
- [ ] Remove Python Webserver
- [ ] Move over to MySQL from Postgres
- [ ] Clean up install script
- [ ] Clean up Unit Testing
- [ ] Add Ncurses GUI for pivoting
- [ ] Maybe move back towards AI/ML in the future? Don't really want Python though
