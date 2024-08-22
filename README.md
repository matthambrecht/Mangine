earchMan
A search engine to find relevant linux/package commands for the input purpose. Leverages an embedding model to provide more accurate results than standard word searches utilized by manpages searches.

Usage
./searchman <arguments> <query>

Arguments
-t/—threshold <0.00-1.00>: Similarity threshold, the higher the value the more relevant the results will be. (Default: 0.35)
-r/—reload: Reloads the search index to account for new manpages for downloaded packages.
—stop: Stops the embedding service.
—start: Starts the embedding service.
How it Works
When SearchMan is installed it will go through your whole system, and scan every manpage through its contents into a retrieval pipeline. This pipeline will take each manpage, chunk it into parts, run an embedding model on the chunks to get a vector representation of the text, and place those chunks into a database with the associated command. When a user runs a query that will then be vectorized by the embedding model and a cosine similarity score is calculated between the chunks stored in the database. Relevant commands above the chosen or default threshold are then returned in an ordered list based on relevance with a relevance score. The user can then use the man command on these results or -h/—help parameter to learn a little more about each command to determine which best fits their need.